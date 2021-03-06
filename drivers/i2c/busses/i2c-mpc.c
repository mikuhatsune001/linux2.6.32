/*
 * (C) Copyright 2003-2004
 * Humboldt Solutions Ltd, adrian@humboldt.co.uk.

 * This is a combined i2c adapter and algorithm driver for the
 * MPC107/Tsi107 PowerPC northbridge and processors that include
 * the same I2C unit (8240, 8245, 85xx).
 *
 * Release 0.8
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/of_i2c.h>

#include <linux/io.h>
#include <linux/fsl_devices.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#include <asm/mpc52xx.h>
#include <sysdev/fsl_soc.h>

#define DRV_NAME "mpc-i2c"

#define MPC_I2C_FDR   0x04
#define MPC_I2C_CR    0x08
#define MPC_I2C_SR    0x0c
#define MPC_I2C_DR    0x10
#define MPC_I2C_DFSRR 0x14

#define CCR_MEN  0x80
#define CCR_MIEN 0x40
#define CCR_MSTA 0x20
#define CCR_MTX  0x10
#define CCR_TXAK 0x08
#define CCR_RSTA 0x04

#define CSR_MCF  0x80
#define CSR_MAAS 0x40
#define CSR_MBB  0x20
#define CSR_MAL  0x10
#define CSR_SRW  0x04
#define CSR_MIF  0x02
#define CSR_RXAK 0x01

struct mpc_i2c {
	struct device *dev;
	void __iomem *base;
	u32 interrupt;
	wait_queue_head_t queue;
	struct i2c_adapter adap;
	int irq;
};

struct mpc_i2c_divider {
	u16 divider;
	u16 fdr;	/* including dfsrr */
};

struct mpc_i2c_match_data {
	void (*setclock)(struct device_node *node,
			 struct mpc_i2c *i2c,
			 u32 clock, u32 prescaler);
	u32 prescaler;
};

static inline void writeccr(struct mpc_i2c *i2c, u32 x)
{
	writeb(x, i2c->base + MPC_I2C_CR);
}

static irqreturn_t mpc_i2c_isr(int irq, void *dev_id)
{
	struct mpc_i2c *i2c = dev_id;
	if (readb(i2c->base + MPC_I2C_SR) & CSR_MIF) {
		/* Read again to allow register to stabilise */
		i2c->interrupt = readb(i2c->base + MPC_I2C_SR);
		writeb(0, i2c->base + MPC_I2C_SR);
		wake_up(&i2c->queue);
	}
	return IRQ_HANDLED;
}

/* Sometimes 9th clock pulse isn't generated, and slave doesn't release
 * the bus, because it wants to send ACK.
 * Following sequence of enabling/disabling and sending start/stop generates
 * the pulse, so it's all OK.
 */
static void mpc_i2c_fixup(struct mpc_i2c *i2c)
{
	writeccr(i2c, 0);
	udelay(30);
	writeccr(i2c, CCR_MEN);
	udelay(30);
	writeccr(i2c, CCR_MSTA | CCR_MTX);
	udelay(30);
	writeccr(i2c, CCR_MSTA | CCR_MTX | CCR_MEN);
	udelay(30);
	writeccr(i2c, CCR_MEN);
	udelay(30);
}

static int i2c_wait(struct mpc_i2c *i2c, unsigned timeout, int writing)
{
	unsigned long orig_jiffies = jiffies;
	u32 x;
	int result = 0;

	if (i2c->irq == NO_IRQ) {
		while (!(readb(i2c->base + MPC_I2C_SR) & CSR_MIF)) {
			schedule();
			if (time_after(jiffies, orig_jiffies + timeout)) {
				dev_dbg(i2c->dev, "timeout\n");
				writeccr(i2c, 0);
				result = -EIO;
				break;
			}
		}
		x = readb(i2c->base + MPC_I2C_SR);
		writeb(0, i2c->base + MPC_I2C_SR);
	} else {
		/* Interrupt mode */
		result = wait_event_timeout(i2c->queue,
			(i2c->interrupt & CSR_MIF), timeout);

		if (unlikely(!(i2c->interrupt & CSR_MIF))) {
			dev_dbg(i2c->dev, "wait timeout\n");
			writeccr(i2c, 0);
			result = -ETIMEDOUT;
		}

		x = i2c->interrupt;
		i2c->interrupt = 0;
	}

	if (result < 0)
		return result;

	if (!(x & CSR_MCF)) {
		dev_dbg(i2c->dev, "unfinished\n");
		return -EIO;
	}

	if (x & CSR_MAL) {
		dev_dbg(i2c->dev, "MAL\n");
		return -EIO;
	}

	if (writing && (x & CSR_RXAK)) {
		dev_dbg(i2c->dev, "No RXAK\n");
		/* generate stop */
		writeccr(i2c, CCR_MEN);
		return -EIO;
	}
	return 0;
}

