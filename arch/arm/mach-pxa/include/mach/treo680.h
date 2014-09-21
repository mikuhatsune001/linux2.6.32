/*
 * GPIOs and interrupts for Palm Treo 680 smartphone
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef _INCLUDE_TREO680_H_
#define _INCLUDE_TREO680_H_

/* GPIOs */
#define GPIO_NR_TREO680_POWER_DETECT	0
#define GPIO_NR_TREO680_AMP_EN		27
#define GPIO_NR_TREO680_KEYB_BL		24
#define GPIO_NR_TREO680_VIBRATE_EN	44
#define GPIO_NR_TREO680_GREEN_LED	20
#define GPIO_NR_TREO680_RED_LED		79
#define GPIO_NR_TREO680_SD_DETECT_N	113
#define GPIO_NR_TREO680_SD_READONLY	33
#define GPIO_NR_TREO680_EP_DETECT_N	116
#define GPIO_NR_TREO680_SD_POWER	42
#define GPIO_NR_TREO680_USB_DETECT	1
#define GPIO_NR_TREO680_USB_PULLUP	114
#define GPIO_NR_TREO680_GSM_POWER	40
#define GPIO_NR_TREO680_GSM_RESET	87
#define GPIO_NR_TREO680_GSM_WAKE	57
#define GPIO_NR_TREO680_GSM_HOST_WAKE	14
#define GPIO_NR_TREO680_GSM_TRIGGER	10
#define GPIO_NR_TREO680_BT_EN		43
#define GPIO_NR_TREO680_IR_EN		115
#define GPIO_NR_TREO680_IR_TXD		47
#define GPIO_NR_TREO680_BL_POWER	38
#define GPIO_NR_TREO680_LCD_POWER	25

/* Various addresses  */
#define TREO680_PHYS_RAM_START	0xa0000000
#define TREO680_PHYS_IO_START	0x40000000
#define TREO680_STR_BASE	0xa2000000

/* BACKLIGHT */
#define TREO680_MAX_INTENSITY		254
#define TREO680_DEFAULT_INTENSITY	160
#define TREO680_LIMIT_MASK		0x7F
#define TREO680_PRESCALER		63
#define TREO680_PERIOD_NS		3500

#endif
