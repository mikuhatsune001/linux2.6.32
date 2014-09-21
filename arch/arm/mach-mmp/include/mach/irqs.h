#ifndef __ASM_MACH_IRQS_H
#define __ASM_MACH_IRQS_H

/*
 * Interrupt numbers for PXA168
 */
#define IRQ_PXA168_NONE			(-1)
#define IRQ_PXA168_SSP3			0
#define IRQ_PXA168_SSP2			1
#define IRQ_PXA168_SSP1			2
#define IRQ_PXA168_SSP0			3
#define IRQ_PXA168_PMIC_INT		4
#define IRQ_PXA168_RTC_INT		5
#define IRQ_PXA168_RTC_ALARM		6
#define IRQ_PXA168_TWSI0		7
#define IRQ_PXA168_GPU			8
#define IRQ_PXA168_KEYPAD		9
#define IRQ_PXA168_ONEWIRE		12
#define IRQ_PXA168_TIMER1		13
#define IRQ_PXA168_TIMER2		14
#define IRQ_PXA168_TIMER3		15
#define IRQ_PXA168_CMU			16
#define IRQ_PXA168_SSP4			17
#define IRQ_PXA168_MSP_WAKEUP		19
#define IRQ_PXA168_CF_WAKEUP		20
#define IRQ_PXA168_XD_WAKEUP		21
#define IRQ_PXA168_MFU			22
#define IRQ_PXA168_MSP			23
#define IRQ_PXA168_CF			24
#define IRQ_PXA168_XD			25
#define IRQ_PXA168_DDR_INT		26
#define IRQ_PXA168_UART1		27
#define IRQ_PXA168_UART2		28
#define IRQ_PXA168_WDT			35
#define IRQ_PXA168_FRQ_CHANGE		38
#define IRQ_PXA168_SDH1			39
#define IRQ_PXA168_SDH2			40
#define IRQ_PXA168_LCD			41
#define IRQ_PXA168_CI			42
#define IRQ_PXA168_USB1			44
#define IRQ_PXA168_NAND			45
#define IRQ_PXA168_HIFI_DMA		46
#define IRQ_PXA168_DMA_INT0		47
#define IRQ_PXA168_DMA_INT1		48
#define IRQ_PXA168_GPIOX		49
#define IRQ_PXA168_USB2			51
#define IRQ_PXA168_AC97			57
#define IRQ_PXA168_TWSI1		58
#define IRQ_PXA168_PMU			60
#define IRQ_PXA168_SM_INT		63

/*
 * Interrupt numbers for PXA910
 */
#define IRQ_PXA910_NONE			(-1)
#define IRQ_PXA910_AIRQ			0
#define IRQ_PXA910_SSP3			1
#define IRQ_PXA910_SSP2			2
#define IRQ_PXA910_SSP1			3
#define IRQ_PXA910_PMIC_INT		4
#define IRQ_PXA910_RTC_INT		5
#define IRQ_PXA910_RTC_ALARM		6
#define IRQ_PXA910_TWSI0		7
#define IRQ_PXA910_GPU			8
#define IRQ_PXA910_KEYPAD		9
#define IRQ_PXA910_ROTARY		10
#define IRQ_PXA910_TRACKBALL		11
#define IRQ_PXA910_ONEWIRE		12
#define IRQ_PXA910_AP1_TIMER1		13
#define IRQ_PXA910_AP1_TIMER2		14
#define IRQ_PXA910_AP1_TIMER3		15
#define IRQ_PXA910_IPC_AP0		16
#define IRQ_PXA910_IPC_AP1		17
#define IRQ_PXA910_IPC_AP2		18
#define IRQ_PXA910_IPC_AP3		19
#define IRQ_PXA910_IPC_AP4		20
#define IRQ_PXA910_IPC_CP0		21
#define IRQ_PXA910_IPC_CP1		22
#define IRQ_PXA910_IPC_CP2		23
#define IRQ_PXA910_IPC_CP3		24
#define IRQ_PXA910_IPC_CP4		25
#define IRQ_PXA910_L2_DDR		26
#define IRQ_PXA910_UART2		27
#define IRQ_PXA910_UART3		28
#define IRQ_PXA910_AP2_TIMER1		29
#define IRQ_PXA910_AP2_TIMER2		30
#define IRQ_PXA910_CP2_TIMER1		31
#define IRQ_PXA910_CP2_TIMER2		32
#define IRQ_PXA910_CP2_TIMER3		33
#define IRQ_PXA910_GSSP			34
#define IRQ_PXA910_CP2_WDT		35
#define IRQ_PXA910_MAIN_PMU		36
#define IRQ_PXA910_CP_FREQ_CHG		37
#define IRQ_PXA910_AP_FREQ_CHG		38
#define IRQ_PXA910_MMC			39
#define IRQ_PXA910_AEU			40
#define IRQ_PXA910_LCD			41
#define IRQ_PXA910_CCIC			42
#define IRQ_PXA910_IRE			43
#define IRQ_PXA910_USB1			44
#define IRQ_PXA910_NAND			45
#define IRQ_PXA910_HIFI_DMA		46
#define IRQ_PXA910_DMA_INT0		47
#define IRQ_PXA910_DMA_INT1		48
#define IRQ_PXA910_AP_GPIO		49
#define IRQ_PXA910_AP2_TIMER3		50
#define IRQ_PXA910_USB2			51
#define IRQ_PXA910_TWSI1		54
#define IRQ_PXA910_CP_GPIO		55
#define IRQ_PXA910_UART1		59	/* Slow UART */
#define IRQ_PXA910_AP_PMU		60
#define IRQ_PXA910_SM_INT		63	/* from PinMux */

#define IRQ_GPIO_START			64
#define IRQ_GPIO_NUM			128
#define IRQ_GPIO(x)			(IRQ_GPIO_START + (x))

#define NR_IRQS		(IRQ_GPIO_START + IRQ_GPIO_NUM)

#endif /* __ASM_MACH_IRQS_H */
