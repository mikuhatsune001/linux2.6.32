/*
 * da9211-regulator.h - Regulator definitions for DA9211
 * Copyright (C) 2014  Dialog Semiconductor Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 */

#ifndef __DA9211_REGISTERS_H__
#define __DA9211_REGISTERS_H__

/* Page selection */
#define	DA9211_REG_PAGE_CON			0x00

/* System Control and Event Registers */
#define	DA9211_REG_STATUS_A			0x50
#define	DA9211_REG_STATUS_B			0x51
#define	DA9211_REG_EVENT_A			0x52
#define	DA9211_REG_EVENT_B			0x53
#define	DA9211_REG_MASK_A			0x54
#define	DA9211_REG_MASK_B			0x55
#define	DA9211_REG_CONTROL_A		0x56

/* GPIO Control Registers */
#define	DA9211_REG_GPIO_0_1			0x58
#define	DA9211_REG_GPIO_2_3			0x59
#define	DA9211_REG_GPIO_4			0x5A

/* Regulator Registers */
#define	DA9211_REG_BUCKA_CONT			0x5D
#define	DA9211_REG_BUCKB_CONT			0x5E
#define	DA9211_REG_BUCK_ILIM			0xD0
#define	DA9211_REG_BUCKA_CONF			0xD1
#define	DA9211_REG_BUCKB_CONF			0xD2
#define	DA9211_REG_BUCK_CONF			0xD3
#define	DA9211_REG_VBACKA_MAX			0xD5
#define	DA9211_REG_VBACKB_MAX			0xD6
#define	DA9211_REG_VBUCKA_A				0xD7
#define	DA9211_REG_VBUCKA_B				0xD8
#define	DA9211_REG_VBUCKB_A				0xD9
#define	DA9211_REG_VBUCKB_B				0xDA

/* I2C Interface Settings */
#define DA9211_REG_INTERFACE			0x105

/* BUCK Phase Selection*/
#define DA9211_REG_CONFIG_E			0x147

/*
 * Registers bits
 */
/* DA9211_REG_PAGE_CON (addr=0x00) */
#define	DA9211_REG_PAGE_SHIFT			1
#define	DA9211_REG_PAGE_MASK			0x02
/* On I2C registers 0x00 - 0xFF */
#define	DA9211_REG_PAGE0			0
/* On I2C registers 0x100 - 0x1FF */
#define	DA9211_REG_PAGE2			2
#define	DA9211_PAGE_WRITE_MODE			0x00
#define	DA9211_REPEAT_WRITE_MODE		0x40
#define	DA9211_PAGE_REVERT			0x80

/* DA9211_REG_STATUS_A (addr=0x50) */
#define	DA9211_GPI0				0x01
#define	DA9211_GPI1				0x02
#define	DA9211_GPI2				0x04
#define	DA9211_GPI3				0x08
#define	DA9211_GPI4				0x10

/* DA9211_REG_EVENT_A (addr=0x52) */
#define	DA9211_E_GPI0				0x01
#define	DA9211_E_GPI1				0x02
#define	DA9211_E_GPI2				0x04
#define	DA9211_E_GPI3				0x08
#define	DA9211_E_GPI4				0x10
#define	DA9211_E_UVLO_IO			0x40

/* DA9211_REG_EVENT_B (addr=0x53) */
#define	DA9211_E_PWRGOOD_A			0x01
#define	DA9211_E_PWRGOOD_B			0x02
#define	DA9211_E_TEMP_WARN			0x04
#define	DA9211_E_TEMP_CRIT			0x08
#define	DA9211_E_OV_CURR_A			0x10
#define	DA9211_E_OV_CURR_B			0x20

/* DA9211_REG_MASK_A (addr=0x54) */
#define	DA9211_M_GPI0				0x01
#define	DA9211_M_GPI1				0x02
#define	DA9211_M_GPI2				0x04
#define	DA9211_M_GPI3				0x08
#define	DA9211_M_GPI4				0x10
#define	DA9211_M_UVLO_IO			0x40

/* DA9211_REG_MASK_B (addr=0x55) */
#define	DA9211_M_PWRGOOD_A			0x01
#define	DA9211_M_PWRGOOD_B			0x02
#define	DA9211_M_TEMP_WARN			0x04
#define	DA9211_M_TEMP_CRIT			0x08
#define	DA9211_M_OV_CURR_A			0x10
#define	DA9211_M_OV_CURR_B			0x20

/* DA9211_REG_CONTROL_A (addr=0x56) */
#define	DA9211_DEBOUNCING_SHIFT		0
#define	DA9211_DEBOUNCING_MASK		0x07
#define	DA9211_SLEW_RATE_SHIFT		3
#define	DA9211_SLEW_RATE_A_MASK		0x18
#define	DA9211_SLEW_RATE_B_SHIFT	5
#define	DA9211_SLEW_RATE_B_MASK		0x60
#define	DA9211_V_LOCK				0x80