#ifdef CONFIG_PPC_MPC52xx
static const struct mpc_i2c_divider mpc_i2c_dividers_52xx[] = {
	{20, 0x20}, {22, 0x21}, {24, 0x22}, {26, 0x23},
	{28, 0x24}, {30, 0x01}, {32, 0x25}, {34, 0x02},
	{36, 0x26}, {40, 0x27}, {44, 0x04}, {48, 0x28},
	{52, 0x63}, {56, 0x29}, {60, 0x41}, {64, 0x2a},
	{68, 0x07}, {72, 0x2b}, {80, 0x2c}, {88, 0x09},
	{96, 0x2d}, {104, 0x0a}, {112, 0x2e}, {120, 0x81},
	{128, 0x2f}, {136, 0x47}, {144, 0x0c}, {160, 0x30},
	{176, 0x49}, {192, 0x31}, {208, 0x4a}, {224, 0x32},
	{240, 0x0f}, {256, 0x33}, {272, 0x87}, {288, 0x10},
	{320, 0x34}, {352, 0x89}, {384, 0x35}, {416, 0x8a},
	{448, 0x36}, {480, 0x13}, {512, 0x37}, {576, 0x14},
	{640, 0x38}, {768, 0x39}, {896, 0x3a}, {960, 0x17},
	{1024, 0x3b}, {1152, 0x18}, {1280, 0x3c}, {1536, 0x3d},
	{1792, 0x3e}, {1920, 0x1b}, {2048, 0x3f}, {2304, 0x1c},
	{2560, 0x1d}, {3072, 0x1e}, {3584, 0x7e}, {3840, 0x1f},
	{4096, 0x7f}, {4608, 0x5c}, {5120, 0x5d}, {6144, 0x5e},
	{7168, 0xbe}, {7680, 0x5f}, {8192, 0xbf}, {9216, 0x9c},
	{10240, 0x9d}, {12288, 0x9e}, {15360, 0x9f}
};

int mpc_i2c_get_fdr_52xx(struct device_node *node, u32 clock, int prescaler)
{
	const struct mpc_i2c_divider *div = NULL;
	unsigned int pvr = mfspr(SPRN_PVR);
	u32 divider;
	int i;

	if (!clock)
		return -EINVAL;

	/* Determine divider value */
	divider = mpc5xxx_get_bus_frequency(node) / clock;

	/*
	 * We want to choose an FDR/DFSR that generates an I2C bus speed that
	 * is equal to or lower than the requested speed.
	 */
	for (i = 0; i < ARRAY_SIZE(mpc_i2c_dividers_52xx); i++) {
		div = &mpc_i2c_dividers_52xx[i];
		/* Old MPC5200 rev A CPUs do not support the high bits */
		if (div->fdr & 0xc0 && pvr == 0x80822011)
			continue;
		if (div->divider >= divider)
			break;
	}

	return div ? (int)div->fdr : -EINVAL;
}

static void mpc_i2c_setclock_52xx(struct device_node *node,
				  struct mpc_i2c *i2c,
				  u32 clock, u32 prescaler)
{
	int ret, fdr;

	ret = mpc_i2c_get_fdr_52xx(node, clock, prescaler);
	fdr = (ret >= 0) ? ret : 0x3f; /* backward compatibility */

	writeb(fdr & 0xff, i2c->base + MPC_I2C_FDR);

	if (ret >= 0)
		dev_info(i2c->dev, "clock %d Hz (fdr=%d)\n", clock, fdr);
}
#else /* !CONFIG_PPC_MPC52xx */
static void mpc_i2c_setclock_52xx(struct device_node *node,
				  struct mpc_i2c *i2c,
				  u32 clock, u32 prescaler)
{
}
#endif /* CONFIG_PPC_MPC52xx*/

