/*
 * include/asm-sh/ubc.h
 *
 * Copyright (C) 1999 Niibe Yutaka
 * Copyright (C) 2002, 2003 Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#ifndef __ASM_SH_UBC_H
#define __ASM_SH_UBC_H
#ifdef __KERNEL__

#include <cpu/ubc.h>

/* User Break Controller */
#if defined(CONFIG_CPU_SUBTYPE_SH7707) || defined(CONFIG_CPU_SUBTYPE_SH7709)
#define UBC_TYPE_SH7729	(current_cpu_data.type == CPU_SH7729)
#else
#define UBC_TYPE_SH7729	0
#endif

#define BAMR_ASID		(1 << 2)
#define BAMR_NONE		0
#define BAMR_10			0x1
#define BAMR_12			0x2
#define BAMR_ALL		0x3
#define BAMR_16			0x8
#define BAMR_20			0x9

#define BBR_INST		(1 << 4)
#define BBR_DATA		(2 << 4)
#define BBR_READ		(1 << 2)
#define BBR_WRITE		(2 << 2)
#define BBR_BYTE		0x1
#define BBR_HALF		0x2
#define BBR_LONG		0x3
#define BBR_QUAD		(1 << 6)	/* SH7750 */
#define BBR_CPU			(1 << 6)	/* SH7709A,SH7729 */
#define BBR_DMA			(2 << 6)	/* SH7709A,SH7729 */

#define BRCR_CMFA		(1 << 15)
#define BRCR_CMFB		(1 << 14)

#if defined CONFIG_CPU_SH2A
#define BRCR_CMFCA		(1 << 15)
#define BRCR_CMFCB		(1 << 14)
#define BRCR_CMFDA		(1 << 13)
#define BRCR_CMFDB		(1 << 12)
#define BRCR_PCBB		(1 << 6)	/* 1: after execution */
#define BRCR_PCBA		(1 << 5)	/* 1: after execution */
#define BRCR_PCTE		0
#else
#define BRCR_PCTE		(1 << 11)
#define BRCR_PCBA		(1 << 10)	/* 1: after execution */
#define BRCR_DBEB		(1 << 7)
#define BRCR_PCBB		(1 << 6)
#define BRCR_SEQ		(1 << 3)
#define BRCR_UBDE		(1 << 0)
#endif

#ifndef __ASSEMBLY__
/* arch/sh/kernel/cpu/ubc.S */
extern void ubc_sleep(void);

#ifdef CONFIG_UBC_WAKEUP
extern void ubc_wakeup(void);
#else
#define ubc_wakeup()	do { } while (0)
#endif
#endif

#endif /* __KERNEL__ */
#endif /* __ASM_SH_UBC_H */
