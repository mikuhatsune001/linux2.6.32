/*
 * Copyright 2007 Robert Schwebel <r.schwebel@pengutronix.de>, Pengutronix
 * Copyright (C) 2009 Sascha Hauer (kernel@pengutronix.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <linux/i2c/at24.h>
#include <linux/dma-mapping.h>
#include <linux/spi/spi.h>
#include <linux/spi/eeprom.h>
#include <linux/irq.h>
#include <linux/gpio.h>

#include <asm/mach/arch.h>
#include <asm/mach-types.h>
#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/iomux.h>
#include <mach/i2c.h>
#include <asm/mach/time.h>
#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
#include <mach/spi.h>
#endif
#include <mach/imx-uart.h>
#include <mach/mxc_nand.h>
#include <mach/irqs.h>
#include <mach/mmc.h>

#include "devices.h"

static int pca100_pins[] = {
	/* UART1 */
	PE12_PF_UART1_TXD,
	PE13_PF_UART1_RXD,
	PE14_PF_UART1_CTS,
	PE15_PF_UART1_RTS,
	/* SDHC */
	PB4_PF_SD2_D0,
	PB5_PF_SD2_D1,
	PB6_PF_SD2_D2,
	PB7_PF_SD2_D3,
	PB8_PF_SD2_CMD,
	PB9_PF_SD2_CLK,
	/* FEC */
	PD0_AIN_FEC_TXD0,
	PD1_AIN_FEC_TXD1,
	PD2_AIN_FEC_TXD2,
	PD3_AIN_FEC_TXD3,
	PD4_AOUT_FEC_RX_ER,
	PD5_AOUT_FEC_RXD1,
	PD6_AOUT_FEC_RXD2,
	PD7_AOUT_FEC_RXD3,
	PD8_AF_FEC_MDIO,
	PD9_AIN_FEC_MDC,
	PD10_AOUT_FEC_CRS,
	PD11_AOUT_FEC_TX_CLK,
	PD12_AOUT_FEC_RXD0,
	PD13_AOUT_FEC_RX_DV,
	PD14_AOUT_FEC_RX_CLK,
	PD15_AOUT_FEC_COL,
	PD16_AIN_FEC_TX_ER,
	PF23_AIN_FEC_TX_EN,
	/* SSI1 */
	PC20_PF_SSI1_FS,
	PC21_PF_SSI1_RXD,
	PC22_PF_SSI1_TXD,
	PC23_PF_SSI1_CLK,
	/* onboard I2C */
	PC5_PF_I2C2_SDA,
	PC6_PF_I2C2_SCL,
	/* external I2C */
	PD17_PF_I2C_DATA,
	PD18_PF_I2C_CLK,
	/* SPI1 */
	PD25_PF_CSPI1_RDY,
	PD29_PF_CSPI1_SCLK,
	PD30_PF_CSPI1_MISO,
	PD31_PF_CSPI1_MOSI,
};

static struct imxuart_platform_data uart_pdata = {
	.flags = IMXUART_HAVE_RTSCTS,
};

static struct mxc_nand_platform_data pca100_nand_board_info = {
	.width = 1,
	.hw_ecc = 1,
};

static struct platform_device *platform_devices[] __initdata = {
	&mxc_w1_master_device,
	&mxc_fec_device,
};

static struct imxi2c_platform_data pca100_i2c_1_data = {
	.bitrate = 100000,
};

static struct at24_platform_data board_eeprom = {
	.byte_len = 4096,
	.page_size = 32,
	.flags = AT24_FLAG_ADDR16,
};

static struct i2c_board_info pca100_i2c_devices[] = {
	{
		I2C_BOARD_INFO("at24", 0x52), /* E0=0, E1=1, E2=0 */
		.platform_data = &board_eeprom,
	}, {
		I2C_BOARD_INFO("rtc-pcf8563", 0x51),
		.type = "pcf8563"
	}, {
		I2C_BOARD_INFO("lm75", 0x4a),
		.type = "lm75"
	}
};

