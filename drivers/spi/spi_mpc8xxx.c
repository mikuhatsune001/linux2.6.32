/*
 * MPC8xxx SPI controller driver.
 *
 * Maintainer: Kumar Gala
 *
 * Copyright (C) 2006 Polycom, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/bug.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <linux/device.h>
#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/of_spi.h>

#include <sysdev/fsl_soc.h>
#include <asm/irq.h>

/* SPI Controller registers */
struct mpc8xxx_spi_reg {
	u8 res1[0x20];
	__be32 mode;
	__be32 event;
	__be32 mask;
	__be32 command;
	__be32 transmit;
	__be32 receive;
};

/* SPI Controller mode register definitions */
#define	SPMODE_LOOP		(1 << 30)
#define	SPMODE_CI_INACTIVEHIGH	(1 << 29)
#define	SPMODE_CP_BEGIN_EDGECLK	(1 << 28)
#define	SPMODE_DIV16		(1 << 27)
#define	SPMODE_REV		(1 << 26)
#define	SPMODE_MS		(1 << 25)
#define	SPMODE_ENABLE		(1 << 24)
#define	SPMODE_LEN(x)		((x) << 20)
#define	SPMODE_PM(x)		((x) << 16)
#define	SPMODE_OP		(1 << 14)
#define	SPMODE_CG(x)		((x) << 7)

/*
 * Default for SPI Mode:
 * 	SPI MODE 0 (inactive low, phase middle, MSB, 8-bit length, slow clk
 */
#define	SPMODE_INIT_VAL (SPMODE_CI_INACTIVEHIGH | SPMODE_DIV16 | SPMODE_REV | \
			 SPMODE_MS | SPMODE_LEN(7) | SPMODE_PM(0xf))

/* SPIE register values */
#define	SPIE_NE		0x00000200	/* Not empty */
#define	SPIE_NF		0x00000100	/* Not full */

/* SPIM register values */
#define	SPIM_NE		0x00000200	/* Not empty */
#define	SPIM_NF		0x00000100	/* Not full */

/* SPI Controller driver's private data. */
struct mpc8xxx_spi {
	struct mpc8xxx_spi_reg __iomem *base;

	/* rx & tx bufs from the spi_transfer */
	const void *tx;
	void *rx;

	/* functions to deal with different sized buffers */
	void (*get_rx) (u32 rx_data, struct mpc8xxx_spi *);
	u32(*get_tx) (struct mpc8xxx_spi *);

	unsigned int count;
	unsigned int irq;

	unsigned nsecs;		/* (clock cycle time)/2 */

	u32 spibrg;		/* SPIBRG input clock */
	u32 rx_shift;		/* RX data reg shift when in qe mode */
	u32 tx_shift;		/* TX data reg shift when in qe mode */

	bool qe_mode;

	struct workqueue_struct *workqueue;
	struct work_struct work;

	struct list_head queue;
	spinlock_t lock;

	struct completion done;
};

struct spi_mpc8xxx_cs {
	/* functions to deal with different sized buffers */
	void (*get_rx) (u32 rx_data, struct mpc8xxx_spi *);
	u32 (*get_tx) (struct mpc8xxx_spi *);
	u32 rx_shift;		/* RX data reg shift when in qe mode */
	u32 tx_shift;		/* TX data reg shift when in qe mode */
	u32 hw_mode;		/* Holds HW mode register settings */
};

static inline void mpc8xxx_spi_write_reg(__be32 __iomem *reg, u32 val)
{
	out_be32(reg, val);
}

static inline u32 mpc8xxx_spi_read_reg(__be32 __iomem *reg)
{
	return in_be32(reg);
}

#define MPC83XX_SPI_RX_BUF(type) 					  \
static									  \
void mpc8xxx_spi_rx_buf_##type(u32 data, struct mpc8xxx_spi *mpc8xxx_spi) \
{									  \
	type *rx = mpc8xxx_spi->rx;					  \
	*rx++ = (type)(data >> mpc8xxx_spi->rx_shift);			  \
	mpc8xxx_spi->rx = rx;						  \
}

#define MPC83XX_SPI_TX_BUF(type)				\
static								\
u32 mpc8xxx_spi_tx_buf_##type(struct mpc8xxx_spi *mpc8xxx_spi)	\
{								\
	u32 data;						\
	const type *tx = mpc8xxx_spi->tx;			\
	if (!tx)						\
		return 0;					\
	data = *tx++ << mpc8xxx_spi->tx_shift;			\
	mpc8xxx_spi->tx = tx;					\
	return data;						\
}