/* DA9211_REG_GPIO_0_1 (addr=0x58) */
#define	DA9211_GPIO0_PIN_SHIFT		0
#define	DA9211_GPIO0_PIN_MASK		0x03
#define	DA9211_GPIO0_PIN_GPI		0x00
#define	DA9211_GPIO0_PIN_GPO_OD		0x02
#define	DA9211_GPIO0_PIN_GPO		0x03
#define	DA9211_GPIO0_TYPE			0x04
#define	DA9211_GPIO0_TYPE_GPI		0x00
#define	DA9211_GPIO0_TYPE_GPO		0x04
#define	DA9211_GPIO0_MODE			0x08
#define	DA9211_GPIO1_PIN_SHIFT		4
#define	DA9211_GPIO1_PIN_MASK		0x30
#define	DA9211_GPIO1_PIN_GPI		0x00
#define	DA9211_GPIO1_PIN_VERROR		0x10
#define	DA9211_GPIO1_PIN_GPO_OD		0x20
#define	DA9211_GPIO1_PIN_GPO		0x30
#define	DA9211_GPIO1_TYPE_SHIFT		0x40
#define	DA9211_GPIO1_TYPE_GPI		0x00
#define	DA9211_GPIO1_TYPE_GPO		0x40
#define	DA9211_GPIO1_MODE			0x80

/* DA9211_REG_GPIO_2_3 (addr=0x59) */
#define	DA9211_GPIO2_PIN_SHIFT		0
#define	DA9211_GPIO2_PIN_MASK		0x03
#define	DA9211_GPIO2_PIN_GPI		0x00
#define	DA9211_GPIO5_PIN_BUCK_CLK	0x10
#define	DA9211_GPIO2_PIN_GPO_OD		0x02
#define	DA9211_GPIO2_PIN_GPO		0x03
#define	DA9211_GPIO2_TYPE			0x04
#define	DA9211_GPIO2_TYPE_GPI		0x00
#define	DA9211_GPIO2_TYPE_GPO		0x04
#define	DA9211_GPIO2_MODE			0x08
#define	DA9211_GPIO3_PIN_SHIFT		4
#define	DA9211_GPIO3_PIN_MASK		0x30
#define	DA9211_GPIO3_PIN_GPI		0x00
#define	DA9211_GPIO3_PIN_IERROR		0x10
#define	DA9211_GPIO3_PIN_GPO_OD		0x20
#define	DA9211_GPIO3_PIN_GPO		0x30
#define	DA9211_GPIO3_TYPE_SHIFT		0x40
#define	DA9211_GPIO3_TYPE_GPI		0x00
#define	DA9211_GPIO3_TYPE_GPO		0x40
#define	DA9211_GPIO3_MODE			0x80

/* DA9211_REG_GPIO_4 (addr=0x5A) */
#define	DA9211_GPIO4_PIN_SHIFT		0
#define	DA9211_GPIO4_PIN_MASK		0x03
#define	DA9211_GPIO4_PIN_GPI		0x00
#define	DA9211_GPIO4_PIN_GPO_OD		0x02
#define	DA9211_GPIO4_PIN_GPO		0x03
#define	DA9211_GPIO4_TYPE			0x04
#define	DA9211_GPIO4_TYPE_GPI		0x00
#define	DA9211_GPIO4_TYPE_GPO		0x04
#define	DA9211_GPIO4_MODE			0x08

/* DA9211_REG_BUCKA_CONT (addr=0x5D) */
#define	DA9211_BUCKA_EN				0x01
#define	DA9211_BUCKA_GPI_SHIFT		1
#define DA9211_BUCKA_GPI_MASK		0x06
#define	DA9211_BUCKA_GPI_OFF		0x00
#define	DA9211_BUCKA_GPI_GPIO0		0x02
#define	DA9211_BUCKA_GPI_GPIO1		0x04
#define	DA9211_BUCKA_GPI_GPIO3		0x06
#define	DA9211_BUCKA_PD_DIS			0x08
#define	DA9211_VBUCKA_SEL			0x10
#define	DA9211_VBUCKA_SEL_A			0x00
#define	DA9211_VBUCKA_SEL_B			0x10
#define	DA9211_VBUCKA_GPI_SHIFT		5
#define	DA9211_VBUCKA_GPI_MASK		0x60
#define	DA9211_VBUCKA_GPI_OFF		0x00
#define	DA9211_VBUCKA_GPI_GPIO1		0x20
#define	DA9211_VBUCKA_GPI_GPIO2		0x40
#define	DA9211_VBUCKA_GPI_GPIO4		0x60

