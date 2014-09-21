/*
 * stmp37xx: AUDIOOUT register definitions
 *
 * Copyright (c) 2008 Freescale Semiconductor
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#define REGS_AUDIOOUT_BASE	(STMP3XXX_REGS_BASE + 0x48000)

#define HW_AUDIOOUT_CTRL	0x0
#define BM_AUDIOOUT_CTRL_RUN	0x00000001
#define BP_AUDIOOUT_CTRL_RUN	0
#define BM_AUDIOOUT_CTRL_FIFO_ERROR_IRQ_EN	0x00000002
#define BM_AUDIOOUT_CTRL_FIFO_OVERFLOW_IRQ	0x00000004
#define BM_AUDIOOUT_CTRL_FIFO_UNDERFLOW_IRQ	0x00000008
#define BM_AUDIOOUT_CTRL_WORD_LENGTH	0x00000040
#define BM_AUDIOOUT_CTRL_CLKGATE	0x40000000
#define BM_AUDIOOUT_CTRL_SFTRST	0x80000000

#define HW_AUDIOOUT_STAT	0x10

#define HW_AUDIOOUT_DACSRR	0x20
#define BM_AUDIOOUT_DACSRR_SRC_FRAC	0x00001FFF
#define BP_AUDIOOUT_DACSRR_SRC_FRAC	0
#define BM_AUDIOOUT_DACSRR_SRC_INT	0x001F0000
#define BP_AUDIOOUT_DACSRR_SRC_INT	16
#define BM_AUDIOOUT_DACSRR_SRC_HOLD	0x07000000
#define BP_AUDIOOUT_DACSRR_SRC_HOLD	24
#define BM_AUDIOOUT_DACSRR_BASEMULT	0x70000000
#define BP_AUDIOOUT_DACSRR_BASEMULT	28

#define HW_AUDIOOUT_DACVOLUME	0x30
#define BM_AUDIOOUT_DACVOLUME_MUTE_RIGHT	0x00000100
#define BM_AUDIOOUT_DACVOLUME_MUTE_LEFT	0x01000000
#define BM_AUDIOOUT_DACVOLUME_EN_ZCD	0x02000000

#define HW_AUDIOOUT_DACDEBUG	0x40

#define HW_AUDIOOUT_HPVOL	0x50
#define BM_AUDIOOUT_HPVOL_MUTE	0x01000000
#define BM_AUDIOOUT_HPVOL_EN_MSTR_ZCD	0x02000000

#define HW_AUDIOOUT_PWRDN	0x70
#define BM_AUDIOOUT_PWRDN_HEADPHONE	0x00000001
#define BP_AUDIOOUT_PWRDN_HEADPHONE	0
#define BM_AUDIOOUT_PWRDN_CAPLESS	0x00000010
#define BM_AUDIOOUT_PWRDN_ADC	0x00000100
#define BM_AUDIOOUT_PWRDN_DAC	0x00001000
#define BM_AUDIOOUT_PWRDN_RIGHT_ADC	0x00010000
#define BM_AUDIOOUT_PWRDN_LINEOUT	0x01000000

#define HW_AUDIOOUT_REFCTRL	0x80
#define BM_AUDIOOUT_REFCTRL_VAG_VAL	0x000000F0
#define BP_AUDIOOUT_REFCTRL_VAG_VAL	4
#define BM_AUDIOOUT_REFCTRL_ADC_REFVAL	0x00000F00
#define BP_AUDIOOUT_REFCTRL_ADC_REFVAL	8
#define BM_AUDIOOUT_REFCTRL_ADJ_VAG	0x00001000
#define BM_AUDIOOUT_REFCTRL_ADJ_ADC	0x00002000
#define BM_AUDIOOUT_REFCTRL_BIAS_CTRL	0x00030000
#define BP_AUDIOOUT_REFCTRL_BIAS_CTRL	16
#define BM_AUDIOOUT_REFCTRL_LOW_PWR	0x00080000
#define BM_AUDIOOUT_REFCTRL_VBG_ADJ	0x00700000
#define BP_AUDIOOUT_REFCTRL_VBG_ADJ	20
#define BM_AUDIOOUT_REFCTRL_XTAL_BGR_BIAS	0x01000000
#define BM_AUDIOOUT_REFCTRL_RAISE_REF	0x02000000

#define HW_AUDIOOUT_ANACTRL	0x90
#define BM_AUDIOOUT_ANACTRL_HP_CLASSAB	0x00000010
#define BM_AUDIOOUT_ANACTRL_HP_HOLD_GND	0x00000020

#define HW_AUDIOOUT_TEST	0xA0
#define BM_AUDIOOUT_TEST_HP_I1_ADJ	0x00C00000
#define BP_AUDIOOUT_TEST_HP_I1_ADJ	22

#define HW_AUDIOOUT_BISTCTRL	0xB0

#define HW_AUDIOOUT_BISTSTAT0	0xC0

#define HW_AUDIOOUT_BISTSTAT1	0xD0

#define HW_AUDIOOUT_ANACLKCTRL	0xE0
#define BM_AUDIOOUT_ANACLKCTRL_CLKGATE	0x80000000

#define HW_AUDIOOUT_DATA	0xF0

#define HW_AUDIOOUT_LINEOUTCTRL	0x100
#define BM_AUDIOOUT_LINEOUTCTRL_VOL_RIGHT	0x0000001F
#define BP_AUDIOOUT_LINEOUTCTRL_VOL_RIGHT	0
#define BM_AUDIOOUT_LINEOUTCTRL_VOL_LEFT	0x00001F00
#define BP_AUDIOOUT_LINEOUTCTRL_VOL_LEFT	8
#define BM_AUDIOOUT_LINEOUTCTRL_CHARGE_CAP	0x00007000
#define BP_AUDIOOUT_LINEOUTCTRL_CHARGE_CAP	12
#define BM_AUDIOOUT_LINEOUTCTRL_VAG_CTRL	0x00F00000
#define BP_AUDIOOUT_LINEOUTCTRL_VAG_CTRL	20
#define BM_AUDIOOUT_LINEOUTCTRL_MUTE	0x01000000
#define BM_AUDIOOUT_LINEOUTCTRL_EN_ZCD	0x02000000

#define HW_AUDIOOUT_VERSION	0x200