MPC83XX_SPI_RX_BUF(u8)
MPC83XX_SPI_RX_BUF(u16)
MPC83XX_SPI_RX_BUF(u32)
MPC83XX_SPI_TX_BUF(u8)
MPC83XX_SPI_TX_BUF(u16)
MPC83XX_SPI_TX_BUF(u32)

static void mpc8xxx_spi_chipselect(struct spi_device *spi, int value)
{
	struct mpc8xxx_spi *mpc8xxx_spi = spi_master_get_devdata(spi->master);
	struct fsl_spi_platform_data *pdata = spi->dev.parent->platform_data;
	bool pol = spi->mode & SPI_CS_HIGH;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;

	if (value == BITBANG_CS_INACTIVE) {
		if (pdata->cs_control)
			pdata->cs_control(spi, !pol);
	}

	if (value == BITBANG_CS_ACTIVE) {
		u32 regval = mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->mode);

		mpc8xxx_spi->rx_shift = cs->rx_shift;
		mpc8xxx_spi->tx_shift = cs->tx_shift;
		mpc8xxx_spi->get_rx = cs->get_rx;
		mpc8xxx_spi->get_tx = cs->get_tx;

		if (cs->hw_mode != regval) {
			unsigned long flags;
			__be32 __iomem *mode = &mpc8xxx_spi->base->mode;

			regval = cs->hw_mode;
			/* Turn off IRQs locally to minimize time that
			 * SPI is disabled
			 */
			local_irq_save(flags);
			/* Turn off SPI unit prior changing mode */
			mpc8xxx_spi_write_reg(mode, regval & ~SPMODE_ENABLE);
			mpc8xxx_spi_write_reg(mode, regval);
			local_irq_restore(flags);
		}
		if (pdata->cs_control)
			pdata->cs_control(spi, pol);
	}
}

static
int mpc8xxx_spi_setup_transfer(struct spi_device *spi, struct spi_transfer *t)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
	u32 regval;
	u8 bits_per_word, pm;
	u32 hz;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;

	mpc8xxx_spi = spi_master_get_devdata(spi->master);

	if (t) {
		bits_per_word = t->bits_per_word;
		hz = t->speed_hz;
	} else {
		bits_per_word = 0;
		hz = 0;
	}

	/* spi_transfer level calls that work per-word */
	if (!bits_per_word)
		bits_per_word = spi->bits_per_word;

	/* Make sure its a bit width we support [4..16, 32] */
	if ((bits_per_word < 4)
	    || ((bits_per_word > 16) && (bits_per_word != 32)))
		return -EINVAL;

	if (!hz)
		hz = spi->max_speed_hz;

	cs->rx_shift = 0;
	cs->tx_shift = 0;
	if (bits_per_word <= 8) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u8;
		cs->get_tx = mpc8xxx_spi_tx_buf_u8;
		if (mpc8xxx_spi->qe_mode) {
			cs->rx_shift = 16;
			cs->tx_shift = 24;
		}
	} else if (bits_per_word <= 16) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u16;
		cs->get_tx = mpc8xxx_spi_tx_buf_u16;
		if (mpc8xxx_spi->qe_mode) {
			cs->rx_shift = 16;
			cs->tx_shift = 16;
		}
	} else if (bits_per_word <= 32) {
		cs->get_rx = mpc8xxx_spi_rx_buf_u32;
		cs->get_tx = mpc8xxx_spi_tx_buf_u32;
	} else
		return -EINVAL;

	if (mpc8xxx_spi->qe_mode && spi->mode & SPI_LSB_FIRST) {
		cs->tx_shift = 0;
		if (bits_per_word <= 8)
			cs->rx_shift = 8;
		else
			cs->rx_shift = 0;
	}

	mpc8xxx_spi->rx_shift = cs->rx_shift;
	mpc8xxx_spi->tx_shift = cs->tx_shift;
	mpc8xxx_spi->get_rx = cs->get_rx;
	mpc8xxx_spi->get_tx = cs->get_tx;

	if (bits_per_word == 32)
		bits_per_word = 0;
	else
		bits_per_word = bits_per_word - 1;

	/* mask out bits we are going to set */
	cs->hw_mode &= ~(SPMODE_LEN(0xF) | SPMODE_DIV16
				  | SPMODE_PM(0xF));

	cs->hw_mode |= SPMODE_LEN(bits_per_word);

	if ((mpc8xxx_spi->spibrg / hz) > 64) {
		cs->hw_mode |= SPMODE_DIV16;
		pm = mpc8xxx_spi->spibrg / (hz * 64);

		WARN_ONCE(pm > 16, "%s: Requested speed is too low: %d Hz. "
			  "Will use %d Hz instead.\n", dev_name(&spi->dev),
			  hz, mpc8xxx_spi->spibrg / 1024);
		if (pm > 16)
			pm = 16;
	} else
		pm = mpc8xxx_spi->spibrg / (hz * 4);
	if (pm)
		pm--;

	cs->hw_mode |= SPMODE_PM(pm);
	regval =  mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->mode);
	if (cs->hw_mode != regval) {
		unsigned long flags;
		__be32 __iomem *mode = &mpc8xxx_spi->base->mode;

		regval = cs->hw_mode;
		/* Turn off IRQs locally to minimize time
		 * that SPI is disabled
		 */
		local_irq_save(flags);
		/* Turn off SPI unit prior changing mode */
		mpc8xxx_spi_write_reg(mode, regval & ~SPMODE_ENABLE);
		mpc8xxx_spi_write_reg(mode, regval);
		local_irq_restore(flags);
	}
	return 0;
}