#ifdef CONFIG_FSL_SOC
static const struct mpc_i2c_divider mpc_i2c_dividers_8xxx[] = {
	{160, 0x0120}, {192, 0x0121}, {224, 0x0122}, {256, 0x0123},
	{288, 0x0100}, {320, 0x0101}, {352, 0x0601}, {384, 0x0102},
	{416, 0x0602}, {448, 0x0126}, {480, 0x0103}, {512, 0x0127},
	{544, 0x0b03}, {576, 0x0104}, {608, 0x1603}, {640, 0x0105},
	{672, 0x2003}, {704, 0x0b05}, {736, 0x2b03}, {768, 0x0106},
	{800, 0x3603}, {832, 0x0b06}, {896, 0x012a}, {960, 0x0107},
	{1024, 0x012b}, {1088, 0x1607}, {1152, 0x0108}, {1216, 0x2b07},
	{1280, 0x0109}, {1408, 0x1609}, {1536, 0x010a}, {1664, 0x160a},
	{1792, 0x012e}, {1920, 0x010b}, {2048, 0x012f}, {2176, 0x2b0b},
	{2304, 0x010c}, {2560, 0x010d}, {2816, 0x2b0d}, {3072, 0x010e},
	{3328, 0x2b0e}, {3584, 0x0132}, {3840, 0x010f}, {4096, 0x0133},
	{4608, 0x0110}, {5120, 0x0111}, {6144, 0x0112}, {7168, 0x0136},
	{7680, 0x0113}, {8192, 0x0137}, {9216, 0x0114}, {10240, 0x0115},
	{12288, 0x0116}, {14336, 0x013a}, {15360, 0x0117}, {16384, 0x013b},
	{18432, 0x0118}, {20480, 0x0119}, {24576, 0x011a}, {28672, 0x013e},
	{30720, 0x011b}, {32768, 0x013f}, {36864, 0x011c}, {40960, 0x011d},
	{49152, 0x011e}, {61440, 0x011f}
};

u32 mpc_i2c_get_sec_cfg_8xxx(void)
{
	struct device_node *node = NULL;
	u32 __iomem *reg;
	u32 val = 0;

	node = of_find_node_by_name(NULL, "global-utilities");
	if (node) {
		const u32 *prop = of_get_property(node, "reg", NULL);
		if (prop) {
			/*
			 * Map and check POR Device Status Register 2
			 * (PORDEVSR2) at 0xE0014
			 */
			reg = ioremap(get_immrbase() + *prop + 0x14, 0x4);
			if (!reg)
				printk(KERN_ERR
				       "Error: couldn't map PORDEVSR2\n");
			else
				val = in_be32(reg) & 0x00000080; /* sec-cfg */
			iounmap(reg);
		}
	}
	if (node)
		of_node_put(node);

	return val;
}

int mpc_i2c_get_fdr_8xxx(struct device_node *node, u32 clock, u32 prescaler)
{
	const struct mpc_i2c_divider *div = NULL;
	u32 divider;
	int i;

	if (!clock)
		return -EINVAL;

	/* Determine proper divider value */
	if (of_device_is_compatible(node, "fsl,mpc8544-i2c"))
		prescaler = mpc_i2c_get_sec_cfg_8xxx() ? 3 : 2;
	if (!prescaler)
		prescaler = 1;

	divider = fsl_get_sys_freq() / clock / prescaler;

	pr_debug("I2C: src_clock=%d clock=%d divider=%d\n",
		 fsl_get_sys_freq(), clock, divider);

	/*
	 * We want to choose an FDR/DFSR that generates an I2C bus speed that
	 * is equal to or lower than the requested speed.
	 */
	for (i = 0; i < ARRAY_SIZE(mpc_i2c_dividers_8xxx); i++) {
		div = &mpc_i2c_dividers_8xxx[i];
		if (div->divider >= divider)
			break;
	}

	return div ? (int)div->fdr : -EINVAL;
}

static void mpc_i2c_setclock_8xxx(struct device_node *node,
				  struct mpc_i2c *i2c,
				  u32 clock, u32 prescaler)
{
	int ret, fdr;

	ret = mpc_i2c_get_fdr_8xxx(node, clock, prescaler);
	fdr = (ret >= 0) ? ret : 0x1031; /* backward compatibility */

	writeb(fdr & 0xff, i2c->base + MPC_I2C_FDR);
	writeb((fdr >> 8) & 0xff, i2c->base + MPC_I2C_DFSRR);

	if (ret >= 0)
		dev_info(i2c->dev, "clock %d Hz (dfsrr=%d fdr=%d)\n",
			 clock, fdr >> 8, fdr & 0xff);
}

#else /* !CONFIG_FSL_SOC */
static void mpc_i2c_setclock_8xxx(struct device_node *node,
				  struct mpc_i2c *i2c,
				  u32 clock, u32 prescaler)
{
}
#endif /* CONFIG_FSL_SOC */

