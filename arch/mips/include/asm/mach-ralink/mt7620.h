/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Parts of this file are based on Ralink's 2.6.21 BSP
 *
 * Copyright (C) 2008-2011 Gabor Juhos <juhosg@openwrt.org>
 * Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 * Copyright (C) 2013 John Crispin <blogic@openwrt.org>
 */

#ifndef _MT7620_REGS_H_
#define _MT7620_REGS_H_

#define MT7620_SYSC_BASE		0x10000000

#define SYSC_REG_CHIP_NAME0		0x00
#define SYSC_REG_CHIP_NAME1		0x04
#define SYSC_REG_CHIP_REV		0x0c
#define SYSC_REG_SYSTEM_CONFIG0		0x10
#define SYSC_REG_SYSTEM_CONFIG1		0x14
#define SYSC_REG_CLKCFG0		0x2c
#define SYSC_REG_CPU_SYS_CLKCFG		0x3c
#define SYSC_REG_CPLL_CONFIG0		0x54
#define SYSC_REG_CPLL_CONFIG1		0x58

#define MT7620N_CHIP_NAME0		0x33365452
#define MT7620N_CHIP_NAME1		0x20203235

#define MT7620A_CHIP_NAME0		0x3637544d
#define MT7620A_CHIP_NAME1		0x20203032

#define SYSCFG0_XTAL_FREQ_SEL		BIT(6)

#define CHIP_REV_PKG_MASK		0x1
#define CHIP_REV_PKG_SHIFT		16
#define CHIP_REV_VER_MASK		0xf
#define CHIP_REV_VER_SHIFT		8
#define CHIP_REV_ECO_MASK		0xf

#define CLKCFG0_PERI_CLK_SEL		BIT(4)

#define CPU_SYS_CLKCFG_OCP_RATIO_SHIFT	16
#define CPU_SYS_CLKCFG_OCP_RATIO_MASK	0xf
#define CPU_SYS_CLKCFG_OCP_RATIO_1	0	/* 1:1   (Reserved) */
#define CPU_SYS_CLKCFG_OCP_RATIO_1_5	1	/* 1:1.5 (Reserved) */
#define CPU_SYS_CLKCFG_OCP_RATIO_2	2	/* 1:2   */
#define CPU_SYS_CLKCFG_OCP_RATIO_2_5	3       /* 1:2.5 (Reserved) */
#define CPU_SYS_CLKCFG_OCP_RATIO_3	4	/* 1:3   */
#define CPU_SYS_CLKCFG_OCP_RATIO_3_5	5	/* 1:3.5 (Reserved) */
#define CPU_SYS_CLKCFG_OCP_RATIO_4	6	/* 1:4   */
#define CPU_SYS_CLKCFG_OCP_RATIO_5	7	/* 1:5   */
#define CPU_SYS_CLKCFG_OCP_RATIO_10	8	/* 1:10  */
#define CPU_SYS_CLKCFG_CPU_FDIV_SHIFT	8
#define CPU_SYS_CLKCFG_CPU_FDIV_MASK	0x1f
#define CPU_SYS_CLKCFG_CPU_FFRAC_SHIFT	0
#define CPU_SYS_CLKCFG_CPU_FFRAC_MASK	0x1f

#define CPLL_CFG0_SW_CFG		BIT(31)
#define CPLL_CFG0_PLL_MULT_RATIO_SHIFT	16
#define CPLL_CFG0_PLL_MULT_RATIO_MASK   0x7
#define CPLL_CFG0_LC_CURFCK		BIT(15)
#define CPLL_CFG0_BYPASS_REF_CLK	BIT(14)
#define CPLL_CFG0_PLL_DIV_RATIO_SHIFT	10
#define CPLL_CFG0_PLL_DIV_RATIO_MASK	0x3

#define CPLL_CFG1_CPU_AUX1		BIT(25)
#define CPLL_CFG1_CPU_AUX0		BIT(24)

#define SYSCFG0_DRAM_TYPE_MASK		0x3
#define SYSCFG0_DRAM_TYPE_SHIFT		4
#define SYSCFG0_DRAM_TYPE_SDRAM		0
#define SYSCFG0_DRAM_TYPE_DDR1		1
#define SYSCFG0_DRAM_TYPE_DDR2		2

#define MT7620_DRAM_BASE		0x0
#define MT7620_SDRAM_SIZE_MIN		2
#define MT7620_SDRAM_SIZE_MAX		64
#define MT7620_DDR1_SIZE_MIN		32
#define MT7620_DDR1_SIZE_MAX		128
#define MT7620_DDR2_SIZE_MIN		32
#define MT7620_DDR2_SIZE_MAX		256

#define MT7620_GPIO_MODE_I2C		BIT(0)
#define MT7620_GPIO_MODE_UART0_SHIFT	2
#define MT7620_GPIO_MODE_UART0_MASK	0x7
#define MT7620_GPIO_MODE_UART0(x)	((x) << MT7620_GPIO_MODE_UART0_SHIFT)
#define MT7620_GPIO_MODE_UARTF		0x0
#define MT7620_GPIO_MODE_PCM_UARTF	0x1
#define MT7620_GPIO_MODE_PCM_I2S	0x2
#define MT7620_GPIO_MODE_I2S_UARTF	0x3
#define MT7620_GPIO_MODE_PCM_GPIO	0x4
#define MT7620_GPIO_MODE_GPIO_UARTF	0x5
#define MT7620_GPIO_MODE_GPIO_I2S	0x6
#define MT7620_GPIO_MODE_GPIO		0x7
#define MT7620_GPIO_MODE_UART1		BIT(5)
#define MT7620_GPIO_MODE_MDIO		BIT(8)
#define MT7620_GPIO_MODE_RGMII1		BIT(9)
#define MT7620_GPIO_MODE_RGMII2		BIT(10)
#define MT7620_GPIO_MODE_SPI		BIT(11)
#define MT7620_GPIO_MODE_SPI_REF_CLK	BIT(12)
#define MT7620_GPIO_MODE_WLED		BIT(13)
#define MT7620_GPIO_MODE_JTAG		BIT(15)
#define MT7620_GPIO_MODE_EPHY		BIT(15)
#define MT7620_GPIO_MODE_WDT		BIT(22)

#endif