static int mpc8xxx_spi_bufs(struct spi_device *spi, struct spi_transfer *t)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
	u32 word, len, bits_per_word;

	mpc8xxx_spi = spi_master_get_devdata(spi->master);

	mpc8xxx_spi->tx = t->tx_buf;
	mpc8xxx_spi->rx = t->rx_buf;
	bits_per_word = spi->bits_per_word;
	if (t->bits_per_word)
		bits_per_word = t->bits_per_word;
	len = t->len;
	if (bits_per_word > 8) {
		/* invalid length? */
		if (len & 1)
			return -EINVAL;
		len /= 2;
	}
	if (bits_per_word > 16) {
		/* invalid length? */
		if (len & 1)
			return -EINVAL;
		len /= 2;
	}
	mpc8xxx_spi->count = len;

	INIT_COMPLETION(mpc8xxx_spi->done);

	/* enable rx ints */
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->mask, SPIM_NE);

	/* transmit word */
	word = mpc8xxx_spi->get_tx(mpc8xxx_spi);
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->transmit, word);

	wait_for_completion(&mpc8xxx_spi->done);

	/* disable rx ints */
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->mask, 0);

	return mpc8xxx_spi->count;
}

static void mpc8xxx_spi_do_one_msg(struct spi_message *m)
{
	struct spi_device *spi = m->spi;
	struct spi_transfer *t;
	unsigned int cs_change;
	const int nsecs = 50;
	int status;

	cs_change = 1;
	status = 0;
	list_for_each_entry(t, &m->transfers, transfer_list) {
		if (t->bits_per_word || t->speed_hz) {
			/* Don't allow changes if CS is active */
			status = -EINVAL;

			if (cs_change)
				status = mpc8xxx_spi_setup_transfer(spi, t);
			if (status < 0)
				break;
		}

		if (cs_change) {
			mpc8xxx_spi_chipselect(spi, BITBANG_CS_ACTIVE);
			ndelay(nsecs);
		}
		cs_change = t->cs_change;
		if (t->len)
			status = mpc8xxx_spi_bufs(spi, t);
		if (status) {
			status = -EMSGSIZE;
			break;
		}
		m->actual_length += t->len;

		if (t->delay_usecs)
			udelay(t->delay_usecs);

		if (cs_change) {
			ndelay(nsecs);
			mpc8xxx_spi_chipselect(spi, BITBANG_CS_INACTIVE);
			ndelay(nsecs);
		}
	}

	m->status = status;
	m->complete(m->context);

	if (status || !cs_change) {
		ndelay(nsecs);
		mpc8xxx_spi_chipselect(spi, BITBANG_CS_INACTIVE);
	}

	mpc8xxx_spi_setup_transfer(spi, NULL);
}