static void mpc_i2c_start(struct mpc_i2c *i2c)
{
	/* Clear arbitration */
	writeb(0, i2c->base + MPC_I2C_SR);
	/* Start with MEN */
	writeccr(i2c, CCR_MEN);
}

static void mpc_i2c_stop(struct mpc_i2c *i2c)
{
	writeccr(i2c, CCR_MEN);
}

static int mpc_write(struct mpc_i2c *i2c, int target,
		     const u8 *data, int length, int restart)
{
	int i, result;
	unsigned timeout = i2c->adap.timeout;
	u32 flags = restart ? CCR_RSTA : 0;

	/* Start as master */
	writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_MTX | flags);
	/* Write target byte */
	writeb((target << 1), i2c->base + MPC_I2C_DR);

	result = i2c_wait(i2c, timeout, 1);
	if (result < 0)
		return result;

	for (i = 0; i < length; i++) {
		/* Write data byte */
		writeb(data[i], i2c->base + MPC_I2C_DR);

		result = i2c_wait(i2c, timeout, 1);
		if (result < 0)
			return result;
	}

	return 0;
}

static int mpc_read(struct mpc_i2c *i2c, int target,
		    u8 *data, int length, int restart)
{
	unsigned timeout = i2c->adap.timeout;
	int i, result;
	u32 flags = restart ? CCR_RSTA : 0;

	/* Switch to read - restart */
	writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_MTX | flags);
	/* Write target address byte - this time with the read flag set */
	writeb((target << 1) | 1, i2c->base + MPC_I2C_DR);

	result = i2c_wait(i2c, timeout, 1);
	if (result < 0)
		return result;

	if (length) {
		if (length == 1)
			writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_TXAK);
		else
			writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA);
		/* Dummy read */
		readb(i2c->base + MPC_I2C_DR);
	}

	for (i = 0; i < length; i++) {
		result = i2c_wait(i2c, timeout, 0);
		if (result < 0)
			return result;

		/* Generate txack on next to last byte */
		if (i == length - 2)
			writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_TXAK);
		/* Do not generate stop on last byte */
		if (i == length - 1)
			writeccr(i2c, CCR_MIEN | CCR_MEN | CCR_MSTA | CCR_MTX);
		data[i] = readb(i2c->base + MPC_I2C_DR);
	}

	return length;
}

static int mpc_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
{
	struct i2c_msg *pmsg;
	int i;
	int ret = 0;
	unsigned long orig_jiffies = jiffies;
	struct mpc_i2c *i2c = i2c_get_adapdata(adap);

	mpc_i2c_start(i2c);

	/* Allow bus up to 1s to become not busy */
	while (readb(i2c->base + MPC_I2C_SR) & CSR_MBB) {
		if (signal_pending(current)) {
			dev_dbg(i2c->dev, "Interrupted\n");
			writeccr(i2c, 0);
			return -EINTR;
		}
		if (time_after(jiffies, orig_jiffies + HZ)) {
			dev_dbg(i2c->dev, "timeout\n");
			if (readb(i2c->base + MPC_I2C_SR) ==
			    (CSR_MCF | CSR_MBB | CSR_RXAK))
				mpc_i2c_fixup(i2c);
			return -EIO;
		}
		schedule();
	}

	for (i = 0; ret >= 0 && i < num; i++) {
		pmsg = &msgs[i];
		dev_dbg(i2c->dev,
			"Doing %s %d bytes to 0x%02x - %d of %d messages\n",
			pmsg->flags & I2C_M_RD ? "read" : "write",
			pmsg->len, pmsg->addr, i + 1, num);
		if (pmsg->flags & I2C_M_RD)
			ret =
			    mpc_read(i2c, pmsg->addr, pmsg->buf, pmsg->len, i);
		else
			ret =
			    mpc_write(i2c, pmsg->addr, pmsg->buf, pmsg->len, i);
	}
	mpc_i2c_stop(i2c);
	return (ret < 0) ? ret : num;
}

static u32 mpc_functionality(struct i2c_adapter *adap)
{
	return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
}

static const struct i2c_algorithm mpc_algo = {
	.master_xfer = mpc_xfer,
	.functionality = mpc_functionality,
};

static struct i2c_adapter mpc_ops = {
	.owner = THIS_MODULE,
	.name = "MPC adapter",
	.algo = &mpc_algo,
	.timeout = HZ,
};