#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
static struct spi_eeprom at25320 = {
	.name		= "at25320an",
	.byte_len	= 4096,
	.page_size	= 32,
	.flags		= EE_ADDR2,
};

static struct spi_board_info pca100_spi_board_info[] __initdata = {
	{
		.modalias = "at25",
		.max_speed_hz = 30000,
		.bus_num = 0,
		.chip_select = 1,
		.platform_data = &at25320,
	},
};

static int pca100_spi_cs[] = {GPIO_PORTD + 28, GPIO_PORTD + 27};

static struct spi_imx_master pca100_spi_0_data = {
	.chipselect	= pca100_spi_cs,
	.num_chipselect = ARRAY_SIZE(pca100_spi_cs),
};
#endif

static int pca100_sdhc2_init(struct device *dev, irq_handler_t detect_irq,
		void *data)
{
	int ret;

	ret = request_irq(IRQ_GPIOC(29), detect_irq,
			  IRQF_DISABLED | IRQF_TRIGGER_FALLING,
			  "imx-mmc-detect", data);
	if (ret)
		printk(KERN_ERR
			"pca100: Failed to reuest irq for sd/mmc detection\n");

	return ret;
}

static void pca100_sdhc2_exit(struct device *dev, void *data)
{
	free_irq(IRQ_GPIOC(29), data);
}

static struct imxmmc_platform_data sdhc_pdata = {
	.init = pca100_sdhc2_init,
	.exit = pca100_sdhc2_exit,
};

static void __init pca100_init(void)
{
	int ret;

	ret = mxc_gpio_setup_multiple_pins(pca100_pins,
			ARRAY_SIZE(pca100_pins), "PCA100");
	if (ret)
		printk(KERN_ERR "pca100: Failed to setup pins (%d)\n", ret);

	mxc_register_device(&mxc_uart_device0, &uart_pdata);

	mxc_gpio_mode(GPIO_PORTC | 29 | GPIO_GPIO | GPIO_IN);
	mxc_register_device(&mxc_sdhc_device1, &sdhc_pdata);

	mxc_register_device(&mxc_nand_device, &pca100_nand_board_info);

	/* only the i2c master 1 is used on this CPU card */
	i2c_register_board_info(1, pca100_i2c_devices,
				ARRAY_SIZE(pca100_i2c_devices));

	mxc_register_device(&mxc_i2c_device1, &pca100_i2c_1_data);

	mxc_gpio_mode(GPIO_PORTD | 28 | GPIO_GPIO | GPIO_OUT);
	mxc_gpio_mode(GPIO_PORTD | 27 | GPIO_GPIO | GPIO_OUT);

	/* GPIO0_IRQ */
	mxc_gpio_mode(GPIO_PORTC | 31 | GPIO_GPIO | GPIO_IN);
	/* GPIO1_IRQ */
	mxc_gpio_mode(GPIO_PORTC | 25 | GPIO_GPIO | GPIO_IN);
	/* GPIO2_IRQ */
	mxc_gpio_mode(GPIO_PORTE | 5 | GPIO_GPIO | GPIO_IN);

#if defined(CONFIG_SPI_IMX) || defined(CONFIG_SPI_IMX_MODULE)
	spi_register_board_info(pca100_spi_board_info,
				ARRAY_SIZE(pca100_spi_board_info));
	mxc_register_device(&mxc_spi_device0, &pca100_spi_0_data);
#endif

	platform_add_devices(platform_devices, ARRAY_SIZE(platform_devices));
}

static void __init pca100_timer_init(void)
{
	mx27_clocks_init(26000000);
}

static struct sys_timer pca100_timer = {
	.init = pca100_timer_init,
};

MACHINE_START(PCA100, "phyCARD-i.MX27")
	.phys_io        = AIPI_BASE_ADDR,
	.io_pg_offst    = ((AIPI_BASE_ADDR_VIRT) >> 18) & 0xfffc,
	.boot_params    = PHYS_OFFSET + 0x100,
	.map_io         = mx27_map_io,
	.init_irq       = mxc_init_irq,
	.init_machine   = pca100_init,
	.timer          = &pca100_timer,
MACHINE_END