static void mpc8xxx_spi_work(struct work_struct *work)
{
	struct mpc8xxx_spi *mpc8xxx_spi = container_of(work, struct mpc8xxx_spi,
						       work);

	spin_lock_irq(&mpc8xxx_spi->lock);
	while (!list_empty(&mpc8xxx_spi->queue)) {
		struct spi_message *m = container_of(mpc8xxx_spi->queue.next,
						   struct spi_message, queue);

		list_del_init(&m->queue);
		spin_unlock_irq(&mpc8xxx_spi->lock);

		mpc8xxx_spi_do_one_msg(m);

		spin_lock_irq(&mpc8xxx_spi->lock);
	}
	spin_unlock_irq(&mpc8xxx_spi->lock);
}

static int mpc8xxx_spi_setup(struct spi_device *spi)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
	int retval;
	u32 hw_mode;
	struct spi_mpc8xxx_cs	*cs = spi->controller_state;

	if (!spi->max_speed_hz)
		return -EINVAL;

	if (!cs) {
		cs = kzalloc(sizeof *cs, GFP_KERNEL);
		if (!cs)
			return -ENOMEM;
		spi->controller_state = cs;
	}
	mpc8xxx_spi = spi_master_get_devdata(spi->master);

	hw_mode = cs->hw_mode; /* Save orginal settings */
	cs->hw_mode = mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->mode);
	/* mask out bits we are going to set */
	cs->hw_mode &= ~(SPMODE_CP_BEGIN_EDGECLK | SPMODE_CI_INACTIVEHIGH
			 | SPMODE_REV | SPMODE_LOOP);

	if (spi->mode & SPI_CPHA)
		cs->hw_mode |= SPMODE_CP_BEGIN_EDGECLK;
	if (spi->mode & SPI_CPOL)
		cs->hw_mode |= SPMODE_CI_INACTIVEHIGH;
	if (!(spi->mode & SPI_LSB_FIRST))
		cs->hw_mode |= SPMODE_REV;
	if (spi->mode & SPI_LOOP)
		cs->hw_mode |= SPMODE_LOOP;

	retval = mpc8xxx_spi_setup_transfer(spi, NULL);
	if (retval < 0) {
		cs->hw_mode = hw_mode; /* Restore settings */
		return retval;
	}
	return 0;
}

static irqreturn_t mpc8xxx_spi_irq(s32 irq, void *context_data)
{
	struct mpc8xxx_spi *mpc8xxx_spi = context_data;
	u32 event;
	irqreturn_t ret = IRQ_NONE;

	/* Get interrupt events(tx/rx) */
	event = mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->event);

	/* We need handle RX first */
	if (event & SPIE_NE) {
		u32 rx_data = mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->receive);

		if (mpc8xxx_spi->rx)
			mpc8xxx_spi->get_rx(rx_data, mpc8xxx_spi);

		ret = IRQ_HANDLED;
	}

	if ((event & SPIE_NF) == 0)
		/* spin until TX is done */
		while (((event =
			 mpc8xxx_spi_read_reg(&mpc8xxx_spi->base->event)) &
						SPIE_NF) == 0)
			cpu_relax();

	mpc8xxx_spi->count -= 1;
	if (mpc8xxx_spi->count) {
		u32 word = mpc8xxx_spi->get_tx(mpc8xxx_spi);
		mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->transmit, word);
	} else {
		complete(&mpc8xxx_spi->done);
	}

	/* Clear the events */
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->event, event);

	return ret;
}
static int mpc8xxx_spi_transfer(struct spi_device *spi,
				struct spi_message *m)
{
	struct mpc8xxx_spi *mpc8xxx_spi = spi_master_get_devdata(spi->master);
	unsigned long flags;

	m->actual_length = 0;
	m->status = -EINPROGRESS;

	spin_lock_irqsave(&mpc8xxx_spi->lock, flags);
	list_add_tail(&m->queue, &mpc8xxx_spi->queue);
	queue_work(mpc8xxx_spi->workqueue, &mpc8xxx_spi->work);
	spin_unlock_irqrestore(&mpc8xxx_spi->lock, flags);

	return 0;
}


static void mpc8xxx_spi_cleanup(struct spi_device *spi)
{
	kfree(spi->controller_state);
}