static int __devinit fsl_i2c_probe(struct of_device *op,
				   const struct of_device_id *match)
{
	struct mpc_i2c *i2c;
	const u32 *prop;
	u32 clock = 0;
	int result = 0;
	int plen;

	i2c = kzalloc(sizeof(*i2c), GFP_KERNEL);
	if (!i2c)
		return -ENOMEM;

	i2c->dev = &op->dev; /* for debug and error output */

	init_waitqueue_head(&i2c->queue);

	i2c->base = of_iomap(op->node, 0);
	if (!i2c->base) {
		dev_err(i2c->dev, "failed to map controller\n");
		result = -ENOMEM;
		goto fail_map;
	}

	i2c->irq = irq_of_parse_and_map(op->node, 0);
	if (i2c->irq != NO_IRQ) { /* i2c->irq = NO_IRQ implies polling */
		result = request_irq(i2c->irq, mpc_i2c_isr,
				     IRQF_SHARED, "i2c-mpc", i2c);
		if (result < 0) {
			dev_err(i2c->dev, "failed to attach interrupt\n");
			goto fail_request;
		}
	}

	if (!of_get_property(op->node, "fsl,preserve-clocking", NULL)) {
		prop = of_get_property(op->node, "clock-frequency", &plen);
		if (prop && plen == sizeof(u32))
			clock = *prop;

		if (match->data) {
			struct mpc_i2c_match_data *data =
				(struct mpc_i2c_match_data *)match->data;
			data->setclock(op->node, i2c, clock, data->prescaler);
		} else {
			/* Backwards compatibility */
			if (of_get_property(op->node, "dfsrr", NULL))
				mpc_i2c_setclock_8xxx(op->node, i2c,
						      clock, 0);
		}
	}

	dev_set_drvdata(&op->dev, i2c);

	i2c->adap = mpc_ops;
	i2c_set_adapdata(&i2c->adap, i2c);
	i2c->adap.dev.parent = &op->dev;

	result = i2c_add_adapter(&i2c->adap);
	if (result < 0) {
		dev_err(i2c->dev, "failed to add adapter\n");
		goto fail_add;
	}
	of_register_i2c_devices(&i2c->adap, op->node);

	return result;

 fail_add:
	dev_set_drvdata(&op->dev, NULL);
	free_irq(i2c->irq, i2c);
 fail_request:
	irq_dispose_mapping(i2c->irq);
	iounmap(i2c->base);
 fail_map:
	kfree(i2c);
	return result;
};

static int __devexit fsl_i2c_remove(struct of_device *op)
{
	struct mpc_i2c *i2c = dev_get_drvdata(&op->dev);

	i2c_del_adapter(&i2c->adap);
	dev_set_drvdata(&op->dev, NULL);

	if (i2c->irq != NO_IRQ)
		free_irq(i2c->irq, i2c);

	irq_dispose_mapping(i2c->irq);
	iounmap(i2c->base);
	kfree(i2c);
	return 0;
};

static const struct of_device_id mpc_i2c_of_match[] = {
	{.compatible = "mpc5200-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_52xx,
		},
	},
	{.compatible = "fsl,mpc5200b-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_52xx,
		},
	},
	{.compatible = "fsl,mpc5200-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_52xx,
		},
	},
	{.compatible = "fsl,mpc8313-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_8xxx,
		},
	},
	{.compatible = "fsl,mpc8543-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_8xxx,
			.prescaler = 2,
		},
	},
	{.compatible = "fsl,mpc8544-i2c",
	 .data = &(struct mpc_i2c_match_data) {
			.setclock = mpc_i2c_setclock_8xxx,
			.prescaler = 3,
		},
	/* Backward compatibility */
	},
	{.compatible = "fsl-i2c", },
	{},
};

MODULE_DEVICE_TABLE(of, mpc_i2c_of_match);


/* Structure for a device driver */
static struct of_platform_driver mpc_i2c_driver = {
	.match_table	= mpc_i2c_of_match,
	.probe		= fsl_i2c_probe,
	.remove		= __devexit_p(fsl_i2c_remove),
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DRV_NAME,
	},
};

static int __init fsl_i2c_init(void)
{
	int rv;

	rv = of_register_platform_driver(&mpc_i2c_driver);
	if (rv)
		printk(KERN_ERR DRV_NAME
		       " of_register_platform_driver failed (%i)\n", rv);
	return rv;
}

static void __exit fsl_i2c_exit(void)
{
	of_unregister_platform_driver(&mpc_i2c_driver);
}

module_init(fsl_i2c_init);
module_exit(fsl_i2c_exit);

MODULE_AUTHOR("Adrian Cox <adrian@humboldt.co.uk>");
MODULE_DESCRIPTION("I2C-Bus adapter for MPC107 bridge and "
		   "MPC824x/85xx/52xx processors");
MODULE_LICENSE("GPL");