/* DA9211_REG_BUCKB_CONT (addr=0x5E) */
#define	DA9211_BUCKB_EN				0x01
#define	DA9211_BUCKB_GPI_SHIFT		1
#define DA9211_BUCKB_GPI_MASK		0x06
#define	DA9211_BUCKB_GPI_OFF		0x00
#define	DA9211_BUCKB_GPI_GPIO0		0x02
#define	DA9211_BUCKB_GPI_GPIO1		0x04
#define	DA9211_BUCKB_GPI_GPIO3		0x06
#define	DA9211_BUCKB_PD_DIS			0x08
#define	DA9211_VBUCKB_SEL			0x10
#define	DA9211_VBUCKB_SEL_A			0x00
#define	DA9211_VBUCKB_SEL_B			0x10
#define	DA9211_VBUCKB_GPI_SHIFT		5
#define	DA9211_VBUCKB_GPI_MASK		0x60
#define	DA9211_VBUCKB_GPI_OFF		0x00
#define	DA9211_VBUCKB_GPI_GPIO1		0x20
#define	DA9211_VBUCKB_GPI_GPIO2		0x40
#define	DA9211_VBUCKB_GPI_GPIO4		0x60

/* DA9211_REG_BUCK_ILIM (addr=0xD0) */
#define DA9211_BUCKA_ILIM_SHIFT			0
#define DA9211_BUCKA_ILIM_MASK			0x0F
#define DA9211_BUCKB_ILIM_SHIFT			4
#define DA9211_BUCKB_ILIM_MASK			0xF0

/* DA9211_REG_BUCKA_CONF (addr=0xD1) */
#define DA9211_BUCKA_MODE_SHIFT			0
#define DA9211_BUCKA_MODE_MASK			0x03
#define	DA9211_BUCKA_MODE_MANUAL		0x00
#define	DA9211_BUCKA_MODE_SLEEP			0x01
#define	DA9211_BUCKA_MODE_SYNC			0x02
#define	DA9211_BUCKA_MODE_AUTO			0x03
#define DA9211_BUCKA_UP_CTRL_SHIFT		2
#define DA9211_BUCKA_UP_CTRL_MASK		0x1C
#define DA9211_BUCKA_DOWN_CTRL_SHIFT	5
#define DA9211_BUCKA_DOWN_CTRL_MASK		0xE0

/* DA9211_REG_BUCKB_CONF (addr=0xD2) */
#define DA9211_BUCKB_MODE_SHIFT			0
#define DA9211_BUCKB_MODE_MASK			0x03
#define	DA9211_BUCKB_MODE_MANUAL		0x00
#define	DA9211_BUCKB_MODE_SLEEP			0x01
#define	DA9211_BUCKB_MODE_SYNC			0x02
#define	DA9211_BUCKB_MODE_AUTO			0x03
#define DA9211_BUCKB_UP_CTRL_SHIFT		2
#define DA9211_BUCKB_UP_CTRL_MASK		0x1C
#define DA9211_BUCKB_DOWN_CTRL_SHIFT	5
#define DA9211_BUCKB_DOWN_CTRL_MASK		0xE0

/* DA9211_REG_BUCK_CONF (addr=0xD3) */
#define DA9211_PHASE_SEL_A_SHIFT		0
#define DA9211_PHASE_SEL_A_MASK			0x03
#define DA9211_PHASE_SEL_B_SHIFT		2
#define DA9211_PHASE_SEL_B_MASK			0x04
#define DA9211_PH_SH_EN_A_SHIFT			3
#define DA9211_PH_SH_EN_A_MASK			0x08
#define DA9211_PH_SH_EN_B_SHIFT			4
#define DA9211_PH_SH_EN_B_MASK			0x10

/* DA9211_REG_VBUCKA_MAX (addr=0xD5) */
#define DA9211_VBUCKA_BASE_SHIFT		0
#define DA9211_VBUCKA_BASE_MASK			0x7F

/* DA9211_REG_VBUCKB_MAX (addr=0xD6) */
#define DA9211_VBUCKB_BASE_SHIFT		0
#define DA9211_VBUCKB_BASE_MASK			0x7F

/* DA9211_REG_VBUCKA/B_A/B (addr=0xD7/0xD8/0xD9/0xDA) */
#define DA9211_VBUCK_SHIFT			0
#define DA9211_VBUCK_MASK			0x7F
#define DA9211_VBUCK_BIAS			0
#define DA9211_BUCK_SL				0x80

/* DA9211_REG_INTERFACE (addr=0x105) */
#define DA9211_IF_BASE_ADDR_SHIFT		4
#define DA9211_IF_BASE_ADDR_MASK		0xF0

/* DA9211_REG_CONFIG_E (addr=0x147) */
#define DA9211_SLAVE_SEL			0x40

#endif	/* __DA9211_REGISTERS_H__ */