static struct spi_master * __devinit
mpc8xxx_spi_probe(struct device *dev, struct resource *mem, unsigned int irq)
{
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct spi_master *master;
	struct mpc8xxx_spi *mpc8xxx_spi;
	u32 regval;
	int ret = 0;

	master = spi_alloc_master(dev, sizeof(struct mpc8xxx_spi));
	if (master == NULL) {
		ret = -ENOMEM;
		goto err;
	}

	dev_set_drvdata(dev, master);

	/* the spi->mode bits understood by this driver: */
	master->mode_bits = SPI_CPOL | SPI_CPHA | SPI_CS_HIGH
			| SPI_LSB_FIRST | SPI_LOOP;

	master->setup = mpc8xxx_spi_setup;
	master->transfer = mpc8xxx_spi_transfer;
	master->cleanup = mpc8xxx_spi_cleanup;

	mpc8xxx_spi = spi_master_get_devdata(master);
	mpc8xxx_spi->qe_mode = pdata->qe_mode;
	mpc8xxx_spi->get_rx = mpc8xxx_spi_rx_buf_u8;
	mpc8xxx_spi->get_tx = mpc8xxx_spi_tx_buf_u8;
	mpc8xxx_spi->spibrg = pdata->sysclk;

	mpc8xxx_spi->rx_shift = 0;
	mpc8xxx_spi->tx_shift = 0;
	if (mpc8xxx_spi->qe_mode) {
		mpc8xxx_spi->rx_shift = 16;
		mpc8xxx_spi->tx_shift = 24;
	}

	init_completion(&mpc8xxx_spi->done);

	mpc8xxx_spi->base = ioremap(mem->start, mem->end - mem->start + 1);
	if (mpc8xxx_spi->base == NULL) {
		ret = -ENOMEM;
		goto put_master;
	}

	mpc8xxx_spi->irq = irq;

	/* Register for SPI Interrupt */
	ret = request_irq(mpc8xxx_spi->irq, mpc8xxx_spi_irq,
			  0, "mpc8xxx_spi", mpc8xxx_spi);

	if (ret != 0)
		goto unmap_io;

	master->bus_num = pdata->bus_num;
	master->num_chipselect = pdata->max_chipselect;

	/* SPI controller initializations */
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->mode, 0);
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->mask, 0);
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->command, 0);
	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->event, 0xffffffff);

	/* Enable SPI interface */
	regval = pdata->initial_spmode | SPMODE_INIT_VAL | SPMODE_ENABLE;
	if (pdata->qe_mode)
		regval |= SPMODE_OP;

	mpc8xxx_spi_write_reg(&mpc8xxx_spi->base->mode, regval);
	spin_lock_init(&mpc8xxx_spi->lock);
	init_completion(&mpc8xxx_spi->done);
	INIT_WORK(&mpc8xxx_spi->work, mpc8xxx_spi_work);
	INIT_LIST_HEAD(&mpc8xxx_spi->queue);

	mpc8xxx_spi->workqueue = create_singlethread_workqueue(
		dev_name(master->dev.parent));
	if (mpc8xxx_spi->workqueue == NULL) {
		ret = -EBUSY;
		goto free_irq;
	}

	ret = spi_register_master(master);
	if (ret < 0)
		goto unreg_master;

	printk(KERN_INFO
	       "%s: MPC8xxx SPI Controller driver at 0x%p (irq = %d)\n",
	       dev_name(dev), mpc8xxx_spi->base, mpc8xxx_spi->irq);

	return master;

unreg_master:
	destroy_workqueue(mpc8xxx_spi->workqueue);
free_irq:
	free_irq(mpc8xxx_spi->irq, mpc8xxx_spi);
unmap_io:
	iounmap(mpc8xxx_spi->base);
put_master:
	spi_master_put(master);
err:
	return ERR_PTR(ret);
}

static int __devexit mpc8xxx_spi_remove(struct device *dev)
{
	struct mpc8xxx_spi *mpc8xxx_spi;
	struct spi_master *master;

	master = dev_get_drvdata(dev);
	mpc8xxx_spi = spi_master_get_devdata(master);

	flush_workqueue(mpc8xxx_spi->workqueue);
	destroy_workqueue(mpc8xxx_spi->workqueue);
	spi_unregister_master(master);

	free_irq(mpc8xxx_spi->irq, mpc8xxx_spi);
	iounmap(mpc8xxx_spi->base);

	return 0;
}

struct mpc8xxx_spi_probe_info {
	struct fsl_spi_platform_data pdata;
	int *gpios;
	bool *alow_flags;
};

static struct mpc8xxx_spi_probe_info *
to_of_pinfo(struct fsl_spi_platform_data *pdata)
{
	return container_of(pdata, struct mpc8xxx_spi_probe_info, pdata);
}

static void mpc8xxx_spi_cs_control(struct spi_device *spi, bool on)
{
	struct device *dev = spi->dev.parent;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(dev->platform_data);
	u16 cs = spi->chip_select;
	int gpio = pinfo->gpios[cs];
	bool alow = pinfo->alow_flags[cs];

	gpio_set_value(gpio, on ^ alow);
}

static int of_mpc8xxx_spi_get_chipselects(struct device *dev)
{
	struct device_node *np = dev_archdata_get_node(&dev->archdata);
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(pdata);
	unsigned int ngpios;
	int i = 0;
	int ret;

	ngpios = of_gpio_count(np);
	if (!ngpios) {
		/*
		 * SPI w/o chip-select line. One SPI device is still permitted
		 * though.
		 */
		pdata->max_chipselect = 1;
		return 0;
	}

	pinfo->gpios = kmalloc(ngpios * sizeof(*pinfo->gpios), GFP_KERNEL);
	if (!pinfo->gpios)
		return -ENOMEM;
	memset(pinfo->gpios, -1, ngpios * sizeof(*pinfo->gpios));

	pinfo->alow_flags = kzalloc(ngpios * sizeof(*pinfo->alow_flags),
				    GFP_KERNEL);
	if (!pinfo->alow_flags) {
		ret = -ENOMEM;
		goto err_alloc_flags;
	}

	for (; i < ngpios; i++) {
		int gpio;
		enum of_gpio_flags flags;

		gpio = of_get_gpio_flags(np, i, &flags);
		if (!gpio_is_valid(gpio)) {
			dev_err(dev, "invalid gpio #%d: %d\n", i, gpio);
			goto err_loop;
		}

		ret = gpio_request(gpio, dev_name(dev));
		if (ret) {
			dev_err(dev, "can't request gpio #%d: %d\n", i, ret);
			goto err_loop;
		}

		pinfo->gpios[i] = gpio;
		pinfo->alow_flags[i] = flags & OF_GPIO_ACTIVE_LOW;

		ret = gpio_direction_output(pinfo->gpios[i],
					    pinfo->alow_flags[i]);
		if (ret) {
			dev_err(dev, "can't set output direction for gpio "
				"#%d: %d\n", i, ret);
			goto err_loop;
		}
	}

	pdata->max_chipselect = ngpios;
	pdata->cs_control = mpc8xxx_spi_cs_control;

	return 0;

err_loop:
	while (i >= 0) {
		if (gpio_is_valid(pinfo->gpios[i]))
			gpio_free(pinfo->gpios[i]);
		i--;
	}

	kfree(pinfo->alow_flags);
	pinfo->alow_flags = NULL;
err_alloc_flags:
	kfree(pinfo->gpios);
	pinfo->gpios = NULL;
	return ret;
}

static int of_mpc8xxx_spi_free_chipselects(struct device *dev)
{
	struct fsl_spi_platform_data *pdata = dev->platform_data;
	struct mpc8xxx_spi_probe_info *pinfo = to_of_pinfo(pdata);
	int i;

	if (!pinfo->gpios)
		return 0;

	for (i = 0; i < pdata->max_chipselect; i++) {
		if (gpio_is_valid(pinfo->gpios[i]))
			gpio_free(pinfo->gpios[i]);
	}

	kfree(pinfo->gpios);
	kfree(pinfo->alow_flags);
	return 0;
}

static int __devinit of_mpc8xxx_spi_probe(struct of_device *ofdev,
					  const struct of_device_id *ofid)
{
	struct device *dev = &ofdev->dev;
	struct device_node *np = ofdev->node;
	struct mpc8xxx_spi_probe_info *pinfo;
	struct fsl_spi_platform_data *pdata;
	struct spi_master *master;
	struct resource mem;
	struct resource irq;
	const void *prop;
	int ret = -ENOMEM;

	pinfo = kzalloc(sizeof(*pinfo), GFP_KERNEL);
	if (!pinfo)
		return -ENOMEM;

	pdata = &pinfo->pdata;
	dev->platform_data = pdata;

	/* Allocate bus num dynamically. */
	pdata->bus_num = -1;

	/* SPI controller is either clocked from QE or SoC clock. */
	pdata->sysclk = get_brgfreq();
	if (pdata->sysclk == -1) {
		pdata->sysclk = fsl_get_sys_freq();
		if (pdata->sysclk == -1) {
			ret = -ENODEV;
			goto err_clk;
		}
	}

	prop = of_get_property(np, "mode", NULL);
	if (prop && !strcmp(prop, "cpu-qe"))
		pdata->qe_mode = 1;

	ret = of_mpc8xxx_spi_get_chipselects(dev);
	if (ret)
		goto err;

	ret = of_address_to_resource(np, 0, &mem);
	if (ret)
		goto err;

	ret = of_irq_to_resource(np, 0, &irq);
	if (!ret) {
		ret = -EINVAL;
		goto err;
	}

	master = mpc8xxx_spi_probe(dev, &mem, irq.start);
	if (IS_ERR(master)) {
		ret = PTR_ERR(master);
		goto err;
	}

	of_register_spi_devices(master, np);

	return 0;

err:
	of_mpc8xxx_spi_free_chipselects(dev);
err_clk:
	kfree(pinfo);
	return ret;
}

static int __devexit of_mpc8xxx_spi_remove(struct of_device *ofdev)
{
	int ret;

	ret = mpc8xxx_spi_remove(&ofdev->dev);
	if (ret)
		return ret;
	of_mpc8xxx_spi_free_chipselects(&ofdev->dev);
	return 0;
}

static const struct of_device_id of_mpc8xxx_spi_match[] = {
	{ .compatible = "fsl,spi" },
	{},
};
MODULE_DEVICE_TABLE(of, of_mpc8xxx_spi_match);

static struct of_platform_driver of_mpc8xxx_spi_driver = {
	.name		= "mpc8xxx_spi",
	.match_table	= of_mpc8xxx_spi_match,
	.probe		= of_mpc8xxx_spi_probe,
	.remove		= __devexit_p(of_mpc8xxx_spi_remove),
};

#ifdef CONFIG_MPC832x_RDB
/*
 * 				XXX XXX XXX
 * This is "legacy" platform driver, was used by the MPC8323E-RDB boards
 * only. The driver should go away soon, since newer MPC8323E-RDB's device
 * tree can work with OpenFirmware driver. But for now we support old trees
 * as well.
 */
static int __devinit plat_mpc8xxx_spi_probe(struct platform_device *pdev)
{
	struct resource *mem;
	unsigned int irq;
	struct spi_master *master;

	if (!pdev->dev.platform_data)
		return -EINVAL;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!mem)
		return -EINVAL;

	irq = platform_get_irq(pdev, 0);
	if (!irq)
		return -EINVAL;

	master = mpc8xxx_spi_probe(&pdev->dev, mem, irq);
	if (IS_ERR(master))
		return PTR_ERR(master);
	return 0;
}

static int __devexit plat_mpc8xxx_spi_remove(struct platform_device *pdev)
{
	return mpc8xxx_spi_remove(&pdev->dev);
}

MODULE_ALIAS("platform:mpc8xxx_spi");
static struct platform_driver mpc8xxx_spi_driver = {
	.probe = plat_mpc8xxx_spi_probe,
	.remove = __exit_p(plat_mpc8xxx_spi_remove),
	.driver = {
		.name = "mpc8xxx_spi",
		.owner = THIS_MODULE,
	},
};

static bool legacy_driver_failed;

static void __init legacy_driver_register(void)
{
	legacy_driver_failed = platform_driver_register(&mpc8xxx_spi_driver);
}

static void __exit legacy_driver_unregister(void)
{
	if (legacy_driver_failed)
		return;
	platform_driver_unregister(&mpc8xxx_spi_driver);
}
#else
static void __init legacy_driver_register(void) {}
static void __exit legacy_driver_unregister(void) {}
#endif /* CONFIG_MPC832x_RDB */

static int __init mpc8xxx_spi_init(void)
{
	legacy_driver_register();
	return of_register_platform_driver(&of_mpc8xxx_spi_driver);
}

static void __exit mpc8xxx_spi_exit(void)
{
	of_unregister_platform_driver(&of_mpc8xxx_spi_driver);
	legacy_driver_unregister();
}

module_init(mpc8xxx_spi_init);
module_exit(mpc8xxx_spi_exit);

MODULE_AUTHOR("Kumar Gala");
MODULE_DESCRIPTION("Simple MPC8xxx SPI Driver");
MODULE_LICENSE("GPL");
