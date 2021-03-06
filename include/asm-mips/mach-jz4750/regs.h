/*
 * linux/include/asm-mips/mach-jz4750/regs.h
 *
 * JZ4750 register definition.
 *
 * Copyright (C) 2008 Ingenic Semiconductor Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __JZ4750_REGS_H__
#define __JZ4750_REGS_H__

#if defined(__ASSEMBLY__) || defined(__LANGUAGE_ASSEMBLY)
#define REG8(addr)	(addr)
#define REG16(addr)	(addr)
#define REG32(addr)	(addr)
#else
#define REG8(addr)	*((volatile unsigned char *)(addr))
#define REG16(addr)	*((volatile unsigned short *)(addr))
#define REG32(addr)	*((volatile unsigned int *)(addr))
#endif

/*
 * Define the module base addresses
 */
#define	CPM_BASE	0xB0000000
#define	INTC_BASE	0xB0001000
#define	TCU_BASE	0xB0002000
#define	WDT_BASE	0xB0002000
#define	RTC_BASE	0xB0003000
#define	GPIO_BASE	0xB0010000
#define	AIC_BASE	0xB0020000
#define	ICDC_BASE	0xB0020000
#define	MSC_BASE	0xB0021000
#define	UART0_BASE	0xB0030000
#define	UART1_BASE	0xB0031000
#define	UART2_BASE	0xB0032000
#define	UART3_BASE	0xB0033000
#define	I2C_BASE	0xB0042000
#define	SSI_BASE	0xB0043000
#define	SADC_BASE	0xB0070000
#define PCM_BASE        0xB0071000
#define	EMC_BASE	0xB3010000
#define	DMAC_BASE	0xB3020000
#define	UHC_BASE	0xB3030000
#define	UDC_BASE	0xB3040000
#define	LCD_BASE	0xB3050000
#define	SLCD_BASE	0xB3050000
#define	TVE_BASE	0xB3050200
#define	CIM_BASE	0xB3060000
#define IPU_BASE	0xB3080000
#define ME_BASE		0xB3090000
#define MC_BASE		0xB30A0000
#define BCH_BASE	0xB30D0000
#define	ETH_BASE	0xB3100000
#define	TCSM_BASE	0xF4000000
#define OWI_BASE        0XB0072000

/*************************************************************************
 * INTC (Interrupt Controller)
 *************************************************************************/
#define INTC_ISR	(INTC_BASE + 0x00)
#define INTC_IMR	(INTC_BASE + 0x04)
#define INTC_IMSR	(INTC_BASE + 0x08)
#define INTC_IMCR	(INTC_BASE + 0x0c)
#define INTC_IPR	(INTC_BASE + 0x10)

#define REG_INTC_ISR	REG32(INTC_ISR)
#define REG_INTC_IMR	REG32(INTC_IMR)
#define REG_INTC_IMSR	REG32(INTC_IMSR)
#define REG_INTC_IMCR	REG32(INTC_IMCR)
#define REG_INTC_IPR	REG32(INTC_IPR)

// 1st-level interrupts
#define IRQ_OWI		0
#define IRQ_I2C		1
#define IRQ_TSSI	2
#define IRQ_UART3	3
#define IRQ_UART2	4
#define IRQ_UART1	5
#define IRQ_UART0	6
#define IRQ_PCM		7
#define IRQ_AIC		8
#define IRQ_RTC		9
#define IRQ_ETH		9
#define IRQ_SADC	10
#define IRQ_SSI1	11
#define IRQ_SSI0	12
#define IRQ_MSC1	13
#define IRQ_MSC0	14
#define IRQ_DMAC1	15
#define IRQ_BCH		16
#define IRQ_UHC		17
#define IRQ_CIM		18
#define IRQ_UDC		19
#define IRQ_DMAC0	20
#define IRQ_TCU2	21
#define IRQ_TCU1	22
#define IRQ_TCU0	23
#define IRQ_GPIO5	24
#define IRQ_GPIO4	25
#define IRQ_GPIO3	26
#define IRQ_GPIO2	27
#define IRQ_GPIO1	28
#define IRQ_GPIO0	29
#define IRQ_IPU		30
#define IRQ_LCD		31

// 2nd-level interrupts
#define IRQ_DMA_0	32  /* 32 to 43 for DMAC0's 0-5  and DMAC1's 0-5 */
#define IRQ_GPIO_0	48  /* 48 to 240 for GPIO pin 0 to 192 */

#define NUM_DMA         MAX_DMA_NUM	/* 12 */
#define NUM_GPIO        MAX_GPIO_NUM	/* GPIO NUM: 192, Jz4750 real num GPIO 178 */


/*************************************************************************
 * RTC
 *************************************************************************/
#define RTC_RCR		(RTC_BASE + 0x00) /* RTC Control Register */
#define RTC_RSR		(RTC_BASE + 0x04) /* RTC Second Register */
#define RTC_RSAR	(RTC_BASE + 0x08) /* RTC Second Alarm Register */
#define RTC_RGR		(RTC_BASE + 0x0c) /* RTC Regulator Register */

#define RTC_HCR		(RTC_BASE + 0x20) /* Hibernate Control Register */
#define RTC_HWFCR	(RTC_BASE + 0x24) /* Hibernate Wakeup Filter Counter Reg */
#define RTC_HRCR	(RTC_BASE + 0x28) /* Hibernate Reset Counter Register */
#define RTC_HWCR	(RTC_BASE + 0x2c) /* Hibernate Wakeup Control Register */
#define RTC_HWRSR	(RTC_BASE + 0x30) /* Hibernate Wakeup Status Register */
#define RTC_HSPR	(RTC_BASE + 0x34) /* Hibernate Scratch Pattern Register */

#define REG_RTC_RCR	REG32(RTC_RCR)
#define REG_RTC_RSR	REG32(RTC_RSR)
#define REG_RTC_RSAR	REG32(RTC_RSAR)
#define REG_RTC_RGR	REG32(RTC_RGR)
#define REG_RTC_HCR	REG32(RTC_HCR)
#define REG_RTC_HWFCR	REG32(RTC_HWFCR)
#define REG_RTC_HRCR	REG32(RTC_HRCR)
#define REG_RTC_HWCR	REG32(RTC_HWCR)
#define REG_RTC_HWRSR	REG32(RTC_HWRSR)
#define REG_RTC_HSPR	REG32(RTC_HSPR)

/* RTC Control Register */
#define RTC_RCR_WRDY_BIT 7
#define RTC_RCR_WRDY	(1 << 7)  /* Write Ready Flag */
#define RTC_RCR_1HZ_BIT	6
#define RTC_RCR_1HZ	(1 << RTC_RCR_1HZ_BIT)  /* 1Hz Flag */
#define RTC_RCR_1HZIE	(1 << 5)  /* 1Hz Interrupt Enable */
#define RTC_RCR_AF_BIT	4
#define RTC_RCR_AF	(1 << RTC_RCR_AF_BIT)  /* Alarm Flag */
#define RTC_RCR_AIE	(1 << 3)  /* Alarm Interrupt Enable */
#define RTC_RCR_AE	(1 << 2)  /* Alarm Enable */
#define RTC_RCR_RTCE	(1 << 0)  /* RTC Enable */

/* RTC Regulator Register */
#define RTC_RGR_LOCK		(1 << 31) /* Lock Bit */
#define RTC_RGR_ADJC_BIT	16
#define RTC_RGR_ADJC_MASK	(0x3ff << RTC_RGR_ADJC_BIT)
#define RTC_RGR_NC1HZ_BIT	0
#define RTC_RGR_NC1HZ_MASK	(0xffff << RTC_RGR_NC1HZ_BIT)

/* Hibernate Control Register */
#define RTC_HCR_PD		(1 << 0)  /* Power Down */

/* Hibernate Wakeup Filter Counter Register */
#define RTC_HWFCR_BIT		5
#define RTC_HWFCR_MASK		(0x7ff << RTC_HWFCR_BIT)

/* Hibernate Reset Counter Register */
#define RTC_HRCR_BIT		5
#define RTC_HRCR_MASK		(0x7f << RTC_HRCR_BIT)

/* Hibernate Wakeup Control Register */
#define RTC_HWCR_EALM		(1 << 0)  /* RTC alarm wakeup enable */

/* Hibernate Wakeup Status Register */
#define RTC_HWRSR_HR		(1 << 5)  /* Hibernate reset */
#define RTC_HWRSR_PPR		(1 << 4)  /* PPR reset */
#define RTC_HWRSR_PIN		(1 << 1)  /* Wakeup pin status bit */
#define RTC_HWRSR_ALM		(1 << 0)  /* RTC alarm status bit */


/*************************************************************************
 * CPM (Clock reset and Power control Management)
 *************************************************************************/
#define CPM_CPCCR	(CPM_BASE+0x00)
#define CPM_CPPCR	(CPM_BASE+0x10)
#define CPM_I2SCDR	(CPM_BASE+0x60)
#define CPM_LPCDR	(CPM_BASE+0x64)
#define CPM_MSCCDR	(CPM_BASE+0x68)
#define CPM_UHCCDR	(CPM_BASE+0x6C)
#define CPM_SSICDR	(CPM_BASE+0x74)

#define CPM_LCR		(CPM_BASE+0x04)
#define CPM_CLKGR	(CPM_BASE+0x20)
#define CPM_SCR		(CPM_BASE+0x24)

#define CPM_HCR		(CPM_BASE+0x30)
#define CPM_HWFCR	(CPM_BASE+0x34)
#define CPM_HRCR	(CPM_BASE+0x38)
#define CPM_HWCR	(CPM_BASE+0x3c)
#define CPM_HWSR	(CPM_BASE+0x40)
#define CPM_HSPR	(CPM_BASE+0x44)

#define CPM_RSR		(CPM_BASE+0x08)


#define REG_CPM_CPCCR	REG32(CPM_CPCCR)
#define REG_CPM_CPPCR	REG32(CPM_CPPCR)
#define REG_CPM_I2SCDR	REG32(CPM_I2SCDR)
#define REG_CPM_LPCDR	REG32(CPM_LPCDR)
#define REG_CPM_MSCCDR	REG32(CPM_MSCCDR)
#define REG_CPM_UHCCDR	REG32(CPM_UHCCDR)
#define REG_CPM_SSICDR	REG32(CPM_SSICDR)

#define REG_CPM_LCR	REG32(CPM_LCR)
#define REG_CPM_CLKGR	REG32(CPM_CLKGR)
#define REG_CPM_SCR	REG32(CPM_SCR)
#define REG_CPM_HCR	REG32(CPM_HCR)
#define REG_CPM_HWFCR	REG32(CPM_HWFCR)
#define REG_CPM_HRCR	REG32(CPM_HRCR)
#define REG_CPM_HWCR	REG32(CPM_HWCR)
#define REG_CPM_HWSR	REG32(CPM_HWSR)
#define REG_CPM_HSPR	REG32(CPM_HSPR)

#define REG_CPM_RSR	REG32(CPM_RSR)


/* Clock Control Register */
#define CPM_CPCCR_I2CS		(1 << 31)
#define CPM_CPCCR_CLKOEN	(1 << 30)
#define CPM_CPCCR_UCS		(1 << 29)
#define CPM_CPCCR_UDIV_BIT	23
#define CPM_CPCCR_UDIV_MASK	(0x3f << CPM_CPCCR_UDIV_BIT)
#define CPM_CPCCR_CE		(1 << 22)
#define CPM_CPCCR_PCS		(1 << 21)
#define CPM_CPCCR_LDIV_BIT	16
#define CPM_CPCCR_LDIV_MASK	(0x1f << CPM_CPCCR_LDIV_BIT)
#define CPM_CPCCR_MDIV_BIT	12
#define CPM_CPCCR_MDIV_MASK	(0x0f << CPM_CPCCR_MDIV_BIT)
#define CPM_CPCCR_PDIV_BIT	8
#define CPM_CPCCR_PDIV_MASK	(0x0f << CPM_CPCCR_PDIV_BIT)
#define CPM_CPCCR_HDIV_BIT	4
#define CPM_CPCCR_HDIV_MASK	(0x0f << CPM_CPCCR_HDIV_BIT)
#define CPM_CPCCR_CDIV_BIT	0
#define CPM_CPCCR_CDIV_MASK	(0x0f << CPM_CPCCR_CDIV_BIT)

/* I2S Clock Divider Register */
#define CPM_I2SCDR_I2SDIV_BIT	0
#define CPM_I2SCDR_I2SDIV_MASK	(0x1ff << CPM_I2SCDR_I2SDIV_BIT)

/* LCD Pixel Clock Divider Register */
#define CPM_LPCDR_PIXDIV_BIT	0
#define CPM_LPCDR_PIXDIV_MASK	(0x1ff << CPM_LPCDR_PIXDIV_BIT)

/* MSC Clock Divider Register */
#define CPM_MSCCDR_MSCDIV_BIT	0
#define CPM_MSCCDR_MSCDIV_MASK	(0x1f << CPM_MSCCDR_MSCDIV_BIT)

/* UHC Clock Divider Register */
#define CPM_UHCCDR_UHCDIV_BIT	0
#define CPM_UHCCDR_UHCDIV_MASK	(0xf << CPM_MSCCDR_UHCDIV_BIT)

/* SSI Clock Divider Register */
#define CPM_SSICDR_SCS		(1<<31) /* SSI clock source selection, 0:EXCLK, 1: PLL */
#define CPM_SSICDR_SSIDIV_BIT	0
#define CPM_SSICDR_SSIDIV_MASK	(0xf << CPM_SSICDR_SSIDIV_BIT)

/* PLL Control Register */
#define CPM_CPPCR_PLLM_BIT	23
#define CPM_CPPCR_PLLM_MASK	(0x1ff << CPM_CPPCR_PLLM_BIT)
#define CPM_CPPCR_PLLN_BIT	18
#define CPM_CPPCR_PLLN_MASK	(0x1f << CPM_CPPCR_PLLN_BIT)
#define CPM_CPPCR_PLLOD_BIT	16
#define CPM_CPPCR_PLLOD_MASK	(0x03 << CPM_CPPCR_PLLOD_BIT)
#define CPM_CPPCR_PLLS		(1 << 10)
#define CPM_CPPCR_PLLBP		(1 << 9)
#define CPM_CPPCR_PLLEN		(1 << 8)
#define CPM_CPPCR_PLLST_BIT	0
#define CPM_CPPCR_PLLST_MASK	(0xff << CPM_CPPCR_PLLST_BIT)

/* Low Power Control Register */
#define CPM_LCR_DOZE_DUTY_BIT 	3
#define CPM_LCR_DOZE_DUTY_MASK 	(0x1f << CPM_LCR_DOZE_DUTY_BIT)
#define CPM_LCR_DOZE_ON		(1 << 2)
#define CPM_LCR_LPM_BIT		0
#define CPM_LCR_LPM_MASK	(0x3 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_IDLE	(0x0 << CPM_LCR_LPM_BIT)
  #define CPM_LCR_LPM_SLEEP	(0x1 << CPM_LCR_LPM_BIT)

/* Clock Gate Register */
#define CPM_CLKGR_UART1		(1 << 15)
#define CPM_CLKGR_UHC		(1 << 14)
#define CPM_CLKGR_IPU		(1 << 13)
#define CPM_CLKGR_DMAC		(1 << 12)
#define CPM_CLKGR_UDC		(1 << 11)
#define CPM_CLKGR_LCD		(1 << 10)
#define CPM_CLKGR_CIM		(1 << 9)
#define CPM_CLKGR_SADC		(1 << 8)
#define CPM_CLKGR_MSC		(1 << 7)
#define CPM_CLKGR_AIC1		(1 << 6)
#define CPM_CLKGR_AIC2		(1 << 5)
#define CPM_CLKGR_SSI		(1 << 4)
#define CPM_CLKGR_I2C		(1 << 3)
#define CPM_CLKGR_RTC		(1 << 2)
#define CPM_CLKGR_TCU		(1 << 1)
#define CPM_CLKGR_UART0		(1 << 0)

/* Sleep Control Register */
#define CPM_SCR_O1ST_BIT	8
#define CPM_SCR_O1ST_MASK	(0xff << CPM_SCR_O1ST_BIT)
#define CPM_SCR_USBPHY_ENABLE	(1 << 6)
#define CPM_SCR_OSC_ENABLE	(1 << 4)

/* Hibernate Control Register */
#define CPM_HCR_PD		(1 << 0)

/* Wakeup Filter Counter Register in Hibernate Mode */
#define CPM_HWFCR_TIME_BIT	0
#define CPM_HWFCR_TIME_MASK	(0x3ff << CPM_HWFCR_TIME_BIT)

/* Reset Counter Register in Hibernate Mode */
#define CPM_HRCR_TIME_BIT	0
#define CPM_HRCR_TIME_MASK	(0x7f << CPM_HRCR_TIME_BIT)

/* Wakeup Control Register in Hibernate Mode */
#define CPM_HWCR_WLE_LOW	(0 << 2)
#define CPM_HWCR_WLE_HIGH	(1 << 2)
#define CPM_HWCR_PIN_WAKEUP	(1 << 1)
#define CPM_HWCR_RTC_WAKEUP	(1 << 0)

/* Wakeup Status Register in Hibernate Mode */
#define CPM_HWSR_WSR_PIN	(1 << 1)
#define CPM_HWSR_WSR_RTC	(1 << 0)

/* Reset Status Register */
#define CPM_RSR_HR		(1 << 2)
#define CPM_RSR_WR		(1 << 1)
#define CPM_RSR_PR		(1 << 0)


/*************************************************************************
 * TCU (Timer Counter Unit)
 *************************************************************************/
#define TCU_TSTR	(TCU_BASE + 0xF0) /* Timer Status Register */
#define TCU_TSTSR	(TCU_BASE + 0xF4) /* Timer Status Set Register */
#define TCU_TSTCR	(TCU_BASE + 0xF8) /* Timer Status Clear Register */
#define TCU_TSR		(TCU_BASE + 0x1C) /* Timer Stop Register */
#define TCU_TSSR	(TCU_BASE + 0x2C) /* Timer Stop Set Register */
#define TCU_TSCR	(TCU_BASE + 0x3C) /* Timer Stop Clear Register */
#define TCU_TER		(TCU_BASE + 0x10) /* Timer Counter Enable Register */
#define TCU_TESR	(TCU_BASE + 0x14) /* Timer Counter Enable Set Register */
#define TCU_TECR	(TCU_BASE + 0x18) /* Timer Counter Enable Clear Register */
#define TCU_TFR		(TCU_BASE + 0x20) /* Timer Flag Register */
#define TCU_TFSR	(TCU_BASE + 0x24) /* Timer Flag Set Register */
#define TCU_TFCR	(TCU_BASE + 0x28) /* Timer Flag Clear Register */
#define TCU_TMR		(TCU_BASE + 0x30) /* Timer Mask Register */
#define TCU_TMSR	(TCU_BASE + 0x34) /* Timer Mask Set Register */
#define TCU_TMCR	(TCU_BASE + 0x38) /* Timer Mask Clear Register */

#define TCU_TDFR0	(TCU_BASE + 0x40) /* Timer Data Full Register */
#define TCU_TDHR0	(TCU_BASE + 0x44) /* Timer Data Half Register */
#define TCU_TCNT0	(TCU_BASE + 0x48) /* Timer Counter Register */
#define TCU_TCSR0	(TCU_BASE + 0x4C) /* Timer Control Register */
#define TCU_TDFR1	(TCU_BASE + 0x50)
#define TCU_TDHR1	(TCU_BASE + 0x54)
#define TCU_TCNT1	(TCU_BASE + 0x58)
#define TCU_TCSR1	(TCU_BASE + 0x5C)
#define TCU_TDFR2	(TCU_BASE + 0x60)
#define TCU_TDHR2	(TCU_BASE + 0x64)
#define TCU_TCNT2	(TCU_BASE + 0x68)
#define TCU_TCSR2	(TCU_BASE + 0x6C)
#define TCU_TDFR3	(TCU_BASE + 0x70)
#define TCU_TDHR3	(TCU_BASE + 0x74)
#define TCU_TCNT3	(TCU_BASE + 0x78)
#define TCU_TCSR3	(TCU_BASE + 0x7C)
#define TCU_TDFR4	(TCU_BASE + 0x80)
#define TCU_TDHR4	(TCU_BASE + 0x84)
#define TCU_TCNT4	(TCU_BASE + 0x88)
#define TCU_TCSR4	(TCU_BASE + 0x8C)
#define TCU_TDFR5	(TCU_BASE + 0x90)
#define TCU_TDHR5	(TCU_BASE + 0x94)
#define TCU_TCNT5	(TCU_BASE + 0x98)
#define TCU_TCSR5	(TCU_BASE + 0x9C)

#define REG_TCU_TSTR	REG32(TCU_TSTR)
#define REG_TCU_TSTSR	REG32(TCU_TSTSR)
#define REG_TCU_TSTCR	REG32(TCU_TSTCR)
#define REG_TCU_TSR	REG8(TCU_TSR)
#define REG_TCU_TSSR	REG8(TCU_TSSR)
#define REG_TCU_TSCR	REG8(TCU_TSCR)
#define REG_TCU_TER	REG8(TCU_TER)
#define REG_TCU_TESR	REG8(TCU_TESR)
#define REG_TCU_TECR	REG8(TCU_TECR)
#define REG_TCU_TFR	REG32(TCU_TFR)
#define REG_TCU_TFSR	REG32(TCU_TFSR)
#define REG_TCU_TFCR	REG32(TCU_TFCR)
#define REG_TCU_TMR	REG32(TCU_TMR)
#define REG_TCU_TMSR	REG32(TCU_TMSR)
#define REG_TCU_TMCR	REG32(TCU_TMCR)
#define REG_TCU_TDFR0	REG16(TCU_TDFR0)
#define REG_TCU_TDHR0	REG16(TCU_TDHR0)
#define REG_TCU_TCNT0	REG16(TCU_TCNT0)
#define REG_TCU_TCSR0	REG16(TCU_TCSR0)
#define REG_TCU_TDFR1	REG16(TCU_TDFR1)
#define REG_TCU_TDHR1	REG16(TCU_TDHR1)
#define REG_TCU_TCNT1	REG16(TCU_TCNT1)
#define REG_TCU_TCSR1	REG16(TCU_TCSR1)
#define REG_TCU_TDFR2	REG16(TCU_TDFR2)
#define REG_TCU_TDHR2	REG16(TCU_TDHR2)
#define REG_TCU_TCNT2	REG16(TCU_TCNT2)
#define REG_TCU_TCSR2	REG16(TCU_TCSR2)
#define REG_TCU_TDFR3	REG16(TCU_TDFR3)
#define REG_TCU_TDHR3	REG16(TCU_TDHR3)
#define REG_TCU_TCNT3	REG16(TCU_TCNT3)
#define REG_TCU_TCSR3	REG16(TCU_TCSR3)
#define REG_TCU_TDFR4	REG16(TCU_TDFR4)
#define REG_TCU_TDHR4	REG16(TCU_TDHR4)
#define REG_TCU_TCNT4	REG16(TCU_TCNT4)
#define REG_TCU_TCSR4	REG16(TCU_TCSR4)

// n = 0,1,2,3,4,5
#define TCU_TDFR(n)	(TCU_BASE + (0x40 + (n)*0x10)) /* Timer Data Full Reg */
#define TCU_TDHR(n)	(TCU_BASE + (0x44 + (n)*0x10)) /* Timer Data Half Reg */
#define TCU_TCNT(n)	(TCU_BASE + (0x48 + (n)*0x10)) /* Timer Counter Reg */
#define TCU_TCSR(n)	(TCU_BASE + (0x4C + (n)*0x10)) /* Timer Control Reg */

#define REG_TCU_TDFR(n)	REG16(TCU_TDFR((n)))
#define REG_TCU_TDHR(n)	REG16(TCU_TDHR((n)))
#define REG_TCU_TCNT(n)	REG16(TCU_TCNT((n)))
#define REG_TCU_TCSR(n)	REG16(TCU_TCSR((n)))

// Register definitions
#define TCU_TCSR_PWM_SD		(1 << 9)
#define TCU_TCSR_PWM_INITL_HIGH	(1 << 8)
#define TCU_TCSR_PWM_EN		(1 << 7)
#define TCU_TCSR_PRESCALE_BIT	3
#define TCU_TCSR_PRESCALE_MASK	(0x7 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1	(0x0 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE4	(0x1 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE16	(0x2 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE64	(0x3 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE256	(0x4 << TCU_TCSR_PRESCALE_BIT)
  #define TCU_TCSR_PRESCALE1024	(0x5 << TCU_TCSR_PRESCALE_BIT)
#define TCU_TCSR_EXT_EN		(1 << 2)
#define TCU_TCSR_RTC_EN		(1 << 1)
#define TCU_TCSR_PCK_EN		(1 << 0)

#define TCU_TER_TCEN5		(1 << 5)
#define TCU_TER_TCEN4		(1 << 4)
#define TCU_TER_TCEN3		(1 << 3)
#define TCU_TER_TCEN2		(1 << 2)
#define TCU_TER_TCEN1		(1 << 1)
#define TCU_TER_TCEN0		(1 << 0)

#define TCU_TESR_TCST5		(1 << 5)
#define TCU_TESR_TCST4		(1 << 4)
#define TCU_TESR_TCST3		(1 << 3)
#define TCU_TESR_TCST2		(1 << 2)
#define TCU_TESR_TCST1		(1 << 1)
#define TCU_TESR_TCST0		(1 << 0)

#define TCU_TECR_TCCL5		(1 << 5)
#define TCU_TECR_TCCL4		(1 << 4)
#define TCU_TECR_TCCL3		(1 << 3)
#define TCU_TECR_TCCL2		(1 << 2)
#define TCU_TECR_TCCL1		(1 << 1)
#define TCU_TECR_TCCL0		(1 << 0)

#define TCU_TFR_HFLAG5		(1 << 21)
#define TCU_TFR_HFLAG4		(1 << 20)
#define TCU_TFR_HFLAG3		(1 << 19)
#define TCU_TFR_HFLAG2		(1 << 18)
#define TCU_TFR_HFLAG1		(1 << 17)
#define TCU_TFR_HFLAG0		(1 << 16)
#define TCU_TFR_FFLAG5		(1 << 5)
#define TCU_TFR_FFLAG4		(1 << 4)
#define TCU_TFR_FFLAG3		(1 << 3)
#define TCU_TFR_FFLAG2		(1 << 2)
#define TCU_TFR_FFLAG1		(1 << 1)
#define TCU_TFR_FFLAG0		(1 << 0)

#define TCU_TFSR_HFLAG5		(1 << 21)
#define TCU_TFSR_HFLAG4		(1 << 20)
#define TCU_TFSR_HFLAG3		(1 << 19)
#define TCU_TFSR_HFLAG2		(1 << 18)
#define TCU_TFSR_HFLAG1		(1 << 17)
#define TCU_TFSR_HFLAG0		(1 << 16)
#define TCU_TFSR_FFLAG5		(1 << 5)
#define TCU_TFSR_FFLAG4		(1 << 4)
#define TCU_TFSR_FFLAG3		(1 << 3)
#define TCU_TFSR_FFLAG2		(1 << 2)
#define TCU_TFSR_FFLAG1		(1 << 1)
#define TCU_TFSR_FFLAG0		(1 << 0)

#define TCU_TFCR_HFLAG5		(1 << 21)
#define TCU_TFCR_HFLAG4		(1 << 20)
#define TCU_TFCR_HFLAG3		(1 << 19)
#define TCU_TFCR_HFLAG2		(1 << 18)
#define TCU_TFCR_HFLAG1		(1 << 17)
#define TCU_TFCR_HFLAG0		(1 << 16)
#define TCU_TFCR_FFLAG5		(1 << 5)
#define TCU_TFCR_FFLAG4		(1 << 4)
#define TCU_TFCR_FFLAG3		(1 << 3)
#define TCU_TFCR_FFLAG2		(1 << 2)
#define TCU_TFCR_FFLAG1		(1 << 1)
#define TCU_TFCR_FFLAG0		(1 << 0)

#define TCU_TMR_HMASK5		(1 << 21)
#define TCU_TMR_HMASK4		(1 << 20)
#define TCU_TMR_HMASK3		(1 << 19)
#define TCU_TMR_HMASK2		(1 << 18)
#define TCU_TMR_HMASK1		(1 << 17)
#define TCU_TMR_HMASK0		(1 << 16)
#define TCU_TMR_FMASK5		(1 << 5)
#define TCU_TMR_FMASK4		(1 << 4)
#define TCU_TMR_FMASK3		(1 << 3)
#define TCU_TMR_FMASK2		(1 << 2)
#define TCU_TMR_FMASK1		(1 << 1)
#define TCU_TMR_FMASK0		(1 << 0)

#define TCU_TMSR_HMST5		(1 << 21)
#define TCU_TMSR_HMST4		(1 << 20)
#define TCU_TMSR_HMST3		(1 << 19)
#define TCU_TMSR_HMST2		(1 << 18)
#define TCU_TMSR_HMST1		(1 << 17)
#define TCU_TMSR_HMST0		(1 << 16)
#define TCU_TMSR_FMST5		(1 << 5)
#define TCU_TMSR_FMST4		(1 << 4)
#define TCU_TMSR_FMST3		(1 << 3)
#define TCU_TMSR_FMST2		(1 << 2)
#define TCU_TMSR_FMST1		(1 << 1)
#define TCU_TMSR_FMST0		(1 << 0)

#define TCU_TMCR_HMCL5		(1 << 21)
#define TCU_TMCR_HMCL4		(1 << 20)
#define TCU_TMCR_HMCL3		(1 << 19)
#define TCU_TMCR_HMCL2		(1 << 18)
#define TCU_TMCR_HMCL1		(1 << 17)
#define TCU_TMCR_HMCL0		(1 << 16)
#define TCU_TMCR_FMCL5		(1 << 5)
#define TCU_TMCR_FMCL4		(1 << 4)
#define TCU_TMCR_FMCL3		(1 << 3)
#define TCU_TMCR_FMCL2		(1 << 2)
#define TCU_TMCR_FMCL1		(1 << 1)
#define TCU_TMCR_FMCL0		(1 << 0)

#define TCU_TSR_WDTS		(1 << 16)
#define TCU_TSR_STOP5		(1 << 5)
#define TCU_TSR_STOP4		(1 << 4)
#define TCU_TSR_STOP3		(1 << 3)
#define TCU_TSR_STOP2		(1 << 2)
#define TCU_TSR_STOP1		(1 << 1)
#define TCU_TSR_STOP0		(1 << 0)

#define TCU_TSSR_WDTSS		(1 << 16)
#define TCU_TSSR_STPS5		(1 << 5)
#define TCU_TSSR_STPS4		(1 << 4)
#define TCU_TSSR_STPS3		(1 << 3)
#define TCU_TSSR_STPS2		(1 << 2)
#define TCU_TSSR_STPS1		(1 << 1)
#define TCU_TSSR_STPS0		(1 << 0)

#define TCU_TSSR_WDTSC		(1 << 16)
#define TCU_TSSR_STPC5		(1 << 5)
#define TCU_TSSR_STPC4		(1 << 4)
#define TCU_TSSR_STPC3		(1 << 3)
#define TCU_TSSR_STPC2		(1 << 2)
#define TCU_TSSR_STPC1		(1 << 1)
#define TCU_TSSR_STPC0		(1 << 0)


/*************************************************************************
 * WDT (WatchDog Timer)
 *************************************************************************/
#define WDT_TDR		(WDT_BASE + 0x00)
#define WDT_TCER	(WDT_BASE + 0x04)
#define WDT_TCNT	(WDT_BASE + 0x08)
#define WDT_TCSR	(WDT_BASE + 0x0C)

#define REG_WDT_TDR	REG16(WDT_TDR)
#define REG_WDT_TCER	REG8(WDT_TCER)
#define REG_WDT_TCNT	REG16(WDT_TCNT)
#define REG_WDT_TCSR	REG16(WDT_TCSR)

// Register definition
#define WDT_TCSR_PRESCALE_BIT	3
#define WDT_TCSR_PRESCALE_MASK	(0x7 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1	(0x0 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE4	(0x1 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE16	(0x2 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE64	(0x3 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE256	(0x4 << WDT_TCSR_PRESCALE_BIT)
  #define WDT_TCSR_PRESCALE1024	(0x5 << WDT_TCSR_PRESCALE_BIT)
#define WDT_TCSR_EXT_EN		(1 << 2)
#define WDT_TCSR_RTC_EN		(1 << 1)
#define WDT_TCSR_PCK_EN		(1 << 0)

#define WDT_TCER_TCEN		(1 << 0)


/*************************************************************************
 * DMAC (DMA Controller)
 *************************************************************************/

#define MAX_DMA_NUM	12  /* max 12 channels */
#define HALF_DMA_NUM	6   /* the number of one dma controller's channels */

/* m is the DMA controller index (0, 1), n is the DMA channel index (0 - 11) */

#define DMAC_DSAR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x00 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA source address */
#define DMAC_DTAR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x04 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA target address */
#define DMAC_DTCR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x08 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA transfer count */
#define DMAC_DRSR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x0c + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA request source */
#define DMAC_DCCSR(n) (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x10 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA control/status */
#define DMAC_DCMD(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x14 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA command */
#define DMAC_DDA(n)   (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x18 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA descriptor address */
#define DMAC_DSD(n)   (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0xc0 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x04)) /* DMA Stride Address */

#define DMAC_DMACR(m)	(DMAC_BASE + 0x0300 + 0x100 * (m))              /* DMA control register */
#define DMAC_DMAIPR(m)	(DMAC_BASE + 0x0304 + 0x100 * (m))              /* DMA interrupt pending */
#define DMAC_DMADBR(m)	(DMAC_BASE + 0x0308 + 0x100 * (m))              /* DMA doorbell */
#define DMAC_DMADBSR(m)	(DMAC_BASE + 0x030C + 0x100 * (m))              /* DMA doorbell set */

#define REG_DMAC_DSAR(n)	REG32(DMAC_DSAR((n)))
#define REG_DMAC_DTAR(n)	REG32(DMAC_DTAR((n)))
#define REG_DMAC_DTCR(n)	REG32(DMAC_DTCR((n)))
#define REG_DMAC_DRSR(n)	REG32(DMAC_DRSR((n)))
#define REG_DMAC_DCCSR(n)	REG32(DMAC_DCCSR((n)))
#define REG_DMAC_DCMD(n)	REG32(DMAC_DCMD((n)))
#define REG_DMAC_DDA(n)		REG32(DMAC_DDA((n)))
#define REG_DMAC_DSD(n)         REG32(DMAC_DSD(n))
#define REG_DMAC_DMACR(m)	REG32(DMAC_DMACR(m))
#define REG_DMAC_DMAIPR(m)	REG32(DMAC_DMAIPR(m))
#define REG_DMAC_DMADBR(m)	REG32(DMAC_DMADBR(m))
#define REG_DMAC_DMADBSR(m)	REG32(DMAC_DMADBSR(m))

// DMA request source register
#define DMAC_DRSR_RS_BIT	0
#define DMAC_DRSR_RS_MASK	(0x1f << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_EXT	(0 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_NAND	(1 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_BCH_ENC	(2 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_BCH_DEC	(3 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_AUTO	(8 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_TSSIIN	(9 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART3OUT	(14 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART3IN	(15 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART2OUT	(16 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART2IN	(17 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART1OUT	(18 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART1IN	(19 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART0OUT	(20 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_UART0IN	(21 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI0OUT	(22 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI0IN	(23 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_AICOUT	(24 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_AICIN	(25 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC0OUT	(26 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC0IN	(27 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_TCU	(28 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SADC	(29 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC1OUT	(30 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_MSC1IN	(31 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI1OUT	(32 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_SSI1IN	(33 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PMOUT	(34 << DMAC_DRSR_RS_BIT)
  #define DMAC_DRSR_RS_PMIN	(35 << DMAC_DRSR_RS_BIT)

// DMA channel control/status register
#define DMAC_DCCSR_NDES		(1 << 31) /* descriptor (0) or not (1) ? */
#define DMAC_DCCSR_DES8    	(1 << 30) /* Descriptor 8 Word */
#define DMAC_DCCSR_DES4    	(0 << 30) /* Descriptor 4 Word */
#define DMAC_DCCSR_CDOA_BIT	16        /* copy of DMA offset address */
#define DMAC_DCCSR_CDOA_MASK	(0xff << DMAC_DCCSR_CDOA_BIT)
#define DMAC_DCCSR_BERR		(1 << 7)  /* BCH error within this transfer, Only for channel 0 */
#define DMAC_DCCSR_INV		(1 << 6)  /* descriptor invalid */
#define DMAC_DCCSR_AR		(1 << 4)  /* address error */
#define DMAC_DCCSR_TT		(1 << 3)  /* transfer terminated */
#define DMAC_DCCSR_HLT		(1 << 2)  /* DMA halted */
#define DMAC_DCCSR_CT		(1 << 1)  /* count terminated */
#define DMAC_DCCSR_EN		(1 << 0)  /* channel enable bit */

// DMA channel command register 
#define DMAC_DCMD_EACKS_LOW  	(1 << 31) /* External DACK Output Level Select, active low */
#define DMAC_DCMD_EACKS_HIGH  	(0 << 31) /* External DACK Output Level Select, active high */
#define DMAC_DCMD_EACKM_WRITE 	(1 << 30) /* External DACK Output Mode Select, output in write cycle */
#define DMAC_DCMD_EACKM_READ 	(0 << 30) /* External DACK Output Mode Select, output in read cycle */
#define DMAC_DCMD_ERDM_BIT      28        /* External DREQ Detection Mode Select */
#define DMAC_DCMD_ERDM_MASK     (0x03 << DMAC_DCMD_ERDM_BIT)
  #define DMAC_DCMD_ERDM_LOW    (0 << DMAC_DCMD_ERDM_BIT)
  #define DMAC_DCMD_ERDM_FALL   (1 << DMAC_DCMD_ERDM_BIT)
  #define DMAC_DCMD_ERDM_HIGH   (2 << DMAC_DCMD_ERDM_BIT)
  #define DMAC_DCMD_ERDM_RISE   (3 << DMAC_DCMD_ERDM_BIT)
#define DMAC_DCMD_BLAST		(1 << 25) /* BCH last */
#define DMAC_DCMD_SAI		(1 << 23) /* source address increment */
#define DMAC_DCMD_DAI		(1 << 22) /* dest address increment */
#define DMAC_DCMD_RDIL_BIT	16        /* request detection interval length */
#define DMAC_DCMD_RDIL_MASK	(0x0f << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_IGN	(0 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_2	(1 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_4	(2 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_8	(3 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_12	(4 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_16	(5 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_20	(6 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_24	(7 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_28	(8 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_32	(9 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_48	(10 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_60	(11 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_64	(12 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_124	(13 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_128	(14 << DMAC_DCMD_RDIL_BIT)
  #define DMAC_DCMD_RDIL_200	(15 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_SWDH_BIT	14  /* source port width */
#define DMAC_DCMD_SWDH_MASK	(0x03 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_32	(0 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_8	(1 << DMAC_DCMD_SWDH_BIT)
  #define DMAC_DCMD_SWDH_16	(2 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_DWDH_BIT	12  /* dest port width */
#define DMAC_DCMD_DWDH_MASK	(0x03 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_32	(0 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_8	(1 << DMAC_DCMD_DWDH_BIT)
  #define DMAC_DCMD_DWDH_16	(2 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DS_BIT	8  /* transfer data size of a data unit */
#define DMAC_DCMD_DS_MASK	(0x07 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_32BIT	(0 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_8BIT	(1 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_16BIT	(2 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_16BYTE	(3 << DMAC_DCMD_DS_BIT)
  #define DMAC_DCMD_DS_32BYTE	(4 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_STDE   	(1 << 5)  /* Stride Disable/Enable */
#define DMAC_DCMD_DES_V		(1 << 4)  /* descriptor valid flag */
#define DMAC_DCMD_DES_VM	(1 << 3)  /* descriptor valid mask: 1:support V-bit */
#define DMAC_DCMD_DES_VIE	(1 << 2)  /* DMA valid error interrupt enable */
#define DMAC_DCMD_TIE		(1 << 1)  /* DMA transfer interrupt enable */
#define DMAC_DCMD_LINK		(1 << 0)  /* descriptor link enable */

// DMA descriptor address register
#define DMAC_DDA_BASE_BIT	12  /* descriptor base address */
#define DMAC_DDA_BASE_MASK	(0x0fffff << DMAC_DDA_BASE_BIT)
#define DMAC_DDA_OFFSET_BIT	4   /* descriptor offset address */
#define DMAC_DDA_OFFSET_MASK	(0x0ff << DMAC_DDA_OFFSET_BIT)

// DMA stride address register
#define DMAC_DSD_TSD_BIT        16  /* target stride address */
#define DMAC_DSD_TSD_MASK      	(0xffff << DMAC_DSD_TSD_BIT)
#define DMAC_DSD_SSD_BIT        0  /* source stride address */
#define DMAC_DSD_SSD_MASK      	(0xffff << DMAC_DSD_SSD_BIT)

// DMA control register
#define DMAC_DMACR_FMSC		(1 << 31)  /* MSC Fast DMA mode */
#define DMAC_DMACR_FSSI		(1 << 30)  /* SSI Fast DMA mode */
#define DMAC_DMACR_FTSSI	(1 << 29)  /* TSSI Fast DMA mode */
#define DMAC_DMACR_FUART	(1 << 28)  /* UART Fast DMA mode */
#define DMAC_DMACR_FAIC		(1 << 27)  /* AIC Fast DMA mode */
#define DMAC_DMACR_PR_BIT	8  /* channel priority mode */
#define DMAC_DMACR_PR_MASK	(0x03 << DMAC_DMACR_PR_BIT)
  #define DMAC_DMACR_PR_012345	(0 << DMAC_DMACR_PR_BIT)
  #define DMAC_DMACR_PR_120345	(1 << DMAC_DMACR_PR_BIT)
  #define DMAC_DMACR_PR_230145	(2 << DMAC_DMACR_PR_BIT)
  #define DMAC_DMACR_PR_340125	(3 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_HLT		(1 << 3)  /* DMA halt flag */
#define DMAC_DMACR_AR		(1 << 2)  /* address error flag */
#define DMAC_DMACR_DMAE		(1 << 0)  /* DMA enable bit */

// DMA doorbell register
#define DMAC_DMADBR_DB5		(1 << 5)  /* doorbell for channel 5 */
#define DMAC_DMADBR_DB4		(1 << 4)  /* doorbell for channel 4 */
#define DMAC_DMADBR_DB3		(1 << 3)  /* doorbell for channel 3 */
#define DMAC_DMADBR_DB2		(1 << 2)  /* doorbell for channel 2 */
#define DMAC_DMADBR_DB1		(1 << 1)  /* doorbell for channel 1 */
#define DMAC_DMADBR_DB0		(1 << 0)  /* doorbell for channel 0 */

// DMA doorbell set register
#define DMAC_DMADBSR_DBS5	(1 << 5)  /* enable doorbell for channel 5 */
#define DMAC_DMADBSR_DBS4	(1 << 4)  /* enable doorbell for channel 4 */
#define DMAC_DMADBSR_DBS3	(1 << 3)  /* enable doorbell for channel 3 */
#define DMAC_DMADBSR_DBS2	(1 << 2)  /* enable doorbell for channel 2 */
#define DMAC_DMADBSR_DBS1	(1 << 1)  /* enable doorbell for channel 1 */
#define DMAC_DMADBSR_DBS0	(1 << 0)  /* enable doorbell for channel 0 */

// DMA interrupt pending register
#define DMAC_DMAIPR_CIRQ5	(1 << 5)  /* irq pending status for channel 5 */
#define DMAC_DMAIPR_CIRQ4	(1 << 4)  /* irq pending status for channel 4 */
#define DMAC_DMAIPR_CIRQ3	(1 << 3)  /* irq pending status for channel 3 */
#define DMAC_DMAIPR_CIRQ2	(1 << 2)  /* irq pending status for channel 2 */
#define DMAC_DMAIPR_CIRQ1	(1 << 1)  /* irq pending status for channel 1 */
#define DMAC_DMAIPR_CIRQ0	(1 << 0)  /* irq pending status for channel 0 */


/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
#define MAX_GPIO_NUM	192

//n = 0,1,2,3,4,5 (PORTA, PORTB, PORTC, PORTD, PORTE, PORTF)
#define GPIO_PXPIN(n)	(GPIO_BASE + (0x00 + (n)*0x100)) /* PIN Level Register */
#define GPIO_PXDAT(n)	(GPIO_BASE + (0x10 + (n)*0x100)) /* Port Data Register */
#define GPIO_PXDATS(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Data Set Register */
#define GPIO_PXDATC(n)	(GPIO_BASE + (0x18 + (n)*0x100)) /* Port Data Clear Register */
#define GPIO_PXIM(n)	(GPIO_BASE + (0x20 + (n)*0x100)) /* Interrupt Mask Register */
#define GPIO_PXIMS(n)	(GPIO_BASE + (0x24 + (n)*0x100)) /* Interrupt Mask Set Reg */
#define GPIO_PXIMC(n)	(GPIO_BASE + (0x28 + (n)*0x100)) /* Interrupt Mask Clear Reg */
#define GPIO_PXPE(n)	(GPIO_BASE + (0x30 + (n)*0x100)) /* Pull Enable Register */
#define GPIO_PXPES(n)	(GPIO_BASE + (0x34 + (n)*0x100)) /* Pull Enable Set Reg. */
#define GPIO_PXPEC(n)	(GPIO_BASE + (0x38 + (n)*0x100)) /* Pull Enable Clear Reg. */
#define GPIO_PXFUN(n)	(GPIO_BASE + (0x40 + (n)*0x100)) /* Function Register */
#define GPIO_PXFUNS(n)	(GPIO_BASE + (0x44 + (n)*0x100)) /* Function Set Register */
#define GPIO_PXFUNC(n)	(GPIO_BASE + (0x48 + (n)*0x100)) /* Function Clear Register */
#define GPIO_PXSEL(n)	(GPIO_BASE + (0x50 + (n)*0x100)) /* Select Register */
#define GPIO_PXSELS(n)	(GPIO_BASE + (0x54 + (n)*0x100)) /* Select Set Register */
#define GPIO_PXSELC(n)	(GPIO_BASE + (0x58 + (n)*0x100)) /* Select Clear Register */
#define GPIO_PXDIR(n)	(GPIO_BASE + (0x60 + (n)*0x100)) /* Direction Register */
#define GPIO_PXDIRS(n)	(GPIO_BASE + (0x64 + (n)*0x100)) /* Direction Set Register */
#define GPIO_PXDIRC(n)	(GPIO_BASE + (0x68 + (n)*0x100)) /* Direction Clear Register */
#define GPIO_PXTRG(n)	(GPIO_BASE + (0x70 + (n)*0x100)) /* Trigger Register */
#define GPIO_PXTRGS(n)	(GPIO_BASE + (0x74 + (n)*0x100)) /* Trigger Set Register */
#define GPIO_PXTRGC(n)	(GPIO_BASE + (0x78 + (n)*0x100)) /* Trigger Set Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (0x80 + (n)*0x100)) /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Flag Clear Register */

#define REG_GPIO_PXPIN(n)	REG32(GPIO_PXPIN((n)))  /* PIN level */
#define REG_GPIO_PXDAT(n)	REG32(GPIO_PXDAT((n)))  /* 1: interrupt pending */
#define REG_GPIO_PXDATS(n)	REG32(GPIO_PXDATS((n)))
#define REG_GPIO_PXDATC(n)	REG32(GPIO_PXDATC((n)))
#define REG_GPIO_PXIM(n)	REG32(GPIO_PXIM((n)))   /* 1: mask pin interrupt */
#define REG_GPIO_PXIMS(n)	REG32(GPIO_PXIMS((n)))
#define REG_GPIO_PXIMC(n)	REG32(GPIO_PXIMC((n)))
#define REG_GPIO_PXPE(n)	REG32(GPIO_PXPE((n)))   /* 1: disable pull up/down */
#define REG_GPIO_PXPES(n)	REG32(GPIO_PXPES((n)))
#define REG_GPIO_PXPEC(n)	REG32(GPIO_PXPEC((n)))
#define REG_GPIO_PXFUN(n)	REG32(GPIO_PXFUN((n)))  /* 0:GPIO or intr, 1:FUNC */
#define REG_GPIO_PXFUNS(n)	REG32(GPIO_PXFUNS((n)))
#define REG_GPIO_PXFUNC(n)	REG32(GPIO_PXFUNC((n)))
#define REG_GPIO_PXSEL(n)	REG32(GPIO_PXSEL((n))) /* 0:GPIO/Fun0,1:intr/fun1*/
#define REG_GPIO_PXSELS(n)	REG32(GPIO_PXSELS((n)))
#define REG_GPIO_PXSELC(n)	REG32(GPIO_PXSELC((n)))
#define REG_GPIO_PXDIR(n)	REG32(GPIO_PXDIR((n))) /* 0:input/low-level-trig/falling-edge-trig, 1:output/high-level-trig/rising-edge-trig */
#define REG_GPIO_PXDIRS(n)	REG32(GPIO_PXDIRS((n)))
#define REG_GPIO_PXDIRC(n)	REG32(GPIO_PXDIRC((n)))
#define REG_GPIO_PXTRG(n)	REG32(GPIO_PXTRG((n))) /* 0:level-trigger, 1:edge-trigger */
#define REG_GPIO_PXTRGS(n)	REG32(GPIO_PXTRGS((n)))
#define REG_GPIO_PXTRGC(n)	REG32(GPIO_PXTRGC((n)))
#define REG_GPIO_PXFLG(n)	REG32(GPIO_PXFLG((n))) /* interrupt flag */
#define REG_GPIO_PXFLGC(n)	REG32(GPIO_PXFLGC((n))) /* interrupt flag */


/*************************************************************************
 * UART
 *************************************************************************/

#define IRDA_BASE	UART0_BASE
#define UART_BASE	UART0_BASE
#define UART_OFF	0x1000

/* Register Offset */
#define OFF_RDR		(0x00)	/* R  8b H'xx */
#define OFF_TDR		(0x00)	/* W  8b H'xx */
#define OFF_DLLR	(0x00)	/* RW 8b H'00 */
#define OFF_DLHR	(0x04)	/* RW 8b H'00 */
#define OFF_IER		(0x04)	/* RW 8b H'00 */
#define OFF_ISR		(0x08)	/* R  8b H'01 */
#define OFF_FCR		(0x08)	/* W  8b H'00 */
#define OFF_LCR		(0x0C)	/* RW 8b H'00 */
#define OFF_MCR		(0x10)	/* RW 8b H'00 */
#define OFF_LSR		(0x14)	/* R  8b H'00 */
#define OFF_MSR		(0x18)	/* R  8b H'00 */
#define OFF_SPR		(0x1C)	/* RW 8b H'00 */
#define OFF_SIRCR	(0x20)	/* RW 8b H'00, UART0 */
#define OFF_UMR		(0x24)	/* RW 8b H'00, UART M Register */
#define OFF_UACR	(0x28)	/* RW 8b H'00, UART Add Cycle Register */

/* Register Address */
#define UART0_RDR	(UART0_BASE + OFF_RDR)
#define UART0_TDR	(UART0_BASE + OFF_TDR)
#define UART0_DLLR	(UART0_BASE + OFF_DLLR)
#define UART0_DLHR	(UART0_BASE + OFF_DLHR)
#define UART0_IER	(UART0_BASE + OFF_IER)
#define UART0_ISR	(UART0_BASE + OFF_ISR)
#define UART0_FCR	(UART0_BASE + OFF_FCR)
#define UART0_LCR	(UART0_BASE + OFF_LCR)
#define UART0_MCR	(UART0_BASE + OFF_MCR)
#define UART0_LSR	(UART0_BASE + OFF_LSR)
#define UART0_MSR	(UART0_BASE + OFF_MSR)
#define UART0_SPR	(UART0_BASE + OFF_SPR)
#define UART0_SIRCR	(UART0_BASE + OFF_SIRCR)
#define UART0_UMR	(UART0_BASE + OFF_UMR)
#define UART0_UACR	(UART0_BASE + OFF_UACR)

#define UART1_RDR	(UART1_BASE + OFF_RDR)
#define UART1_TDR	(UART1_BASE + OFF_TDR)
#define UART1_DLLR	(UART1_BASE + OFF_DLLR)
#define UART1_DLHR	(UART1_BASE + OFF_DLHR)
#define UART1_IER	(UART1_BASE + OFF_IER)
#define UART1_ISR	(UART1_BASE + OFF_ISR)
#define UART1_FCR	(UART1_BASE + OFF_FCR)
#define UART1_LCR	(UART1_BASE + OFF_LCR)
#define UART1_MCR	(UART1_BASE + OFF_MCR)
#define UART1_LSR	(UART1_BASE + OFF_LSR)
#define UART1_MSR	(UART1_BASE + OFF_MSR)
#define UART1_SPR	(UART1_BASE + OFF_SPR)
#define UART1_SIRCR	(UART1_BASE + OFF_SIRCR)

#define UART2_RDR	(UART2_BASE + OFF_RDR)
#define UART2_TDR	(UART2_BASE + OFF_TDR)
#define UART2_DLLR	(UART2_BASE + OFF_DLLR)
#define UART2_DLHR	(UART2_BASE + OFF_DLHR)
#define UART2_IER	(UART2_BASE + OFF_IER)
#define UART2_ISR	(UART2_BASE + OFF_ISR)
#define UART2_FCR	(UART2_BASE + OFF_FCR)
#define UART2_LCR	(UART2_BASE + OFF_LCR)
#define UART2_MCR	(UART2_BASE + OFF_MCR)
#define UART2_LSR	(UART2_BASE + OFF_LSR)
#define UART2_MSR	(UART2_BASE + OFF_MSR)
#define UART2_SPR	(UART2_BASE + OFF_SPR)
#define UART2_SIRCR	(UART2_BASE + OFF_SIRCR)

#define UART3_RDR	(UART3_BASE + OFF_RDR)
#define UART3_TDR	(UART3_BASE + OFF_TDR)
#define UART3_DLLR	(UART3_BASE + OFF_DLLR)
#define UART3_DLHR	(UART3_BASE + OFF_DLHR)
#define UART3_IER	(UART3_BASE + OFF_IER)
#define UART3_ISR	(UART3_BASE + OFF_ISR)
#define UART3_FCR	(UART3_BASE + OFF_FCR)
#define UART3_LCR	(UART3_BASE + OFF_LCR)
#define UART3_MCR	(UART3_BASE + OFF_MCR)
#define UART3_LSR	(UART3_BASE + OFF_LSR)
#define UART3_MSR	(UART3_BASE + OFF_MSR)
#define UART3_SPR	(UART3_BASE + OFF_SPR)
#define UART3_SIRCR	(UART3_BASE + OFF_SIRCR)


/*
 * Define macros for UARTIER
 * UART Interrupt Enable Register
 */
#define UARTIER_RIE	(1 << 0)	/* 0: receive fifo full interrupt disable */
#define UARTIER_TIE	(1 << 1)	/* 0: transmit fifo empty interrupt disable */
#define UARTIER_RLIE	(1 << 2)	/* 0: receive line status interrupt disable */
#define UARTIER_MIE	(1 << 3)	/* 0: modem status interrupt disable */
#define UARTIER_RTIE	(1 << 4)	/* 0: receive timeout interrupt disable */

/*
 * Define macros for UARTISR
 * UART Interrupt Status Register
 */
#define UARTISR_IP	(1 << 0)	/* 0: interrupt is pending  1: no interrupt */
#define UARTISR_IID	(7 << 1)	/* Source of Interrupt */
#define UARTISR_IID_MSI		(0 << 1)  /* Modem status interrupt */
#define UARTISR_IID_THRI	(1 << 1)  /* Transmitter holding register empty */
#define UARTISR_IID_RDI		(2 << 1)  /* Receiver data interrupt */
#define UARTISR_IID_RLSI	(3 << 1)  /* Receiver line status interrupt */
#define UARTISR_IID_RTO		(6 << 1)  /* Receive timeout */
#define UARTISR_FFMS		(3 << 6)  /* FIFO mode select, set when UARTFCR.FE is set to 1 */
#define UARTISR_FFMS_NO_FIFO	(0 << 6)
#define UARTISR_FFMS_FIFO_MODE	(3 << 6)

/*
 * Define macros for UARTFCR
 * UART FIFO Control Register
 */
#define UARTFCR_FE	(1 << 0)	/* 0: non-FIFO mode  1: FIFO mode */
#define UARTFCR_RFLS	(1 << 1)	/* write 1 to flush receive FIFO */
#define UARTFCR_TFLS	(1 << 2)	/* write 1 to flush transmit FIFO */
#define UARTFCR_DMS	(1 << 3)	/* 0: disable DMA mode */
#define UARTFCR_UUE	(1 << 4)	/* 0: disable UART */
#define UARTFCR_RTRG	(3 << 6)	/* Receive FIFO Data Trigger */
#define UARTFCR_RTRG_1	(0 << 6)
#define UARTFCR_RTRG_4	(1 << 6)
#define UARTFCR_RTRG_8	(2 << 6)
#define UARTFCR_RTRG_15	(3 << 6)

/*
 * Define macros for UARTLCR
 * UART Line Control Register
 */
#define UARTLCR_WLEN	(3 << 0)	/* word length */
#define UARTLCR_WLEN_5	(0 << 0)
#define UARTLCR_WLEN_6	(1 << 0)
#define UARTLCR_WLEN_7	(2 << 0)
#define UARTLCR_WLEN_8	(3 << 0)
#define UARTLCR_STOP	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
					   1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UARTLCR_STOP1	(0 << 2)
#define UARTLCR_STOP2	(1 << 2)
#define UARTLCR_PE	(1 << 3)	/* 0: parity disable */
#define UARTLCR_PROE	(1 << 4)	/* 0: even parity  1: odd parity */
#define UARTLCR_SPAR	(1 << 5)	/* 0: sticky parity disable */
#define UARTLCR_SBRK	(1 << 6)	/* write 0 normal, write 1 send break */
#define UARTLCR_DLAB	(1 << 7)	/* 0: access UARTRDR/TDR/IER  1: access UARTDLLR/DLHR */

/*
 * Define macros for UARTLSR
 * UART Line Status Register
 */
#define UARTLSR_DR	(1 << 0)	/* 0: receive FIFO is empty  1: receive data is ready */
#define UARTLSR_ORER	(1 << 1)	/* 0: no overrun error */
#define UARTLSR_PER	(1 << 2)	/* 0: no parity error */
#define UARTLSR_FER	(1 << 3)	/* 0; no framing error */
#define UARTLSR_BRK	(1 << 4)	/* 0: no break detected  1: receive a break signal */
#define UARTLSR_TDRQ	(1 << 5)	/* 1: transmit FIFO half "empty" */
#define UARTLSR_TEMT	(1 << 6)	/* 1: transmit FIFO and shift registers empty */
#define UARTLSR_RFER	(1 << 7)	/* 0: no receive error  1: receive error in FIFO mode */

/*
 * Define macros for UARTMCR
 * UART Modem Control Register
 */
#define UARTMCR_RTS	(1 << 1)	/* 0: RTS_ output high, 1: RTS_ output low */
#define UARTMCR_LOOP	(1 << 4)	/* 0: normal  1: loopback mode */
#define UARTMCR_MCE	(1 << 7)	/* 0: modem function is disable */

/*
 * Define macros for UARTMSR
 * UART Modem Status Register
 */
#define UARTMSR_CCTS	(1 << 0)        /* 1: a change on CTS_ pin */
#define UARTMSR_CTS	(1 << 4)	/* 0: CTS_ pin is high */

/*
 * Define macros for SIRCR
 * Slow IrDA Control Register
 */
#define SIRCR_TSIRE	(1 << 0)  /* 0: transmitter is in UART mode  1: SIR mode */
#define SIRCR_RSIRE	(1 << 1)  /* 0: receiver is in UART mode  1: SIR mode */
#define SIRCR_TPWS	(1 << 2)  /* 0: transmit 0 pulse width is 3/16 of bit length
					   1: 0 pulse width is 1.6us for 115.2Kbps */
#define SIRCR_TDPL	(1 << 3)  /* 0: encoder generates a positive pulse for 0 */
#define SIRCR_RDPL	(1 << 4)  /* 0: decoder interprets positive pulse as 0 */


/*************************************************************************
 * AIC (AC97/I2S Controller)
 *************************************************************************/
#define	AIC_FR			(AIC_BASE + 0x000)
#define	AIC_CR			(AIC_BASE + 0x004)
#define	AIC_ACCR1		(AIC_BASE + 0x008)
#define	AIC_ACCR2		(AIC_BASE + 0x00C)
#define	AIC_I2SCR		(AIC_BASE + 0x010)
#define	AIC_SR			(AIC_BASE + 0x014)
#define	AIC_ACSR		(AIC_BASE + 0x018)
#define	AIC_I2SSR		(AIC_BASE + 0x01C)
#define	AIC_ACCAR		(AIC_BASE + 0x020)
#define	AIC_ACCDR		(AIC_BASE + 0x024)
#define	AIC_ACSAR		(AIC_BASE + 0x028)
#define	AIC_ACSDR		(AIC_BASE + 0x02C)
#define	AIC_I2SDIV		(AIC_BASE + 0x030)
#define	AIC_DR			(AIC_BASE + 0x034)

#define	REG_AIC_FR		REG32(AIC_FR)
#define	REG_AIC_CR		REG32(AIC_CR)
#define	REG_AIC_ACCR1		REG32(AIC_ACCR1)
#define	REG_AIC_ACCR2		REG32(AIC_ACCR2)
#define	REG_AIC_I2SCR		REG32(AIC_I2SCR)
#define	REG_AIC_SR		REG32(AIC_SR)
#define	REG_AIC_ACSR		REG32(AIC_ACSR)
#define	REG_AIC_I2SSR		REG32(AIC_I2SSR)
#define	REG_AIC_ACCAR		REG32(AIC_ACCAR)
#define	REG_AIC_ACCDR		REG32(AIC_ACCDR)
#define	REG_AIC_ACSAR		REG32(AIC_ACSAR)
#define	REG_AIC_ACSDR		REG32(AIC_ACSDR)
#define	REG_AIC_I2SDIV		REG32(AIC_I2SDIV)
#define	REG_AIC_DR		REG32(AIC_DR)

/* AIC Controller Configuration Register (AIC_FR) */

#define	AIC_FR_RFTH_BIT		12        /* Receive FIFO Threshold */
#define	AIC_FR_RFTH_MASK	(0xf << AIC_FR_RFTH_BIT)
#define	AIC_FR_TFTH_BIT		8         /* Transmit FIFO Threshold */
#define	AIC_FR_TFTH_MASK	(0xf << AIC_FR_TFTH_BIT)
#define	AIC_FR_LSMP		(1 << 6)  /* Play Zero sample or last sample */
#define	AIC_FR_ICDC		(1 << 5)  /* External(0) or Internal CODEC(1) */
#define	AIC_FR_AUSEL		(1 << 4)  /* AC97(0) or I2S/MSB-justified(1) */
#define	AIC_FR_RST		(1 << 3)  /* AIC registers reset */
#define	AIC_FR_BCKD		(1 << 2)  /* I2S BIT_CLK direction, 0:input,1:output */
#define	AIC_FR_SYNCD		(1 << 1)  /* I2S SYNC direction, 0:input,1:output */
#define	AIC_FR_ENB		(1 << 0)  /* AIC enable bit */

/* AIC Controller Common Control Register (AIC_CR) */

#define	AIC_CR_OSS_BIT		19  /* Output Sample Size from memory (AIC V2 only) */
#define	AIC_CR_OSS_MASK		(0x7 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_8BIT	(0x0 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_16BIT	(0x1 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_18BIT	(0x2 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_20BIT	(0x3 << AIC_CR_OSS_BIT)
  #define AIC_CR_OSS_24BIT	(0x4 << AIC_CR_OSS_BIT)
#define	AIC_CR_ISS_BIT		16  /* Input Sample Size from memory (AIC V2 only) */
#define	AIC_CR_ISS_MASK		(0x7 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_8BIT	(0x0 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_16BIT	(0x1 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_18BIT	(0x2 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_20BIT	(0x3 << AIC_CR_ISS_BIT)
  #define AIC_CR_ISS_24BIT	(0x4 << AIC_CR_ISS_BIT)
#define	AIC_CR_RDMS		(1 << 15)  /* Receive DMA enable */
#define	AIC_CR_TDMS		(1 << 14)  /* Transmit DMA enable */
#define	AIC_CR_M2S		(1 << 11)  /* Mono to Stereo enable */
#define	AIC_CR_ENDSW		(1 << 10)  /* Endian switch enable */
#define	AIC_CR_AVSTSU		(1 << 9)   /* Signed <-> Unsigned toggle enable */
#define	AIC_CR_FLUSH		(1 << 8)   /* Flush FIFO */
#define	AIC_CR_EROR		(1 << 6)   /* Enable ROR interrupt */
#define	AIC_CR_ETUR		(1 << 5)   /* Enable TUR interrupt */
#define	AIC_CR_ERFS		(1 << 4)   /* Enable RFS interrupt */
#define	AIC_CR_ETFS		(1 << 3)   /* Enable TFS interrupt */
#define	AIC_CR_ENLBF		(1 << 2)   /* Enable Loopback Function */
#define	AIC_CR_ERPL		(1 << 1)   /* Enable Playback Function */
#define	AIC_CR_EREC		(1 << 0)   /* Enable Record Function */

/* AIC Controller AC-link Control Register 1 (AIC_ACCR1) */

#define	AIC_ACCR1_RS_BIT	16          /* Receive Valid Slots */
#define	AIC_ACCR1_RS_MASK	(0x3ff << AIC_ACCR1_RS_BIT)
  #define AIC_ACCR1_RS_SLOT12	  (1 << 25) /* Slot 12 valid bit */
  #define AIC_ACCR1_RS_SLOT11	  (1 << 24) /* Slot 11 valid bit */
  #define AIC_ACCR1_RS_SLOT10	  (1 << 23) /* Slot 10 valid bit */
  #define AIC_ACCR1_RS_SLOT9	  (1 << 22) /* Slot 9 valid bit, LFE */
  #define AIC_ACCR1_RS_SLOT8	  (1 << 21) /* Slot 8 valid bit, Surround Right */
  #define AIC_ACCR1_RS_SLOT7	  (1 << 20) /* Slot 7 valid bit, Surround Left */
  #define AIC_ACCR1_RS_SLOT6	  (1 << 19) /* Slot 6 valid bit, PCM Center */
  #define AIC_ACCR1_RS_SLOT5	  (1 << 18) /* Slot 5 valid bit */
  #define AIC_ACCR1_RS_SLOT4	  (1 << 17) /* Slot 4 valid bit, PCM Right */
  #define AIC_ACCR1_RS_SLOT3	  (1 << 16) /* Slot 3 valid bit, PCM Left */
#define	AIC_ACCR1_XS_BIT	0          /* Transmit Valid Slots */
#define	AIC_ACCR1_XS_MASK	(0x3ff << AIC_ACCR1_XS_BIT)
  #define AIC_ACCR1_XS_SLOT12	  (1 << 9) /* Slot 12 valid bit */
  #define AIC_ACCR1_XS_SLOT11	  (1 << 8) /* Slot 11 valid bit */
  #define AIC_ACCR1_XS_SLOT10	  (1 << 7) /* Slot 10 valid bit */
  #define AIC_ACCR1_XS_SLOT9	  (1 << 6) /* Slot 9 valid bit, LFE */
  #define AIC_ACCR1_XS_SLOT8	  (1 << 5) /* Slot 8 valid bit, Surround Right */
  #define AIC_ACCR1_XS_SLOT7	  (1 << 4) /* Slot 7 valid bit, Surround Left */
  #define AIC_ACCR1_XS_SLOT6	  (1 << 3) /* Slot 6 valid bit, PCM Center */
  #define AIC_ACCR1_XS_SLOT5	  (1 << 2) /* Slot 5 valid bit */
  #define AIC_ACCR1_XS_SLOT4	  (1 << 1) /* Slot 4 valid bit, PCM Right */
  #define AIC_ACCR1_XS_SLOT3	  (1 << 0) /* Slot 3 valid bit, PCM Left */

/* AIC Controller AC-link Control Register 2 (AIC_ACCR2) */

#define	AIC_ACCR2_ERSTO		(1 << 18) /* Enable RSTO interrupt */
#define	AIC_ACCR2_ESADR		(1 << 17) /* Enable SADR interrupt */
#define	AIC_ACCR2_ECADT		(1 << 16) /* Enable CADT interrupt */
#define	AIC_ACCR2_OASS_BIT	8  /* Output Sample Size for AC-link */
#define	AIC_ACCR2_OASS_MASK	(0x3 << AIC_ACCR2_OASS_BIT)
  #define AIC_ACCR2_OASS_20BIT	  (0 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 20-bit */
  #define AIC_ACCR2_OASS_18BIT	  (1 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 18-bit */
  #define AIC_ACCR2_OASS_16BIT	  (2 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 16-bit */
  #define AIC_ACCR2_OASS_8BIT	  (3 << AIC_ACCR2_OASS_BIT) /* Output Audio Sample Size is 8-bit */
#define	AIC_ACCR2_IASS_BIT	6  /* Output Sample Size for AC-link */
#define	AIC_ACCR2_IASS_MASK	(0x3 << AIC_ACCR2_IASS_BIT)
  #define AIC_ACCR2_IASS_20BIT	  (0 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 20-bit */
  #define AIC_ACCR2_IASS_18BIT	  (1 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 18-bit */
  #define AIC_ACCR2_IASS_16BIT	  (2 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 16-bit */
  #define AIC_ACCR2_IASS_8BIT	  (3 << AIC_ACCR2_IASS_BIT) /* Input Audio Sample Size is 8-bit */
#define	AIC_ACCR2_SO		(1 << 3)  /* SDATA_OUT output value */
#define	AIC_ACCR2_SR		(1 << 2)  /* RESET# pin level */
#define	AIC_ACCR2_SS		(1 << 1)  /* SYNC pin level */
#define	AIC_ACCR2_SA		(1 << 0)  /* SYNC and SDATA_OUT alternation */

/* AIC Controller I2S/MSB-justified Control Register (AIC_I2SCR) */

#define	AIC_I2SCR_STPBK		(1 << 12) /* Stop BIT_CLK for I2S/MSB-justified */
#define	AIC_I2SCR_WL_BIT	1  /* Input/Output Sample Size for I2S/MSB-justified */
#define	AIC_I2SCR_WL_MASK	(0x7 << AIC_I2SCR_WL_BIT)
  #define AIC_I2SCR_WL_24BIT	  (0 << AIC_I2SCR_WL_BIT) /* Word Length is 24 bit */
  #define AIC_I2SCR_WL_20BIT	  (1 << AIC_I2SCR_WL_BIT) /* Word Length is 20 bit */
  #define AIC_I2SCR_WL_18BIT	  (2 << AIC_I2SCR_WL_BIT) /* Word Length is 18 bit */
  #define AIC_I2SCR_WL_16BIT	  (3 << AIC_I2SCR_WL_BIT) /* Word Length is 16 bit */
  #define AIC_I2SCR_WL_8BIT	  (4 << AIC_I2SCR_WL_BIT) /* Word Length is 8 bit */
#define	AIC_I2SCR_AMSL		(1 << 0) /* 0:I2S, 1:MSB-justified */

/* AIC Controller FIFO Status Register (AIC_SR) */

#define	AIC_SR_RFL_BIT		24  /* Receive FIFO Level */
#define	AIC_SR_RFL_MASK		(0x3f << AIC_SR_RFL_BIT)
#define	AIC_SR_TFL_BIT		8   /* Transmit FIFO level */
#define	AIC_SR_TFL_MASK		(0x3f << AIC_SR_TFL_BIT)
#define	AIC_SR_ROR		(1 << 6) /* Receive FIFO Overrun */
#define	AIC_SR_TUR		(1 << 5) /* Transmit FIFO Underrun */
#define	AIC_SR_RFS		(1 << 4) /* Receive FIFO Service Request */
#define	AIC_SR_TFS		(1 << 3) /* Transmit FIFO Service Request */

/* AIC Controller AC-link Status Register (AIC_ACSR) */

#define	AIC_ACSR_SLTERR		(1 << 21) /* Slot Error Flag */
#define	AIC_ACSR_CRDY		(1 << 20) /* External CODEC Ready Flag */
#define	AIC_ACSR_CLPM		(1 << 19) /* External CODEC low power mode flag */
#define	AIC_ACSR_RSTO		(1 << 18) /* External CODEC regs read status timeout */
#define	AIC_ACSR_SADR		(1 << 17) /* External CODEC regs status addr and data received */
#define	AIC_ACSR_CADT		(1 << 16) /* Command Address and Data Transmitted */

/* AIC Controller I2S/MSB-justified Status Register (AIC_I2SSR) */

#define	AIC_I2SSR_BSY		(1 << 2)  /* AIC Busy in I2S/MSB-justified format */

/* AIC Controller AC97 codec Command Address Register (AIC_ACCAR) */

#define	AIC_ACCAR_CAR_BIT	0
#define	AIC_ACCAR_CAR_MASK	(0xfffff << AIC_ACCAR_CAR_BIT)

/* AIC Controller AC97 codec Command Data Register (AIC_ACCDR) */

#define	AIC_ACCDR_CDR_BIT	0
#define	AIC_ACCDR_CDR_MASK	(0xfffff << AIC_ACCDR_CDR_BIT)

/* AIC Controller AC97 codec Status Address Register (AIC_ACSAR) */

#define	AIC_ACSAR_SAR_BIT	0
#define	AIC_ACSAR_SAR_MASK	(0xfffff << AIC_ACSAR_SAR_BIT)

/* AIC Controller AC97 codec Status Data Register (AIC_ACSDR) */

#define	AIC_ACSDR_SDR_BIT	0
#define	AIC_ACSDR_SDR_MASK	(0xfffff << AIC_ACSDR_SDR_BIT)

/* AIC Controller I2S/MSB-justified Clock Divider Register (AIC_I2SDIV) */

#define	AIC_I2SDIV_DIV_BIT	0
#define	AIC_I2SDIV_DIV_MASK	(0x7f << AIC_I2SDIV_DIV_BIT)
  #define AIC_I2SDIV_BITCLK_3072KHZ	(0x0C << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 3.072MHz */
  #define AIC_I2SDIV_BITCLK_2836KHZ	(0x0D << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 2.836MHz */
  #define AIC_I2SDIV_BITCLK_1418KHZ	(0x1A << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 1.418MHz */
  #define AIC_I2SDIV_BITCLK_1024KHZ	(0x24 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 1.024MHz */
  #define AIC_I2SDIV_BITCLK_7089KHZ	(0x34 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 708.92KHz */
  #define AIC_I2SDIV_BITCLK_512KHZ	(0x48 << AIC_I2SDIV_DIV_BIT) /* BIT_CLK of 512.00KHz */


/*************************************************************************
 * ICDC (Internal CODEC)
 *************************************************************************/
#define	ICDC_CR			(ICDC_BASE + 0x0400)  /* ICDC Control Register */
#define	ICDC_APWAIT		(ICDC_BASE + 0x0404)  /* Anti-Pop WAIT Stage Timing Control Register */
#define	ICDC_APPRE		(ICDC_BASE + 0x0408)  /* Anti-Pop HPEN-PRE Stage Timing Control Register */
#define	ICDC_APHPEN		(ICDC_BASE + 0x040C)  /* Anti-Pop HPEN Stage Timing Control Register */
#define	ICDC_APSR		(ICDC_BASE + 0x0410)  /* Anti-Pop Status Register */
#define ICDC_CDCCR1             (ICDC_BASE + 0x0080)
#define ICDC_CDCCR2             (ICDC_BASE + 0x0084)

#define	REG_ICDC_CR		REG32(ICDC_CR)
#define	REG_ICDC_APWAIT		REG32(ICDC_APWAIT)
#define	REG_ICDC_APPRE		REG32(ICDC_APPRE)
#define	REG_ICDC_APHPEN		REG32(ICDC_APHPEN)
#define	REG_ICDC_APSR		REG32(ICDC_APSR)
#define REG_ICDC_CDCCR1         REG32(ICDC_CDCCR1)
#define REG_ICDC_CDCCR2         REG32(ICDC_CDCCR2)

/* ICDC Control Register */
#define	ICDC_CR_LINVOL_BIT	24 /* LINE Input Volume Gain: GAIN=LINVOL*1.5-34.5 */
#define	ICDC_CR_LINVOL_MASK	(0x1f << ICDC_CR_LINVOL_BIT)
#define	ICDC_CR_ASRATE_BIT	20 /* Audio Sample Rate */
#define	ICDC_CR_ASRATE_MASK	(0x0f << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_8000	(0x0 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_11025	(0x1 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_12000	(0x2 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_16000	(0x3 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_22050	(0x4 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_24000	(0x5 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_32000	(0x6 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_44100	(0x7 << ICDC_CR_ASRATE_BIT)
  #define ICDC_CR_ASRATE_48000	(0x8 << ICDC_CR_ASRATE_BIT)
#define	ICDC_CR_MICBG_BIT	18 /* MIC Boost Gain */
#define	ICDC_CR_MICBG_MASK	(0x3 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_0DB	(0x0 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_6DB	(0x1 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_12DB	(0x2 << ICDC_CR_MICBG_BIT)
  #define ICDC_CR_MICBG_20DB	(0x3 << ICDC_CR_MICBG_BIT)
#define	ICDC_CR_HPVOL_BIT	16 /* Headphone Volume Gain */
#define	ICDC_CR_HPVOL_MASK	(0x3 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_0DB	(0x0 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_2DB	(0x1 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_4DB	(0x2 << ICDC_CR_HPVOL_BIT)
  #define ICDC_CR_HPVOL_6DB	(0x3 << ICDC_CR_HPVOL_BIT)
#define ICDC_CR_ELINEIN		(1 << 13) /* Enable LINE Input */
#define ICDC_CR_EMIC		(1 << 12) /* Enable MIC Input */
#define ICDC_CR_SW1ON		(1 << 11) /* Switch 1 in CODEC is on */
#define ICDC_CR_EADC		(1 << 10) /* Enable ADC */
#define ICDC_CR_SW2ON		(1 << 9)  /* Switch 2 in CODEC is on */
#define ICDC_CR_EDAC		(1 << 8)  /* Enable DAC */
#define ICDC_CR_HPMUTE		(1 << 5)  /* Headphone Mute */
#define ICDC_CR_HPTON		(1 << 4)  /* Headphone Amplifier Trun On */
#define ICDC_CR_HPTOFF		(1 << 3)  /* Headphone Amplifier Trun Off */
#define ICDC_CR_TAAP		(1 << 2)  /* Turn Around of the Anti-Pop Procedure */
#define ICDC_CR_EAP		(1 << 1)  /* Enable Anti-Pop Procedure */
#define ICDC_CR_SUSPD		(1 << 0)  /* CODEC Suspend */

/* Anti-Pop WAIT Stage Timing Control Register */
#define	ICDC_APWAIT_WAITSN_BIT	0
#define	ICDC_APWAIT_WAITSN_MASK	(0x7ff << ICDC_APWAIT_WAITSN_BIT)

/* Anti-Pop HPEN-PRE Stage Timing Control Register */
#define	ICDC_APPRE_PRESN_BIT	0
#define	ICDC_APPRE_PRESN_MASK	(0x1ff << ICDC_APPRE_PRESN_BIT)

/* Anti-Pop HPEN Stage Timing Control Register */
#define	ICDC_APHPEN_HPENSN_BIT	0
#define	ICDC_APHPEN_HPENSN_MASK	(0x3fff << ICDC_APHPEN_HPENSN_BIT)

/* Anti-Pop Status Register */
#define	ICDC_SR_HPST_BIT	14  /* Headphone Amplifier State */
#define	ICDC_SR_HPST_MASK	(0x7 << ICDC_SR_HPST_BIT)
#define ICDC_SR_HPST_HP_OFF	 (0x0 << ICDC_SR_HPST_BIT) /* HP amplifier is off */
#define ICDC_SR_HPST_TON_WAIT	 (0x1 << ICDC_SR_HPST_BIT) /* wait state in turn-on */
  #define ICDC_SR_HPST_TON_PRE	 (0x2 << ICDC_SR_HPST_BIT) /* pre-enable state in turn-on */
#define ICDC_SR_HPST_TON_HPEN	 (0x3 << ICDC_SR_HPST_BIT) /* HP enable state in turn-on */
  #define ICDC_SR_HPST_TOFF_HPEN (0x4 << ICDC_SR_HPST_BIT) /* HP enable state in turn-off */
  #define ICDC_SR_HPST_TOFF_PRE  (0x5 << ICDC_SR_HPST_BIT) /* pre-enable state in turn-off */
  #define ICDC_SR_HPST_TOFF_WAIT (0x6 << ICDC_SR_HPST_BIT) /* wait state in turn-off */
  #define ICDC_SR_HPST_HP_ON	 (0x7 << ICDC_SR_HPST_BIT) /* HP amplifier is on */
#define	ICDC_SR_SNCNT_BIT	0  /* Sample Number Counter */
#define	ICDC_SR_SNCNT_MASK	(0x3fff << ICDC_SR_SNCNT_BIT)

/*************************************************************************
 * PCM Controller
 *************************************************************************/

#define PCM_CTL                 (PCM_BASE + 0x000)
#define PCM_CFG                 (PCM_BASE + 0x004)
#define PCM_DP                  (PCM_BASE + 0x008)
#define PCM_INTC                (PCM_BASE + 0x00c)
#define PCM_INTS                (PCM_BASE + 0x010)
#define PCM_DIV                 (PCM_BASE + 0x014)

#define REG_PCM_CTL             REG32(PCM_CTL)
#define REG_PCM_CFG             REG32(PCM_CFG)
#define REG_PCM_DP              REG32(PCM_DP)
#define REG_PCM_INTC            REG32(PCM_INTC)
#define REG_PCM_INTS            REG32(PCM_INTS)
#define REG_PCM_DIV             REG32(PCM_DIV)

/* PCM Controller control Register (PCM_CTL) */

#define PCM_CTL_ERDMA		(1 << 9)  /* Enable Receive DMA */
#define PCM_CTL_ETDMA           (1 << 8)  /* Enable Transmit DMA */
#define PCM_CTL_LSMP		(1 << 7)  /* Play Zero sample or last sample */
#define PCM_CTL_TFEN            (1 << 6)  /* Enable the RX-FIFO */
#define PCM_CTL_RFEN            (1 << 5)  /* Enable the TX-FIFO */
#define PCM_CTL_FLUSH           (1 << 4)  /* FIFO flush */
#define PCM_CTL_RST             (1 << 3)
//#define PCM_CTL_SYNEN           (1 << 2)  /* Enable the sync division logic */
#define PCM_CTL_CLKEN           (1 << 1)  /* Enable the clock division logic */
#define PCM_CTL_PCMEN           (1 << 0)  /* Enable PCM module */

/* PCM Controller configure Register (PCM_CFG) */

#define PCM_CFG_SLOT_BIT        13
#define PCM_CFG_SLOT_MASK       (0x3 << PCM_CFG_SLOT_BIT)
  #define PCM_CFG_SLOT_0	  (0 << PCM_CFG_SLOT_BIT) /* Slot is 0 */
  #define PCM_CFG_SLOT_1	  (1 << PCM_CFG_SLOT_BIT) /* Slot is 1 */
  #define PCM_CFG_SLOT_2	  (2 << PCM_CFG_SLOT_BIT) /* Slot is 2 */
  #define PCM_CFG_SLOT_3	  (3 << PCM_CFG_SLOT_BIT) /* Slot is 3 */
#define PCM_CFG_ISS_BIT         12
#define PCM_CFG_ISS_MASK        (0x1 << PCM_CFG_ISS_BIT)
  #define PCM_CFG_ISS_8           (0 << PCM_CFG_ISS_BIT)
  #define PCM_CFG_ISS_16          (1 << PCM_CFG_ISS_BIT)
#define PCM_CFG_OSS_BIT         11
#define PCM_CFG_OSS_MASK        (0x1 << PCM_CFG_OSS_BIT)
  #define PCM_CFG_OSS_8           (0 << PCM_CFG_OSS_BIT)
  #define PCM_CFG_OSS_16          (1 << PCM_CFG_OSS_BIT)
#define PCM_CFG_IMSBPOS         (1 << 10)
#define PCM_CFG_OMSBPOS         (1 << 9)
#define	PCM_CFG_RFTH_BIT	5        /* Receive FIFO Threshold */
#define	PCM_CFG_RFTH_MASK	(0xf << PCM_CFG_RFTH_BIT)
#define	PCM_CFG_TFTH_BIT	1         /* Transmit FIFO Threshold */
#define	PCM_CFG_TFTH_MASK	(0xf << PCM_CFG_TFTH_BIT)
#define PCM_CFG_MODE            (0x0 << 0)

/* PCM Controller interrupt control Register (PCM_INTC) */

#define PCM_INTC_ETFS           (1 << 3)
#define PCM_INTC_ETUR           (1 << 2)
#define PCM_INTC_ERFS           (1 << 1)
#define PCM_INTC_EROR           (1 << 0)

/* PCM Controller interrupt status Register (PCM_INTS) */

#define PCM_INTS_TFL_BIT        9
#define PCM_INTS_TFL_MASK       (0x1f << PCM_INTS_TFL_BIT)
#define PCM_INTS_RFL_BIT        2
#define PCM_INTS_RFL_MASK       (0x1f << PCM_INTS_RFL_BIT)

/* PCM Controller clock division Register (PCM_DIV) */
#define PCM_DIV_SYNL_BIT        11
#define PCM_DIV_SYNL_MASK       (0x3f << PCM_DIV_SYNL_BIT)
#define PCM_DIV_SYNDIV_BIT      6
#define PCM_DIV_SYNDIV_MASK     (0x1f << PCM_DIV_SYNDIV_BIT)
#define PCM_DIV_CLKDIV_BIT      0
#define PCM_DIV_CLKDIV_MASK     (0x3f << PCM_DIV_CLKDIV_BIT)


/*************************************************************************
 * I2C
 *************************************************************************/
#define	I2C_DR			(I2C_BASE + 0x000)
#define	I2C_CR			(I2C_BASE + 0x004)
#define	I2C_SR			(I2C_BASE + 0x008)
#define	I2C_GR			(I2C_BASE + 0x00C)

#define	REG_I2C_DR		REG8(I2C_DR)
#define	REG_I2C_CR		REG8(I2C_CR)
#define REG_I2C_SR		REG8(I2C_SR)
#define REG_I2C_GR		REG16(I2C_GR)

/* I2C Control Register (I2C_CR) */

#define I2C_CR_IEN		(1 << 4)
#define I2C_CR_STA		(1 << 3)
#define I2C_CR_STO		(1 << 2)
#define I2C_CR_AC		(1 << 1)
#define I2C_CR_I2CE		(1 << 0)

/* I2C Status Register (I2C_SR) */

#define I2C_SR_STX		(1 << 4)
#define I2C_SR_BUSY		(1 << 3)
#define I2C_SR_TEND		(1 << 2)
#define I2C_SR_DRF		(1 << 1)
#define I2C_SR_ACKF		(1 << 0)


/*************************************************************************
 * SSI (Synchronous Serial Interface)
 *************************************************************************/
/* n = 0, 1 (SSI0, SSI1) */
#define	SSI_DR(n)		(SSI_BASE + 0x000 + (n)*0x2000)
#define	SSI_CR0(n)		(SSI_BASE + 0x004 + (n)*0x2000)
#define	SSI_CR1(n)		(SSI_BASE + 0x008 + (n)*0x2000)
#define	SSI_SR(n)		(SSI_BASE + 0x00C + (n)*0x2000)
#define	SSI_ITR(n)		(SSI_BASE + 0x010 + (n)*0x2000)
#define	SSI_ICR(n)		(SSI_BASE + 0x014 + (n)*0x2000)
#define	SSI_GR(n)		(SSI_BASE + 0x018 + (n)*0x2000)

#define	REG_SSI_DR(n)		REG32(SSI_DR(n))
#define	REG_SSI_CR0(n)		REG16(SSI_CR0(n))
#define	REG_SSI_CR1(n)		REG32(SSI_CR1(n))
#define	REG_SSI_SR(n)		REG32(SSI_SR(n))
#define	REG_SSI_ITR(n)		REG16(SSI_ITR(n))
#define	REG_SSI_ICR(n)		REG8(SSI_ICR(n))
#define	REG_SSI_GR(n)		REG16(SSI_GR(n))

/* SSI Data Register (SSI_DR) */

#define	SSI_DR_GPC_BIT		0
#define	SSI_DR_GPC_MASK		(0x1ff << SSI_DR_GPC_BIT)

#define SSI_MAX_FIFO_ENTRIES 	128 /* 128 txfifo and 128 rxfifo */

/* SSI Control Register 0 (SSI_CR0) */

#define SSI_CR0_SSIE		(1 << 15)
#define SSI_CR0_TIE		(1 << 14)
#define SSI_CR0_RIE		(1 << 13)
#define SSI_CR0_TEIE		(1 << 12)
#define SSI_CR0_REIE		(1 << 11)
#define SSI_CR0_LOOP		(1 << 10)
#define SSI_CR0_RFINE		(1 << 9)
#define SSI_CR0_RFINC		(1 << 8)
#define SSI_CR0_EACLRUN		(1 << 7) /* hardware auto clear underrun when TxFifo no empty */
#define SSI_CR0_FSEL		(1 << 6)
#define SSI_CR0_TFLUSH		(1 << 2)
#define SSI_CR0_RFLUSH		(1 << 1)
#define SSI_CR0_DISREV		(1 << 0)

/* SSI Control Register 1 (SSI_CR1) */

#define SSI_CR1_FRMHL_BIT	30
#define SSI_CR1_FRMHL_MASK	(0x3 << SSI_CR1_FRMHL_BIT)
  #define SSI_CR1_FRMHL_CELOW_CE2LOW	(0 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is low valid and SSI_CE2_ is low valid */
  #define SSI_CR1_FRMHL_CEHIGH_CE2LOW	(1 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is high valid and SSI_CE2_ is low valid */
  #define SSI_CR1_FRMHL_CELOW_CE2HIGH	(2 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is low valid  and SSI_CE2_ is high valid */
  #define SSI_CR1_FRMHL_CEHIGH_CE2HIGH	(3 << SSI_CR1_FRMHL_BIT) /* SSI_CE_ is high valid and SSI_CE2_ is high valid */
#define SSI_CR1_TFVCK_BIT	28
#define SSI_CR1_TFVCK_MASK	(0x3 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_0	  (0 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_1	  (1 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_2	  (2 << SSI_CR1_TFVCK_BIT)
  #define SSI_CR1_TFVCK_3	  (3 << SSI_CR1_TFVCK_BIT)
#define SSI_CR1_TCKFI_BIT	26
#define SSI_CR1_TCKFI_MASK	(0x3 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_0	  (0 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_1	  (1 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_2	  (2 << SSI_CR1_TCKFI_BIT)
  #define SSI_CR1_TCKFI_3	  (3 << SSI_CR1_TCKFI_BIT)
#define SSI_CR1_LFST		(1 << 25)
#define SSI_CR1_ITFRM		(1 << 24)
#define SSI_CR1_UNFIN		(1 << 23)
#define SSI_CR1_MULTS		(1 << 22)
#define SSI_CR1_FMAT_BIT	20
#define SSI_CR1_FMAT_MASK	(0x3 << SSI_CR1_FMAT_BIT)
  #define SSI_CR1_FMAT_SPI	  (0 << SSI_CR1_FMAT_BIT) /* Motorola¡¯s SPI format */
  #define SSI_CR1_FMAT_SSP	  (1 << SSI_CR1_FMAT_BIT) /* TI's SSP format */
  #define SSI_CR1_FMAT_MW1	  (2 << SSI_CR1_FMAT_BIT) /* National Microwire 1 format */
  #define SSI_CR1_FMAT_MW2	  (3 << SSI_CR1_FMAT_BIT) /* National Microwire 2 format */
#define SSI_CR1_TTRG_BIT	16 /* SSI1 TX trigger */
#define SSI_CR1_TTRG_MASK	(0xf << SSI1_CR1_TTRG_BIT) 
#define SSI_CR1_MCOM_BIT	12
#define SSI_CR1_MCOM_MASK	(0xf << SSI_CR1_MCOM_BIT)
  #define SSI_CR1_MCOM_1BIT	  (0x0 << SSI_CR1_MCOM_BIT) /* 1-bit command selected */
  #define SSI_CR1_MCOM_2BIT	  (0x1 << SSI_CR1_MCOM_BIT) /* 2-bit command selected */
  #define SSI_CR1_MCOM_3BIT	  (0x2 << SSI_CR1_MCOM_BIT) /* 3-bit command selected */
  #define SSI_CR1_MCOM_4BIT	  (0x3 << SSI_CR1_MCOM_BIT) /* 4-bit command selected */
  #define SSI_CR1_MCOM_5BIT	  (0x4 << SSI_CR1_MCOM_BIT) /* 5-bit command selected */
  #define SSI_CR1_MCOM_6BIT	  (0x5 << SSI_CR1_MCOM_BIT) /* 6-bit command selected */
  #define SSI_CR1_MCOM_7BIT	  (0x6 << SSI_CR1_MCOM_BIT) /* 7-bit command selected */
  #define SSI_CR1_MCOM_8BIT	  (0x7 << SSI_CR1_MCOM_BIT) /* 8-bit command selected */
  #define SSI_CR1_MCOM_9BIT	  (0x8 << SSI_CR1_MCOM_BIT) /* 9-bit command selected */
  #define SSI_CR1_MCOM_10BIT	  (0x9 << SSI_CR1_MCOM_BIT) /* 10-bit command selected */
  #define SSI_CR1_MCOM_11BIT	  (0xA << SSI_CR1_MCOM_BIT) /* 11-bit command selected */
  #define SSI_CR1_MCOM_12BIT	  (0xB << SSI_CR1_MCOM_BIT) /* 12-bit command selected */
  #define SSI_CR1_MCOM_13BIT	  (0xC << SSI_CR1_MCOM_BIT) /* 13-bit command selected */
  #define SSI_CR1_MCOM_14BIT	  (0xD << SSI_CR1_MCOM_BIT) /* 14-bit command selected */
  #define SSI_CR1_MCOM_15BIT	  (0xE << SSI_CR1_MCOM_BIT) /* 15-bit command selected */
  #define SSI_CR1_MCOM_16BIT	  (0xF << SSI_CR1_MCOM_BIT) /* 16-bit command selected */
#define SSI_CR1_RTRG_BIT	8 /* SSI RX trigger */
#define SSI_CR1_RTRG_MASK	(0xf << SSI1_CR1_RTRG_BIT)
#define SSI_CR1_FLEN_BIT	4
#define SSI_CR1_FLEN_MASK	(0xf << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_2BIT	  (0x0 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_3BIT	  (0x1 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_4BIT	  (0x2 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_5BIT	  (0x3 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_6BIT	  (0x4 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_7BIT	  (0x5 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_8BIT	  (0x6 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_9BIT	  (0x7 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_10BIT	  (0x8 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_11BIT	  (0x9 << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_12BIT	  (0xA << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_13BIT	  (0xB << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_14BIT	  (0xC << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_15BIT	  (0xD << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_16BIT	  (0xE << SSI_CR1_FLEN_BIT)
  #define SSI_CR1_FLEN_17BIT	  (0xF << SSI_CR1_FLEN_BIT)
#define SSI_CR1_PHA		(1 << 1)
#define SSI_CR1_POL		(1 << 0)

/* SSI Status Register (SSI_SR) */

#define SSI_SR_TFIFONUM_BIT	16
#define SSI_SR_TFIFONUM_MASK	(0xff << SSI_SR_TFIFONUM_BIT)
#define SSI_SR_RFIFONUM_BIT	8
#define SSI_SR_RFIFONUM_MASK	(0xff << SSI_SR_RFIFONUM_BIT)
#define SSI_SR_END		(1 << 7)
#define SSI_SR_BUSY		(1 << 6)
#define SSI_SR_TFF		(1 << 5)
#define SSI_SR_RFE		(1 << 4)
#define SSI_SR_TFHE		(1 << 3)
#define SSI_SR_RFHF		(1 << 2)
#define SSI_SR_UNDR		(1 << 1)
#define SSI_SR_OVER		(1 << 0)

/* SSI Interval Time Control Register (SSI_ITR) */

#define	SSI_ITR_CNTCLK		(1 << 15)
#define SSI_ITR_IVLTM_BIT	0
#define SSI_ITR_IVLTM_MASK	(0x7fff << SSI_ITR_IVLTM_BIT)


/*************************************************************************
 * MSC
 *************************************************************************/
#define	MSC_STRPCL		(MSC_BASE + 0x000)
#define	MSC_STAT		(MSC_BASE + 0x004)
#define	MSC_CLKRT		(MSC_BASE + 0x008)
#define	MSC_CMDAT		(MSC_BASE + 0x00C)
#define	MSC_RESTO		(MSC_BASE + 0x010)
#define	MSC_RDTO		(MSC_BASE + 0x014)
#define	MSC_BLKLEN		(MSC_BASE + 0x018)
#define	MSC_NOB			(MSC_BASE + 0x01C)
#define	MSC_SNOB		(MSC_BASE + 0x020)
#define	MSC_IMASK		(MSC_BASE + 0x024)
#define	MSC_IREG		(MSC_BASE + 0x028)
#define	MSC_CMD			(MSC_BASE + 0x02C)
#define	MSC_ARG			(MSC_BASE + 0x030)
#define	MSC_RES			(MSC_BASE + 0x034)
#define	MSC_RXFIFO		(MSC_BASE + 0x038)
#define	MSC_TXFIFO		(MSC_BASE + 0x03C)

#define	REG_MSC_STRPCL		REG16(MSC_STRPCL)
#define	REG_MSC_STAT		REG32(MSC_STAT)
#define	REG_MSC_CLKRT		REG16(MSC_CLKRT)
#define	REG_MSC_CMDAT		REG32(MSC_CMDAT)
#define	REG_MSC_RESTO		REG16(MSC_RESTO)
#define	REG_MSC_RDTO		REG16(MSC_RDTO)
#define	REG_MSC_BLKLEN		REG16(MSC_BLKLEN)
#define	REG_MSC_NOB		REG16(MSC_NOB)
#define	REG_MSC_SNOB		REG16(MSC_SNOB)
#define	REG_MSC_IMASK		REG16(MSC_IMASK)
#define	REG_MSC_IREG		REG16(MSC_IREG)
#define	REG_MSC_CMD		REG8(MSC_CMD)
#define	REG_MSC_ARG		REG32(MSC_ARG)
#define	REG_MSC_RES		REG16(MSC_RES)
#define	REG_MSC_RXFIFO		REG32(MSC_RXFIFO)
#define	REG_MSC_TXFIFO		REG32(MSC_TXFIFO)

/* MSC Clock and Control Register (MSC_STRPCL) */

#define MSC_STRPCL_SEND_CCSD		(1 << 15) /*send command completion signal disable to ceata */
#define MSC_STRPCL_SEND_AS_CCSD		(1 << 14) /*send internally generated stop after sending ccsd */
#define MSC_STRPCL_EXIT_MULTIPLE	(1 << 7)
#define MSC_STRPCL_EXIT_TRANSFER	(1 << 6)
#define MSC_STRPCL_START_READWAIT	(1 << 5)
#define MSC_STRPCL_STOP_READWAIT	(1 << 4)
#define MSC_STRPCL_RESET		(1 << 3)
#define MSC_STRPCL_START_OP		(1 << 2)
#define MSC_STRPCL_CLOCK_CONTROL_BIT	0
#define MSC_STRPCL_CLOCK_CONTROL_MASK	(0x3 << MSC_STRPCL_CLOCK_CONTROL_BIT)
  #define MSC_STRPCL_CLOCK_CONTROL_STOP	  (0x1 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Stop MMC/SD clock */
  #define MSC_STRPCL_CLOCK_CONTROL_START  (0x2 << MSC_STRPCL_CLOCK_CONTROL_BIT) /* Start MMC/SD clock */

/* MSC Status Register (MSC_STAT) */
#define MSC_STAT_AUTO_CMD_DONE		(1 << 31) /*12 is internally generated by controller has finished */
#define MSC_STAT_IS_RESETTING		(1 << 15)
#define MSC_STAT_SDIO_INT_ACTIVE	(1 << 14)
#define MSC_STAT_PRG_DONE		(1 << 13)
#define MSC_STAT_DATA_TRAN_DONE		(1 << 12)
#define MSC_STAT_END_CMD_RES		(1 << 11)
#define MSC_STAT_DATA_FIFO_AFULL	(1 << 10)
#define MSC_STAT_IS_READWAIT		(1 << 9)
#define MSC_STAT_CLK_EN			(1 << 8)
#define MSC_STAT_DATA_FIFO_FULL		(1 << 7)
#define MSC_STAT_DATA_FIFO_EMPTY	(1 << 6)
#define MSC_STAT_CRC_RES_ERR		(1 << 5)
#define MSC_STAT_CRC_READ_ERROR		(1 << 4)
#define MSC_STAT_CRC_WRITE_ERROR_BIT	2
#define MSC_STAT_CRC_WRITE_ERROR_MASK	(0x3 << MSC_STAT_CRC_WRITE_ERROR_BIT)
  #define MSC_STAT_CRC_WRITE_ERROR_NO		(0 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No error on transmission of data */
  #define MSC_STAT_CRC_WRITE_ERROR		(1 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* Card observed erroneous transmission of data */
  #define MSC_STAT_CRC_WRITE_ERROR_NOSTS	(2 << MSC_STAT_CRC_WRITE_ERROR_BIT) /* No CRC status is sent back */
#define MSC_STAT_TIME_OUT_RES		(1 << 1)
#define MSC_STAT_TIME_OUT_READ		(1 << 0)

/* MSC Bus Clock Control Register (MSC_CLKRT) */

#define	MSC_CLKRT_CLK_RATE_BIT		0
#define	MSC_CLKRT_CLK_RATE_MASK		(0x7 << MSC_CLKRT_CLK_RATE_BIT)
  #define MSC_CLKRT_CLK_RATE_DIV_1	(0x0 << MSC_CLKRT_CLK_RATE_BIT) /* CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_2	(0x1 << MSC_CLKRT_CLK_RATE_BIT) /* 1/2 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_4	(0x2 << MSC_CLKRT_CLK_RATE_BIT) /* 1/4 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_8	(0x3 << MSC_CLKRT_CLK_RATE_BIT) /* 1/8 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_16	(0x4 << MSC_CLKRT_CLK_RATE_BIT) /* 1/16 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_32	(0x5 << MSC_CLKRT_CLK_RATE_BIT) /* 1/32 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_64	(0x6 << MSC_CLKRT_CLK_RATE_BIT) /* 1/64 of CLK_SRC */
  #define MSC_CLKRT_CLK_RATE_DIV_128	(0x7 << MSC_CLKRT_CLK_RATE_BIT) /* 1/128 of CLK_SRC */

/* MSC Command Sequence Control Register (MSC_CMDAT) */

#define	MSC_CMDAT_CCS_EXPECTED		(1 << 31) /* interrupts are enabled in ce-ata */
#define	MSC_CMDAT_READ_CEATA		(1 << 30)
#define	MSC_CMDAT_SDIO_PRDT		(1 << 17) /* exact 2 cycle */
#define	MSC_CMDAT_SEND_AS_STOP		(1 << 16)
#define	MSC_CMDAT_RTRG_BIT		14
  #define MSC_CMDAT_RTRG_EQUALT_8	(0x0 << MSC_CMDAT_RTRG_BIT)
  #define MSC_CMDAT_RTRG_EQUALT_16	(0x1 << MSC_CMDAT_RTRG_BIT) /* reset value */
  #define MSC_CMDAT_RTRG_EQUALT_24	(0x2 << MSC_CMDAT_RTRG_BIT)

#define	MSC_CMDAT_TTRG_BIT		12
  #define MSC_CMDAT_TTRG_LESS_8		(0x0 << MSC_CMDAT_TTRG_BIT)
  #define MSC_CMDAT_TTRG_LESS_16	(0x1 << MSC_CMDAT_TTRG_BIT) /*reset value  */
  #define MSC_CMDAT_TTRG_LESS_24	(0x2 << MSC_CMDAT_TTRG_BIT)
#define	MSC_CMDAT_STOP_ABORT		(1 << 11)
#define	MSC_CMDAT_BUS_WIDTH_BIT		9
#define	MSC_CMDAT_BUS_WIDTH_MASK	(0x3 << MSC_CMDAT_BUS_WIDTH_BIT)
  #define MSC_CMDAT_BUS_WIDTH_1BIT	(0x0 << MSC_CMDAT_BUS_WIDTH_BIT) /* 1-bit data bus */
  #define MSC_CMDAT_BUS_WIDTH_4BIT	(0x2 << MSC_CMDAT_BUS_WIDTH_BIT) /* 4-bit data bus */
  #define CMDAT_BUS_WIDTH1		(0x0 << MSC_CMDAT_BUS_WIDTH_BIT)
  #define CMDAT_BUS_WIDTH4		(0x2 << MSC_CMDAT_BUS_WIDTH_BIT)
#define	MSC_CMDAT_DMA_EN		(1 << 8)
#define	MSC_CMDAT_INIT			(1 << 7)
#define	MSC_CMDAT_BUSY			(1 << 6)
#define	MSC_CMDAT_STREAM_BLOCK		(1 << 5)
#define	MSC_CMDAT_WRITE			(1 << 4)
#define	MSC_CMDAT_READ			(0 << 4)
#define	MSC_CMDAT_DATA_EN		(1 << 3)
#define	MSC_CMDAT_RESPONSE_BIT	0
#define	MSC_CMDAT_RESPONSE_MASK	(0x7 << MSC_CMDAT_RESPONSE_BIT)
  #define MSC_CMDAT_RESPONSE_NONE (0x0 << MSC_CMDAT_RESPONSE_BIT) /* No response */
  #define MSC_CMDAT_RESPONSE_R1	  (0x1 << MSC_CMDAT_RESPONSE_BIT) /* Format R1 and R1b */
  #define MSC_CMDAT_RESPONSE_R2	  (0x2 << MSC_CMDAT_RESPONSE_BIT) /* Format R2 */
  #define MSC_CMDAT_RESPONSE_R3	  (0x3 << MSC_CMDAT_RESPONSE_BIT) /* Format R3 */
  #define MSC_CMDAT_RESPONSE_R4	  (0x4 << MSC_CMDAT_RESPONSE_BIT) /* Format R4 */
  #define MSC_CMDAT_RESPONSE_R5	  (0x5 << MSC_CMDAT_RESPONSE_BIT) /* Format R5 */
  #define MSC_CMDAT_RESPONSE_R6	  (0x6 << MSC_CMDAT_RESPONSE_BIT) /* Format R6 */

#define	CMDAT_DMA_EN	(1 << 8)
#define	CMDAT_INIT	(1 << 7)
#define	CMDAT_BUSY	(1 << 6)
#define	CMDAT_STREAM	(1 << 5)
#define	CMDAT_WRITE	(1 << 4)
#define	CMDAT_DATA_EN	(1 << 3)

/* MSC Interrupts Mask Register (MSC_IMASK) */

#define	MSC_IMASK_AUTO_CMD_DONE		(1 << 8)
#define	MSC_IMASK_SDIO			(1 << 7)
#define	MSC_IMASK_TXFIFO_WR_REQ		(1 << 6)
#define	MSC_IMASK_RXFIFO_RD_REQ		(1 << 5)
#define	MSC_IMASK_END_CMD_RES		(1 << 2)
#define	MSC_IMASK_PRG_DONE		(1 << 1)
#define	MSC_IMASK_DATA_TRAN_DONE	(1 << 0)


/* MSC Interrupts Status Register (MSC_IREG) */

#define	MSC_IREG_AUTO_CMD_DONE		(1 << 8)
#define	MSC_IREG_SDIO			(1 << 7)
#define	MSC_IREG_TXFIFO_WR_REQ		(1 << 6)
#define	MSC_IREG_RXFIFO_RD_REQ		(1 << 5)
#define	MSC_IREG_END_CMD_RES		(1 << 2)
#define	MSC_IREG_PRG_DONE		(1 << 1)
#define	MSC_IREG_DATA_TRAN_DONE		(1 << 0)


/*************************************************************************
 * EMC (External Memory Controller)
 *************************************************************************/
#define EMC_SMCR0	(EMC_BASE + 0x10)  /* Static Memory Control Register 0 */
#define EMC_SMCR1	(EMC_BASE + 0x14)  /* Static Memory Control Register 1 */
#define EMC_SMCR2	(EMC_BASE + 0x18)  /* Static Memory Control Register 2 */
#define EMC_SMCR3	(EMC_BASE + 0x1c)  /* Static Memory Control Register 3 */
#define EMC_SMCR4	(EMC_BASE + 0x20)  /* Static Memory Control Register 4 */
#define EMC_SACR0	(EMC_BASE + 0x30)  /* Static Memory Bank 0 Addr Config Reg */
#define EMC_SACR1	(EMC_BASE + 0x34)  /* Static Memory Bank 1 Addr Config Reg */
#define EMC_SACR2	(EMC_BASE + 0x38)  /* Static Memory Bank 2 Addr Config Reg */
#define EMC_SACR3	(EMC_BASE + 0x3c)  /* Static Memory Bank 3 Addr Config Reg */
#define EMC_SACR4	(EMC_BASE + 0x40)  /* Static Memory Bank 4 Addr Config Reg */

#define EMC_NFCSR	(EMC_BASE + 0x050) /* NAND Flash Control/Status Register */
#define EMC_NFECR	(EMC_BASE + 0x100) /* NAND Flash ECC Control Register */
#define EMC_NFECC	(EMC_BASE + 0x104) /* NAND Flash ECC Data Register */
#define EMC_NFPAR0	(EMC_BASE + 0x108) /* NAND Flash RS Parity 0 Register */
#define EMC_NFPAR1	(EMC_BASE + 0x10c) /* NAND Flash RS Parity 1 Register */
#define EMC_NFPAR2	(EMC_BASE + 0x110) /* NAND Flash RS Parity 2 Register */
#define EMC_NFINTS	(EMC_BASE + 0x114) /* NAND Flash Interrupt Status Register */
#define EMC_NFINTE	(EMC_BASE + 0x118) /* NAND Flash Interrupt Enable Register */
#define EMC_NFERR0	(EMC_BASE + 0x11c) /* NAND Flash RS Error Report 0 Register */
#define EMC_NFERR1	(EMC_BASE + 0x120) /* NAND Flash RS Error Report 1 Register */
#define EMC_NFERR2	(EMC_BASE + 0x124) /* NAND Flash RS Error Report 2 Register */
#define EMC_NFERR3	(EMC_BASE + 0x128) /* NAND Flash RS Error Report 3 Register */

#define EMC_DMCR	(EMC_BASE + 0x80)  /* DRAM Control Register */
#define EMC_RTCSR	(EMC_BASE + 0x84)  /* Refresh Time Control/Status Register */
#define EMC_RTCNT	(EMC_BASE + 0x88)  /* Refresh Timer Counter */
#define EMC_RTCOR	(EMC_BASE + 0x8c)  /* Refresh Time Constant Register */
#define EMC_DMAR0	(EMC_BASE + 0x90)  /* SDRAM Bank 0 Addr Config Register */
#define EMC_SDMR0	(EMC_BASE + 0xa000) /* Mode Register of SDRAM bank 0 */


#define REG_EMC_SMCR0	REG32(EMC_SMCR0)
#define REG_EMC_SMCR1	REG32(EMC_SMCR1)
#define REG_EMC_SMCR2	REG32(EMC_SMCR2)
#define REG_EMC_SMCR3	REG32(EMC_SMCR3)
#define REG_EMC_SMCR4	REG32(EMC_SMCR4)
#define REG_EMC_SACR0	REG32(EMC_SACR0)
#define REG_EMC_SACR1	REG32(EMC_SACR1)
#define REG_EMC_SACR2	REG32(EMC_SACR2)
#define REG_EMC_SACR3	REG32(EMC_SACR3)
#define REG_EMC_SACR4	REG32(EMC_SACR4)

#define REG_EMC_NFCSR	REG32(EMC_NFCSR)
#define REG_EMC_NFECR	REG32(EMC_NFECR)
#define REG_EMC_NFECC	REG32(EMC_NFECC)
#define REG_EMC_NFPAR0	REG32(EMC_NFPAR0)
#define REG_EMC_NFPAR1	REG32(EMC_NFPAR1)
#define REG_EMC_NFPAR2	REG32(EMC_NFPAR2)
#define REG_EMC_NFINTS	REG32(EMC_NFINTS)
#define REG_EMC_NFINTE	REG32(EMC_NFINTE)
#define REG_EMC_NFERR0	REG32(EMC_NFERR0)
#define REG_EMC_NFERR1	REG32(EMC_NFERR1)
#define REG_EMC_NFERR2	REG32(EMC_NFERR2)
#define REG_EMC_NFERR3	REG32(EMC_NFERR3)

#define REG_EMC_DMCR	REG32(EMC_DMCR)
#define REG_EMC_RTCSR	REG16(EMC_RTCSR)
#define REG_EMC_RTCNT	REG16(EMC_RTCNT)
#define REG_EMC_RTCOR	REG16(EMC_RTCOR)
#define REG_EMC_DMAR0	REG32(EMC_DMAR0)

/* Static Memory Control Register */
#define EMC_SMCR_STRV_BIT	24
#define EMC_SMCR_STRV_MASK	(0x0f << EMC_SMCR_STRV_BIT)
#define EMC_SMCR_TAW_BIT	20
#define EMC_SMCR_TAW_MASK	(0x0f << EMC_SMCR_TAW_BIT)
#define EMC_SMCR_TBP_BIT	16
#define EMC_SMCR_TBP_MASK	(0x0f << EMC_SMCR_TBP_BIT)
#define EMC_SMCR_TAH_BIT	12
#define EMC_SMCR_TAH_MASK	(0x07 << EMC_SMCR_TAH_BIT)
#define EMC_SMCR_TAS_BIT	8
#define EMC_SMCR_TAS_MASK	(0x07 << EMC_SMCR_TAS_BIT)
#define EMC_SMCR_BW_BIT		6
#define EMC_SMCR_BW_MASK	(0x03 << EMC_SMCR_BW_BIT)
  #define EMC_SMCR_BW_8BIT	(0 << EMC_SMCR_BW_BIT)
  #define EMC_SMCR_BW_16BIT	(1 << EMC_SMCR_BW_BIT)
  #define EMC_SMCR_BW_32BIT	(2 << EMC_SMCR_BW_BIT)
#define EMC_SMCR_BCM		(1 << 3)
#define EMC_SMCR_BL_BIT		1
#define EMC_SMCR_BL_MASK	(0x03 << EMC_SMCR_BL_BIT)
  #define EMC_SMCR_BL_4		(0 << EMC_SMCR_BL_BIT)
  #define EMC_SMCR_BL_8		(1 << EMC_SMCR_BL_BIT)
  #define EMC_SMCR_BL_16	(2 << EMC_SMCR_BL_BIT)
  #define EMC_SMCR_BL_32	(3 << EMC_SMCR_BL_BIT)
#define EMC_SMCR_SMT		(1 << 0)

/* Static Memory Bank Addr Config Reg */
#define EMC_SACR_BASE_BIT	8
#define EMC_SACR_BASE_MASK	(0xff << EMC_SACR_BASE_BIT)
#define EMC_SACR_MASK_BIT	0
#define EMC_SACR_MASK_MASK	(0xff << EMC_SACR_MASK_BIT)

/* NAND Flash Control/Status Register */
#define EMC_NFCSR_NFCE4		(1 << 7) /* NAND Flash Enable */
#define EMC_NFCSR_NFE4		(1 << 6) /* NAND Flash FCE# Assertion Enable */
#define EMC_NFCSR_NFCE3		(1 << 5)
#define EMC_NFCSR_NFE3		(1 << 4)
#define EMC_NFCSR_NFCE2		(1 << 3)
#define EMC_NFCSR_NFE2		(1 << 2)
#define EMC_NFCSR_NFCE1		(1 << 1)
#define EMC_NFCSR_NFE1		(1 << 0)

/* NAND Flash ECC Control Register */
#define EMC_NFECR_PRDY		(1 << 4) /* Parity Ready */
#define EMC_NFECR_RS_DECODING	(0 << 3) /* RS is in decoding phase */
#define EMC_NFECR_RS_ENCODING	(1 << 3) /* RS is in encoding phase */
#define EMC_NFECR_HAMMING	(0 << 2) /* Select HAMMING Correction Algorithm */
#define EMC_NFECR_RS		(1 << 2) /* Select RS Correction Algorithm */
#define EMC_NFECR_ERST		(1 << 1) /* ECC Reset */
#define EMC_NFECR_ECCE		(1 << 0) /* ECC Enable */

/* NAND Flash ECC Data Register */
#define EMC_NFECC_ECC2_BIT	16
#define EMC_NFECC_ECC2_MASK	(0xff << EMC_NFECC_ECC2_BIT)
#define EMC_NFECC_ECC1_BIT	8
#define EMC_NFECC_ECC1_MASK	(0xff << EMC_NFECC_ECC1_BIT)
#define EMC_NFECC_ECC0_BIT	0
#define EMC_NFECC_ECC0_MASK	(0xff << EMC_NFECC_ECC0_BIT)

/* NAND Flash Interrupt Status Register */
#define EMC_NFINTS_ERRCNT_BIT	29       /* Error Count */
#define EMC_NFINTS_ERRCNT_MASK	(0x7 << EMC_NFINTS_ERRCNT_BIT)
#define EMC_NFINTS_PADF		(1 << 4) /* Padding Finished */
#define EMC_NFINTS_DECF		(1 << 3) /* Decoding Finished */
#define EMC_NFINTS_ENCF		(1 << 2) /* Encoding Finished */
#define EMC_NFINTS_UNCOR	(1 << 1) /* Uncorrectable Error Occurred */
#define EMC_NFINTS_ERR		(1 << 0) /* Error Occurred */

/* NAND Flash Interrupt Enable Register */
#define EMC_NFINTE_PADFE	(1 << 4) /* Padding Finished Interrupt Enable */
#define EMC_NFINTE_DECFE	(1 << 3) /* Decoding Finished Interrupt Enable */
#define EMC_NFINTE_ENCFE	(1 << 2) /* Encoding Finished Interrupt Enable */
#define EMC_NFINTE_UNCORE	(1 << 1) /* Uncorrectable Error Occurred Intr Enable */
#define EMC_NFINTE_ERRE		(1 << 0) /* Error Occurred Interrupt */

/* NAND Flash RS Error Report Register */
#define EMC_NFERR_INDEX_BIT	16       /* Error Symbol Index */
#define EMC_NFERR_INDEX_MASK	(0x1ff << EMC_NFERR_INDEX_BIT)
#define EMC_NFERR_MASK_BIT	0        /* Error Symbol Value */
#define EMC_NFERR_MASK_MASK	(0x1ff << EMC_NFERR_MASK_BIT)


/* DRAM Control Register */
#define EMC_DMCR_BW_BIT		31
#define EMC_DMCR_BW		(1 << EMC_DMCR_BW_BIT)
#define EMC_DMCR_CA_BIT		26
#define EMC_DMCR_CA_MASK	(0x07 << EMC_DMCR_CA_BIT)
  #define EMC_DMCR_CA_8		(0 << EMC_DMCR_CA_BIT)
  #define EMC_DMCR_CA_9		(1 << EMC_DMCR_CA_BIT)
  #define EMC_DMCR_CA_10	(2 << EMC_DMCR_CA_BIT)
  #define EMC_DMCR_CA_11	(3 << EMC_DMCR_CA_BIT)
  #define EMC_DMCR_CA_12	(4 << EMC_DMCR_CA_BIT)
#define EMC_DMCR_RMODE		(1 << 25)
#define EMC_DMCR_RFSH		(1 << 24)
#define EMC_DMCR_MRSET		(1 << 23)
#define EMC_DMCR_RA_BIT		20
#define EMC_DMCR_RA_MASK	(0x03 << EMC_DMCR_RA_BIT)
  #define EMC_DMCR_RA_11	(0 << EMC_DMCR_RA_BIT)
  #define EMC_DMCR_RA_12	(1 << EMC_DMCR_RA_BIT)
  #define EMC_DMCR_RA_13	(2 << EMC_DMCR_RA_BIT)
#define EMC_DMCR_BA_BIT		19
#define EMC_DMCR_BA		(1 << EMC_DMCR_BA_BIT)
#define EMC_DMCR_PDM		(1 << 18)
#define EMC_DMCR_EPIN		(1 << 17)
#define EMC_DMCR_TRAS_BIT	13
#define EMC_DMCR_TRAS_MASK	(0x07 << EMC_DMCR_TRAS_BIT)
#define EMC_DMCR_RCD_BIT	11
#define EMC_DMCR_RCD_MASK	(0x03 << EMC_DMCR_RCD_BIT)
#define EMC_DMCR_TPC_BIT	8
#define EMC_DMCR_TPC_MASK	(0x07 << EMC_DMCR_TPC_BIT)
#define EMC_DMCR_TRWL_BIT	5
#define EMC_DMCR_TRWL_MASK	(0x03 << EMC_DMCR_TRWL_BIT)
#define EMC_DMCR_TRC_BIT	2
#define EMC_DMCR_TRC_MASK	(0x07 << EMC_DMCR_TRC_BIT)
#define EMC_DMCR_TCL_BIT	0
#define EMC_DMCR_TCL_MASK	(0x03 << EMC_DMCR_TCL_BIT)

/* Refresh Time Control/Status Register */
#define EMC_RTCSR_CMF		(1 << 7)
#define EMC_RTCSR_CKS_BIT	0
#define EMC_RTCSR_CKS_MASK	(0x07 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_DISABLE	(0 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_4	(1 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_16	(2 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_64	(3 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_256	(4 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_1024	(5 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_2048	(6 << EMC_RTCSR_CKS_BIT)
  #define EMC_RTCSR_CKS_4096	(7 << EMC_RTCSR_CKS_BIT)

/* SDRAM Bank Address Configuration Register */
#define EMC_DMAR_BASE_BIT	8
#define EMC_DMAR_BASE_MASK	(0xff << EMC_DMAR_BASE_BIT)
#define EMC_DMAR_MASK_BIT	0
#define EMC_DMAR_MASK_MASK	(0xff << EMC_DMAR_MASK_BIT)

/* Mode Register of SDRAM bank 0 */
#define EMC_SDMR_BM		(1 << 9) /* Write Burst Mode */
#define EMC_SDMR_OM_BIT		7        /* Operating Mode */
#define EMC_SDMR_OM_MASK	(3 << EMC_SDMR_OM_BIT)
  #define EMC_SDMR_OM_NORMAL	(0 << EMC_SDMR_OM_BIT)
#define EMC_SDMR_CAS_BIT	4        /* CAS Latency */
#define EMC_SDMR_CAS_MASK	(7 << EMC_SDMR_CAS_BIT)
  #define EMC_SDMR_CAS_1	(1 << EMC_SDMR_CAS_BIT)
  #define EMC_SDMR_CAS_2	(2 << EMC_SDMR_CAS_BIT)
  #define EMC_SDMR_CAS_3	(3 << EMC_SDMR_CAS_BIT)
#define EMC_SDMR_BT_BIT		3        /* Burst Type */
#define EMC_SDMR_BT_MASK	(1 << EMC_SDMR_BT_BIT)
  #define EMC_SDMR_BT_SEQ	(0 << EMC_SDMR_BT_BIT) /* Sequential */
  #define EMC_SDMR_BT_INT	(1 << EMC_SDMR_BT_BIT) /* Interleave */
#define EMC_SDMR_BL_BIT		0        /* Burst Length */
#define EMC_SDMR_BL_MASK	(7 << EMC_SDMR_BL_BIT)
  #define EMC_SDMR_BL_1		(0 << EMC_SDMR_BL_BIT)
  #define EMC_SDMR_BL_2		(1 << EMC_SDMR_BL_BIT)
  #define EMC_SDMR_BL_4		(2 << EMC_SDMR_BL_BIT)
  #define EMC_SDMR_BL_8		(3 << EMC_SDMR_BL_BIT)

#define EMC_SDMR_CAS2_16BIT \
  (EMC_SDMR_CAS_2 | EMC_SDMR_BT_SEQ | EMC_SDMR_BL_2)
#define EMC_SDMR_CAS2_32BIT \
  (EMC_SDMR_CAS_2 | EMC_SDMR_BT_SEQ | EMC_SDMR_BL_4)
#define EMC_SDMR_CAS3_16BIT \
  (EMC_SDMR_CAS_3 | EMC_SDMR_BT_SEQ | EMC_SDMR_BL_2)
#define EMC_SDMR_CAS3_32BIT \
  (EMC_SDMR_CAS_3 | EMC_SDMR_BT_SEQ | EMC_SDMR_BL_4)


/*************************************************************************
 * CIM
 *************************************************************************/
#define	CIM_CFG			(CIM_BASE + 0x0000)
#define	CIM_CTRL		(CIM_BASE + 0x0004)
#define	CIM_STATE		(CIM_BASE + 0x0008)
#define	CIM_IID			(CIM_BASE + 0x000C)
#define	CIM_RXFIFO		(CIM_BASE + 0x0010)
#define	CIM_DA			(CIM_BASE + 0x0020)
#define	CIM_FA			(CIM_BASE + 0x0024)
#define	CIM_FID			(CIM_BASE + 0x0028)
#define	CIM_CMD			(CIM_BASE + 0x002C)
#define	CIM_SIZE		(CIM_BASE + 0x0030)
#define	CIM_OFFSET		(CIM_BASE + 0x0034)

#define	REG_CIM_CFG		REG32(CIM_CFG)
#define	REG_CIM_CTRL		REG32(CIM_CTRL)
#define	REG_CIM_STATE		REG32(CIM_STATE)
#define	REG_CIM_IID		REG32(CIM_IID)
#define	REG_CIM_RXFIFO		REG32(CIM_RXFIFO)
#define	REG_CIM_DA		REG32(CIM_DA)
#define	REG_CIM_FA		REG32(CIM_FA)
#define	REG_CIM_FID		REG32(CIM_FID)
#define	REG_CIM_CMD		REG32(CIM_CMD)
#define	REG_CIM_SIZE		REG32(CIM_SIZE)
#define	REG_CIM_OFFSET		REG32(CIM_OFFSET)

/* CIM Configuration Register  (CIM_CFG) */

#define	CIM_CFG_ORDER_BIT	18
#define	CIM_CFG_ORDER_MASK	(0x3 << CIM_CFG_ORDER_BIT)
  #define CIM_CFG_ORDER_0	  (0x0 << CIM_CFG_ORDER_BIT)
  #define CIM_CFG_ORDER_1	  (0x1 << CIM_CFG_ORDER_BIT)
  #define CIM_CFG_ORDER_2	  (0x2 << CIM_CFG_ORDER_BIT)
  #define CIM_CFG_ORDER_3	  (0x3 << CIM_CFG_ORDER_BIT)
#define	CIM_CFG_DF_BIT		16
#define	CIM_CFG_DF_MASK		  (0x3 << CIM_CFG_DF_BIT)
  #define CIM_CFG_DF_RAWRGB	  (0x0 << CIM_CFG_DF_BIT)
  #define CIM_CFG_DF_YUV444	  (0x1 << CIM_CFG_DF_BIT)
  #define CIM_CFG_DF_YUV422	  (0x2 << CIM_CFG_DF_BIT)
  #define CIM_CFG_DF_ITU656	  (0x3 << CIM_CFG_DF_BIT)
#define	CIM_CFG_INV_DAT		(1 << 15)
#define	CIM_CFG_VSP		(1 << 14)
#define	CIM_CFG_HSP		(1 << 13)
#define	CIM_CFG_PCP		(1 << 12)
#define	CIM_CFG_DUMMY_ZERO	(1 << 9)
#define	CIM_CFG_EXT_VSYNC	(1 << 8)
#define	CIM_CFG_PACK_BIT	4
#define	CIM_CFG_PACK_MASK	(0x7 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_0	  (0 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_1	  (1 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_2	  (2 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_3	  (3 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_4	  (4 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_5	  (5 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_6	  (6 << CIM_CFG_PACK_BIT)
  #define CIM_CFG_PACK_7	  (7 << CIM_CFG_PACK_BIT)
#define	CIM_CFG_BYPASS_BIT	2
#define	CIM_CFG_BYPASS_MASK	(1 << CIM_CFG_BYPASS_BIT)
  #define CIM_CFG_BYPASS	  (1 << CIM_CFG_BYPASS_BIT)
#define	CIM_CFG_DSM_BIT		0
#define	CIM_CFG_DSM_MASK	(0x3 << CIM_CFG_DSM_BIT)
  #define CIM_CFG_DSM_CPM	  (0 << CIM_CFG_DSM_BIT) /* CCIR656 Progressive Mode */
  #define CIM_CFG_DSM_CIM	  (1 << CIM_CFG_DSM_BIT) /* CCIR656 Interlace Mode */
  #define CIM_CFG_DSM_GCM	  (2 << CIM_CFG_DSM_BIT) /* Gated Clock Mode */
  #define CIM_CFG_DSM_NGCM	  (3 << CIM_CFG_DSM_BIT) /* Non-Gated Clock Mode */

/* CIM Control Register  (CIM_CTRL) */

#define	CIM_CTRL_MCLKDIV_BIT	24
#define	CIM_CTRL_MCLKDIV_MASK	(0xff << CIM_CTRL_MCLKDIV_BIT)
#define	CIM_CTRL_FRC_BIT	16
#define	CIM_CTRL_FRC_MASK	(0xf << CIM_CTRL_FRC_BIT)
  #define CIM_CTRL_FRC_1	  (0x0 << CIM_CTRL_FRC_BIT) /* Sample every frame */
  #define CIM_CTRL_FRC_2	  (0x1 << CIM_CTRL_FRC_BIT) /* Sample 1/2 frame */
  #define CIM_CTRL_FRC_3	  (0x2 << CIM_CTRL_FRC_BIT) /* Sample 1/3 frame */
  #define CIM_CTRL_FRC_4	  (0x3 << CIM_CTRL_FRC_BIT) /* Sample 1/4 frame */
  #define CIM_CTRL_FRC_5	  (0x4 << CIM_CTRL_FRC_BIT) /* Sample 1/5 frame */
  #define CIM_CTRL_FRC_6	  (0x5 << CIM_CTRL_FRC_BIT) /* Sample 1/6 frame */
  #define CIM_CTRL_FRC_7	  (0x6 << CIM_CTRL_FRC_BIT) /* Sample 1/7 frame */
  #define CIM_CTRL_FRC_8	  (0x7 << CIM_CTRL_FRC_BIT) /* Sample 1/8 frame */
  #define CIM_CTRL_FRC_9	  (0x8 << CIM_CTRL_FRC_BIT) /* Sample 1/9 frame */
  #define CIM_CTRL_FRC_10	  (0x9 << CIM_CTRL_FRC_BIT) /* Sample 1/10 frame */
  #define CIM_CTRL_FRC_11	  (0xA << CIM_CTRL_FRC_BIT) /* Sample 1/11 frame */
  #define CIM_CTRL_FRC_12	  (0xB << CIM_CTRL_FRC_BIT) /* Sample 1/12 frame */
  #define CIM_CTRL_FRC_13	  (0xC << CIM_CTRL_FRC_BIT) /* Sample 1/13 frame */
  #define CIM_CTRL_FRC_14	  (0xD << CIM_CTRL_FRC_BIT) /* Sample 1/14 frame */
  #define CIM_CTRL_FRC_15	  (0xE << CIM_CTRL_FRC_BIT) /* Sample 1/15 frame */
  #define CIM_CTRL_FRC_16	  (0xF << CIM_CTRL_FRC_BIT) /* Sample 1/16 frame */
#define	CIM_CTRL_SIZEEN_BIT	14
#define	CIM_CTRL_SIZEEN_MASK	(0x1 << CIM_CTRL_SIZEEN_BIT)
#define	CIM_CTRL_VDDM		(1 << 13)
#define	CIM_CTRL_DMA_SOFM	(1 << 12)
#define	CIM_CTRL_DMA_EOFM	(1 << 11)
#define	CIM_CTRL_DMA_STOPM	(1 << 10)
#define	CIM_CTRL_RXF_TRIGM	(1 << 9)
#define	CIM_CTRL_RXF_OFM	(1 << 8)
#define	CIM_CTRL_RXF_TRIG_BIT	4
#define	CIM_CTRL_RXF_TRIG_MASK	(0x7 << CIM_CTRL_RXF_TRIG_BIT)
  #define CIM_CTRL_RXF_TRIG_4	  (0 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 4 */
  #define CIM_CTRL_RXF_TRIG_8	  (1 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 8 */
  #define CIM_CTRL_RXF_TRIG_12	  (2 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 12 */
  #define CIM_CTRL_RXF_TRIG_16	  (3 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 16 */
  #define CIM_CTRL_RXF_TRIG_20	  (4 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 20 */
  #define CIM_CTRL_RXF_TRIG_24	  (5 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 24 */
  #define CIM_CTRL_RXF_TRIG_28	  (6 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 28 */
  #define CIM_CTRL_RXF_TRIG_32	  (7 << CIM_CTRL_RXF_TRIG_BIT) /* RXFIFO Trigger Value is 32 */
#define	CIM_CTRL_DMA_EN		(1 << 2)
#define	CIM_CTRL_RXF_RST	(1 << 1)
#define	CIM_CTRL_ENA		(1 << 0)

/* CIM State Register  (CIM_STATE) */

#define	CIM_STATE_DMA_SOF	(1 << 6)
#define	CIM_STATE_DMA_EOF	(1 << 5)
#define	CIM_STATE_DMA_STOP	(1 << 4)
#define	CIM_STATE_RXF_OF	(1 << 3)
#define	CIM_STATE_RXF_TRIG	(1 << 2)
#define	CIM_STATE_RXF_EMPTY	(1 << 1)
#define	CIM_STATE_VDD		(1 << 0)

/* CIM DMA Command Register (CIM_CMD) */

#define	CIM_CMD_SOFINT		(1 << 31)
#define	CIM_CMD_EOFINT		(1 << 30)
#define	CIM_CMD_STOP		(1 << 28)
#define	CIM_CMD_LEN_BIT		0
#define	CIM_CMD_LEN_MASK	(0xffffff << CIM_CMD_LEN_BIT)

/* CIM Image Size Register  (CIM_SIZE) */
#define	CIM_SIZE_LPF_BIT	16
#define	CIM_SIZE_LPF_MASK	(0x1fff << CIM_SIZE_LPF_BIT)
#define	CIM_SIZE_PPL_BIT	0
#define	CIM_SIZE_PPL_MASK	(0x1fff << CIM_SIZE_PPL_BIT)

/* CIM Image Offset Register  (CIM_OFFISET) */
#define	CIM_OFFSET_V_BIT	16
#define	CIM_OFFSET_V_MASK	(0xfff << CIM_OFFSET_V_BIT)
#define	CIM_OFFSET_H_BIT	0
#define	CIM_OFFSET_H_MASK	(0xfff << CIM_OFFSET_H_BIT) /*OFFSET_H should be even number*/

/*************************************************************************
 * SADC (Smart A/D Controller)
 *************************************************************************/

#define SADC_ENA	(SADC_BASE + 0x00)  /* ADC Enable Register */
#define SADC_CFG	(SADC_BASE + 0x04)  /* ADC Configure Register */
#define SADC_CTRL	(SADC_BASE + 0x08)  /* ADC Control Register */
#define SADC_STATE	(SADC_BASE + 0x0C)  /* ADC Status Register*/
#define SADC_SAMETIME	(SADC_BASE + 0x10)  /* ADC Same Point Time Register */
#define SADC_WAITTIME	(SADC_BASE + 0x14)  /* ADC Wait Time Register */
#define SADC_TSDAT	(SADC_BASE + 0x18)  /* ADC Touch Screen Data Register */
#define SADC_BATDAT	(SADC_BASE + 0x1C)  /* ADC PBAT Data Register */
#define SADC_SADDAT	(SADC_BASE + 0x20)  /* ADC SADCIN Data Register */

#define REG_SADC_ENA		REG8(SADC_ENA)
#define REG_SADC_CFG		REG32(SADC_CFG)
#define REG_SADC_CTRL		REG8(SADC_CTRL)
#define REG_SADC_STATE		REG8(SADC_STATE)
#define REG_SADC_SAMETIME	REG16(SADC_SAMETIME)
#define REG_SADC_WAITTIME	REG16(SADC_WAITTIME)
#define REG_SADC_TSDAT		REG32(SADC_TSDAT)
#define REG_SADC_BATDAT		REG16(SADC_BATDAT)
#define REG_SADC_SADDAT		REG16(SADC_SADDAT)

/* ADC Enable Register */
#define SADC_ENA_ADEN		(1 << 7)  /* Touch Screen Enable */
#define SADC_ENA_TSEN		(1 << 2)  /* Touch Screen Enable */
#define SADC_ENA_PBATEN		(1 << 1)  /* PBAT Enable */
#define SADC_ENA_SADCINEN	(1 << 0)  /* SADCIN Enable */

/* ADC Configure Register */
#define SADC_CFG_EXIN           (1 << 30)
#define SADC_CFG_CLKOUT_NUM_BIT	16
#define SADC_CFG_CLKOUT_NUM_MASK (0x7 << SADC_CFG_CLKOUT_NUM_BIT)
#define SADC_CFG_TS_DMA		(1 << 15)  /* Touch Screen DMA Enable */
#define SADC_CFG_XYZ_BIT	13  /* XYZ selection */
#define SADC_CFG_XYZ_MASK	(0x3 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XY		(0 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XYZ		(1 << SADC_CFG_XYZ_BIT)
  #define SADC_CFG_XYZ1Z2	(2 << SADC_CFG_XYZ_BIT)
#define SADC_CFG_SNUM_BIT	10  /* Sample Number */
#define SADC_CFG_SNUM_MASK	(0x7 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_1	(0x0 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_2	(0x1 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_3	(0x2 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_4	(0x3 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_5	(0x4 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_6	(0x5 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_8	(0x6 << SADC_CFG_SNUM_BIT)
  #define SADC_CFG_SNUM_9	(0x7 << SADC_CFG_SNUM_BIT)
#define SADC_CFG_CLKDIV_BIT	5  /* AD Converter frequency clock divider */
#define SADC_CFG_CLKDIV_MASK	(0x1f << SADC_CFG_CLKDIV_BIT)
#define SADC_CFG_PBAT_HIGH	(0 << 4)  /* PBAT >= 2.5V */
#define SADC_CFG_PBAT_LOW	(1 << 4)  /* PBAT < 2.5V */
#define SADC_CFG_CMD_BIT	0  /* ADC Command */
#define SADC_CFG_CMD_MASK	(0xf << SADC_CFG_CMD_BIT)
  #define SADC_CFG_CMD_X_SE	(0x0 << SADC_CFG_CMD_BIT) /* X Single-End */
  #define SADC_CFG_CMD_Y_SE	(0x1 << SADC_CFG_CMD_BIT) /* Y Single-End */
  #define SADC_CFG_CMD_X_DIFF	(0x2 << SADC_CFG_CMD_BIT) /* X Differential */
  #define SADC_CFG_CMD_Y_DIFF	(0x3 << SADC_CFG_CMD_BIT) /* Y Differential */
  #define SADC_CFG_CMD_Z1_DIFF	(0x4 << SADC_CFG_CMD_BIT) /* Z1 Differential */
  #define SADC_CFG_CMD_Z2_DIFF	(0x5 << SADC_CFG_CMD_BIT) /* Z2 Differential */
  #define SADC_CFG_CMD_Z3_DIFF	(0x6 << SADC_CFG_CMD_BIT) /* Z3 Differential */
  #define SADC_CFG_CMD_Z4_DIFF	(0x7 << SADC_CFG_CMD_BIT) /* Z4 Differential */
  #define SADC_CFG_CMD_TP_SE	(0x8 << SADC_CFG_CMD_BIT) /* Touch Pressure */
  #define SADC_CFG_CMD_PBATH_SE	(0x9 << SADC_CFG_CMD_BIT) /* PBAT >= 2.5V */
  #define SADC_CFG_CMD_PBATL_SE	(0xa << SADC_CFG_CMD_BIT) /* PBAT < 2.5V */
  #define SADC_CFG_CMD_SADCIN_SE (0xb << SADC_CFG_CMD_BIT) /* Measure SADCIN */
  #define SADC_CFG_CMD_INT_PEN	(0xc << SADC_CFG_CMD_BIT) /* INT_PEN Enable */

/* ADC Control Register */
#define SADC_CTRL_PENDM		(1 << 4)  /* Pen Down Interrupt Mask */
#define SADC_CTRL_PENUM		(1 << 3)  /* Pen Up Interrupt Mask */
#define SADC_CTRL_TSRDYM	(1 << 2)  /* Touch Screen Data Ready Interrupt Mask */
#define SADC_CTRL_PBATRDYM	(1 << 1)  /* PBAT Data Ready Interrupt Mask */
#define SADC_CTRL_SRDYM		(1 << 0)  /* SADCIN Data Ready Interrupt Mask */

/* ADC Status Register */
#define SADC_STATE_TSBUSY	(1 << 7)  /* TS A/D is working */
#define SADC_STATE_PBATBUSY	(1 << 6)  /* PBAT A/D is working */
#define SADC_STATE_SBUSY	(1 << 5)  /* SADCIN A/D is working */
#define SADC_STATE_PEND		(1 << 4)  /* Pen Down Interrupt Flag */
#define SADC_STATE_PENU		(1 << 3)  /* Pen Up Interrupt Flag */
#define SADC_STATE_TSRDY	(1 << 2)  /* Touch Screen Data Ready Interrupt Flag */
#define SADC_STATE_PBATRDY	(1 << 1)  /* PBAT Data Ready Interrupt Flag */
#define SADC_STATE_SRDY		(1 << 0)  /* SADCIN Data Ready Interrupt Flag */

/* ADC Touch Screen Data Register */
#define SADC_TSDAT_DATA0_BIT	0
#define SADC_TSDAT_DATA0_MASK	(0xfff << SADC_TSDAT_DATA0_BIT)
#define SADC_TSDAT_TYPE0	(1 << 15)
#define SADC_TSDAT_DATA1_BIT	16
#define SADC_TSDAT_DATA1_MASK	(0xfff << SADC_TSDAT_DATA1_BIT)
#define SADC_TSDAT_TYPE1	(1 << 31)


/*************************************************************************
 * SLCD (Smart LCD Controller)
 *************************************************************************/

#define SLCD_CFG	(SLCD_BASE + 0xA0)  /* SLCD Configure Register */
#define SLCD_CTRL	(SLCD_BASE + 0xA4)  /* SLCD Control Register */
#define SLCD_STATE	(SLCD_BASE + 0xA8)  /* SLCD Status Register */
#define SLCD_DATA	(SLCD_BASE + 0xAC)  /* SLCD Data Register */

#define REG_SLCD_CFG	REG32(SLCD_CFG)
#define REG_SLCD_CTRL	REG8(SLCD_CTRL)
#define REG_SLCD_STATE	REG8(SLCD_STATE)
#define REG_SLCD_DATA	REG32(SLCD_DATA)

/* SLCD Configure Register */
#define SLCD_CFG_BURST_BIT	14
#define SLCD_CFG_BURST_MASK	(0x3 << SLCD_CFG_BURST_BIT)
  #define SLCD_CFG_BURST_4_WORD	(0 << SLCD_CFG_BURST_BIT)
  #define SLCD_CFG_BURST_8_WORD	(1 << SLCD_CFG_BURST_BIT)
#define SLCD_CFG_DWIDTH_BIT	10
#define SLCD_CFG_DWIDTH_MASK	(0x7 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_18	(0 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_16	(1 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_8_x3	(2 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_8_x2	(3 << SLCD_CFG_DWIDTH_BIT)
  #define SLCD_CFG_DWIDTH_9_x2	(4 << SLCD_CFG_DWIDTH_BIT)
#define SLCD_CFG_CWIDTH_16BIT	(0 << 8)
#define SLCD_CFG_CWIDTH_8BIT	(1 << 8)
#define SLCD_CFG_CS_ACTIVE_LOW	(0 << 4)
#define SLCD_CFG_CS_ACTIVE_HIGH	(1 << 4)
#define SLCD_CFG_RS_CMD_LOW	(0 << 3)
#define SLCD_CFG_RS_CMD_HIGH	(1 << 3)
#define SLCD_CFG_CLK_ACTIVE_FALLING	(0 << 1)
#define SLCD_CFG_CLK_ACTIVE_RISING	(1 << 1)
#define SLCD_CFG_TYPE_PARALLEL	(0 << 0)
#define SLCD_CFG_TYPE_SERIAL	(1 << 0)

/* SLCD Control Register */
#define SLCD_CTRL_DMA_EN	(1 << 0)

/* SLCD Status Register */
#define SLCD_STATE_BUSY		(1 << 0)

/* SLCD Data Register */
#define SLCD_DATA_RS_DATA	(0 << 31)
#define SLCD_DATA_RS_COMMAND	(1 << 31)

/*************************************************************************
 * LCD (LCD Controller)
 *************************************************************************/
#define LCD_CFG		(LCD_BASE + 0x00) /* LCD Configure Register */
#define LCD_CTRL	(LCD_BASE + 0x30) /* LCD Control Register */
#define LCD_STATE	(LCD_BASE + 0x34) /* LCD Status Register */

#define LCD_OSDC	(LCD_BASE + 0x100) /* LCD OSD Configure Register */
#define LCD_OSDCTRL	(LCD_BASE + 0x104) /* LCD OSD Control Register */
#define LCD_OSDS	(LCD_BASE + 0x108) /* LCD OSD Status Register */
#define LCD_BGC		(LCD_BASE + 0x10C) /* LCD Background Color Register */
#define LCD_KEY0	(LCD_BASE + 0x110) /* LCD Foreground Color Key Register 0 */
#define LCD_KEY1	(LCD_BASE + 0x114) /* LCD Foreground Color Key Register 1 */
#define LCD_ALPHA	(LCD_BASE + 0x118) /* LCD ALPHA Register */
#define LCD_IPUR	(LCD_BASE + 0x11C) /* LCD IPU Restart Register */

#define LCD_VAT		(LCD_BASE + 0x0c) /* Virtual Area Setting Register */
#define LCD_DAH		(LCD_BASE + 0x10) /* Display Area Horizontal Start/End Point */
#define LCD_DAV		(LCD_BASE + 0x14) /* Display Area Vertical Start/End Point */

#define LCD_XYP0	(LCD_BASE + 0x120) /* Foreground 0 XY Position Register */
#define LCD_XYP1	(LCD_BASE + 0x124) /* Foreground 1 XY Position Register */
#define LCD_SIZE0	(LCD_BASE + 0x128) /* Foreground 0 Size Register */
#define LCD_SIZE1	(LCD_BASE + 0x12C) /* Foreground 1 Size Register */
#define LCD_RGBC	(LCD_BASE + 0x90) /* RGB Controll Register */

#define LCD_VSYNC	(LCD_BASE + 0x04) /* Vertical Synchronize Register */
#define LCD_HSYNC	(LCD_BASE + 0x08) /* Horizontal Synchronize Register */
#define LCD_PS		(LCD_BASE + 0x18) /* PS Signal Setting */
#define LCD_CLS		(LCD_BASE + 0x1c) /* CLS Signal Setting */
#define LCD_SPL		(LCD_BASE + 0x20) /* SPL Signal Setting */
#define LCD_REV		(LCD_BASE + 0x24) /* REV Signal Setting */
#define LCD_IID		(LCD_BASE + 0x38) /* Interrupt ID Register */
#define LCD_DA0		(LCD_BASE + 0x40) /* Descriptor Address Register 0 */
#define LCD_SA0		(LCD_BASE + 0x44) /* Source Address Register 0 */
#define LCD_FID0	(LCD_BASE + 0x48) /* Frame ID Register 0 */
#define LCD_CMD0	(LCD_BASE + 0x4c) /* DMA Command Register 0 */
#define LCD_DA1		(LCD_BASE + 0x50) /* Descriptor Address Register 1 */
#define LCD_SA1		(LCD_BASE + 0x54) /* Source Address Register 1 */
#define LCD_FID1	(LCD_BASE + 0x58) /* Frame ID Register 1 */
#define LCD_CMD1	(LCD_BASE + 0x5c) /* DMA Command Register 1 */

#define LCD_OFFS0	(LCD_BASE + 0x60) /* DMA Offsize Register 0 */
#define LCD_PW0		(LCD_BASE + 0x64) /* DMA Page Width Register 0 */
#define LCD_CNUM0	(LCD_BASE + 0x68) /* DMA Command Counter Register 0 */
#define LCD_DESSIZE0	(LCD_BASE + 0x6C) /* Foreground Size in Descriptor 0 Register*/
#define LCD_OFFS1	(LCD_BASE + 0x70) /* DMA Offsize Register 1 */
#define LCD_PW1		(LCD_BASE + 0x74) /* DMA Page Width Register 1 */
#define LCD_CNUM1	(LCD_BASE + 0x78) /* DMA Command Counter Register 1 */
#define LCD_DESSIZE1	(LCD_BASE + 0x7C) /* Foreground Size in Descriptor 1 Register*/

#define REG_LCD_CFG	REG32(LCD_CFG)
#define REG_LCD_CTRL	REG32(LCD_CTRL)
#define REG_LCD_STATE	REG32(LCD_STATE)

#define REG_LCD_OSDC	REG16(LCD_OSDC)
#define REG_LCD_OSDCTRL	REG16(LCD_OSDCTRL)
#define REG_LCD_OSDS	REG16(LCD_OSDS)
#define REG_LCD_BGC	REG32(LCD_BGC)
#define REG_LCD_KEY0	REG32(LCD_KEY0)
#define REG_LCD_KEY1	REG32(LCD_KEY1)
#define REG_LCD_ALPHA	REG8(LCD_ALPHA)
#define REG_LCD_IPUR	REG32(LCD_IPUR)

#define REG_LCD_VAT	REG32(LCD_VAT)
#define REG_LCD_DAH	REG32(LCD_DAH)
#define REG_LCD_DAV	REG32(LCD_DAV)

#define REG_LCD_XYP0	REG32(LCD_XYP0)
#define REG_LCD_XYP1	REG32(LCD_XYP1)
#define REG_LCD_SIZE0	REG32(LCD_SIZE0)
#define REG_LCD_SIZE1	REG32(LCD_SIZE1)
#define REG_LCD_RGBC	REG16(LCD_RGBC)

#define REG_LCD_VSYNC	REG32(LCD_VSYNC)
#define REG_LCD_HSYNC	REG32(LCD_HSYNC)
#define REG_LCD_PS	REG32(LCD_PS)
#define REG_LCD_CLS	REG32(LCD_CLS)
#define REG_LCD_SPL	REG32(LCD_SPL)
#define REG_LCD_REV	REG32(LCD_REV)
#define REG_LCD_IID	REG32(LCD_IID)
#define REG_LCD_DA0	REG32(LCD_DA0)
#define REG_LCD_SA0	REG32(LCD_SA0)
#define REG_LCD_FID0	REG32(LCD_FID0)
#define REG_LCD_CMD0	REG32(LCD_CMD0)
#define REG_LCD_DA1	REG32(LCD_DA1)
#define REG_LCD_SA1	REG32(LCD_SA1)
#define REG_LCD_FID1	REG32(LCD_FID1)
#define REG_LCD_CMD1	REG32(LCD_CMD1)

#define REG_LCD_OFFS0	REG32(LCD_OFFS0)
#define REG_LCD_PW0	REG32(LCD_PW0)
#define REG_LCD_CNUM0	REG32(LCD_CNUM0)
#define REG_LCD_DESSIZE0	REG32(LCD_DESSIZE0)
#define REG_LCD_OFFS1	REG32(LCD_OFFS1)
#define REG_LCD_PW1	REG32(LCD_PW1)
#define REG_LCD_CNUM1	REG32(LCD_CNUM1)
#define REG_LCD_DESSIZE1	REG32(LCD_DESSIZE1)

/* LCD Configure Register */
#define LCD_CFG_LCDPIN_BIT	31  /* LCD pins selection */
#define LCD_CFG_LCDPIN_MASK	(0x1 << LCD_CFG_LCDPIN_BIT)
  #define LCD_CFG_LCDPIN_LCD	(0x0 << LCD_CFG_LCDPIN_BIT)
  #define LCD_CFG_LCDPIN_SLCD	(0x1 << LCD_CFG_LCDPIN_BIT)
#define LCD_CFG_FUHOLD		(1 << 29) /* hold pixel clock when outFIFO underrun */
#define LCD_CFG_NEWDES		(1 << 28) /* use new descripter. old: 4words, new:8words */
#define LCD_CFG_PALBP		(1 << 27) /* bypass data format and alpha blending */
#define LCD_CFG_TVEN		(1 << 26) /* indicate the terminal is lcd or tv */
#define LCD_CFG_RECOVER		(1 << 25) /* Auto recover when output fifo underrun */
#define LCD_CFG_DITHER		(1 << 24) /* Dither function */
#define LCD_CFG_PSM		(1 << 23) /* PS signal mode */
#define LCD_CFG_CLSM		(1 << 22) /* CLS signal mode */
#define LCD_CFG_SPLM		(1 << 21) /* SPL signal mode */
#define LCD_CFG_REVM		(1 << 20) /* REV signal mode */
#define LCD_CFG_HSYNM		(1 << 19) /* HSYNC signal mode */
#define LCD_CFG_PCLKM		(1 << 18) /* PCLK signal mode */
#define LCD_CFG_INVDAT		(1 << 17) /* Inverse output data */
#define LCD_CFG_SYNDIR_IN	(1 << 16) /* VSYNC&HSYNC direction */
#define LCD_CFG_PSP		(1 << 15) /* PS pin reset state */
#define LCD_CFG_CLSP		(1 << 14) /* CLS pin reset state */
#define LCD_CFG_SPLP		(1 << 13) /* SPL pin reset state */
#define LCD_CFG_REVP		(1 << 12) /* REV pin reset state */
#define LCD_CFG_HSP		(1 << 11) /* HSYNC pority:0-active high,1-active low */
#define LCD_CFG_PCP		(1 << 10) /* PCLK pority:0-rising,1-falling */
#define LCD_CFG_DEP		(1 << 9)  /* DE pority:0-active high,1-active low */
#define LCD_CFG_VSP		(1 << 8)  /* VSYNC pority:0-rising,1-falling */
#define MODE_TFT_18BIT          (1 << 7)  /* 18bit TFT */
#define MODE_TFT_16BIT          (0 << 7)  /* 16bit TFT */
#define LCD_CFG_PDW_BIT		4  /* STN pins utilization */
#define LCD_CFG_PDW_MASK	(0x3 << LCD_DEV_PDW_BIT)
#define LCD_CFG_PDW_1		(0 << LCD_CFG_PDW_BIT) /* LCD_D[0] */
  #define LCD_CFG_PDW_2		(1 << LCD_CFG_PDW_BIT) /* LCD_D[0:1] */
  #define LCD_CFG_PDW_4		(2 << LCD_CFG_PDW_BIT) /* LCD_D[0:3]/LCD_D[8:11] */
  #define LCD_CFG_PDW_8		(3 << LCD_CFG_PDW_BIT) /* LCD_D[0:7]/LCD_D[8:15] */
#define LCD_CFG_MODE_BIT	0  /* Display Device Mode Select */
#define LCD_CFG_MODE_MASK	(0x0f << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_GENERIC_TFT	(0 << LCD_CFG_MODE_BIT) /* 16,18 bit TFT */
  #define LCD_CFG_MODE_SPECIAL_TFT_1	(1 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SPECIAL_TFT_2	(2 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SPECIAL_TFT_3	(3 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_NONINTER_CCIR656	(4 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_INTER_CCIR656	(6 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SINGLE_CSTN	(8 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SINGLE_MSTN	(9 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_DUAL_CSTN	(10 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_DUAL_MSTN	(11 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SERIAL_TFT	(12 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_LCM  		(13 << LCD_CFG_MODE_BIT)
  /* JZ47XX defines */
  #define LCD_CFG_MODE_SHARP_HR		(1 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_CASIO_TFT	(2 << LCD_CFG_MODE_BIT)
  #define LCD_CFG_MODE_SAMSUNG_ALPHA	(3 << LCD_CFG_MODE_BIT)

/* LCD Control Register */
#define LCD_CTRL_BST_BIT	28  /* Burst Length Selection */
#define LCD_CTRL_BST_MASK	(0x03 << LCD_CTRL_BST_BIT)
  #define LCD_CTRL_BST_4	(0 << LCD_CTRL_BST_BIT) /* 4-word */
  #define LCD_CTRL_BST_8	(1 << LCD_CTRL_BST_BIT) /* 8-word */
  #define LCD_CTRL_BST_16	(2 << LCD_CTRL_BST_BIT) /* 16-word */
  #define LCD_CTRL_BST_32	(3 << LCD_CTRL_BST_BIT) /* 32-word */
#define LCD_CTRL_RGB565		(0 << 27) /* RGB565 mode(foreground 0 in OSD mode) */
#define LCD_CTRL_RGB555		(1 << 27) /* RGB555 mode(foreground 0 in OSD mode) */
#define LCD_CTRL_OFUP		(1 << 26) /* Output FIFO underrun protection enable */
#define LCD_CTRL_FRC_BIT	24  /* STN FRC Algorithm Selection */
#define LCD_CTRL_FRC_MASK	(0x03 << LCD_CTRL_FRC_BIT)
  #define LCD_CTRL_FRC_16	(0 << LCD_CTRL_FRC_BIT) /* 16 grayscale */
  #define LCD_CTRL_FRC_4	(1 << LCD_CTRL_FRC_BIT) /* 4 grayscale */
  #define LCD_CTRL_FRC_2	(2 << LCD_CTRL_FRC_BIT) /* 2 grayscale */
#define LCD_CTRL_PDD_BIT	16  /* Load Palette Delay Counter */
#define LCD_CTRL_PDD_MASK	(0xff << LCD_CTRL_PDD_BIT)
#define LCD_CTRL_EOFM		(1 << 13) /* EOF interrupt mask */
#define LCD_CTRL_SOFM		(1 << 12) /* SOF interrupt mask */
#define LCD_CTRL_OFUM		(1 << 11) /* Output FIFO underrun interrupt mask */
#define LCD_CTRL_IFUM0		(1 << 10) /* Input FIFO 0 underrun interrupt mask */
#define LCD_CTRL_IFUM1		(1 << 9)  /* Input FIFO 1 underrun interrupt mask */
#define LCD_CTRL_LDDM		(1 << 8)  /* LCD disable done interrupt mask */
#define LCD_CTRL_QDM		(1 << 7)  /* LCD quick disable done interrupt mask */
#define LCD_CTRL_BEDN		(1 << 6)  /* Endian selection */
#define LCD_CTRL_PEDN		(1 << 5)  /* Endian in byte:0-msb first, 1-lsb first */
#define LCD_CTRL_DIS		(1 << 4)  /* Disable indicate bit */
#define LCD_CTRL_ENA		(1 << 3)  /* LCD enable bit */
#define LCD_CTRL_BPP_BIT	0  /* Bits Per Pixel */
#define LCD_CTRL_BPP_MASK	(0x07 << LCD_CTRL_BPP_BIT)
  #define LCD_CTRL_BPP_1	(0 << LCD_CTRL_BPP_BIT) /* 1 bpp */
  #define LCD_CTRL_BPP_2	(1 << LCD_CTRL_BPP_BIT) /* 2 bpp */
  #define LCD_CTRL_BPP_4	(2 << LCD_CTRL_BPP_BIT) /* 4 bpp */
  #define LCD_CTRL_BPP_8	(3 << LCD_CTRL_BPP_BIT) /* 8 bpp */
  #define LCD_CTRL_BPP_16	(4 << LCD_CTRL_BPP_BIT) /* 15/16 bpp */
  #define LCD_CTRL_BPP_18_24	(5 << LCD_CTRL_BPP_BIT) /* 18/24/32 bpp */

/* LCD Status Register */
#define LCD_STATE_QD		(1 << 7) /* Quick Disable Done */
#define LCD_STATE_EOF		(1 << 5) /* EOF Flag */
#define LCD_STATE_SOF		(1 << 4) /* SOF Flag */
#define LCD_STATE_OFU		(1 << 3) /* Output FIFO Underrun */
#define LCD_STATE_IFU0		(1 << 2) /* Input FIFO 0 Underrun */
#define LCD_STATE_IFU1		(1 << 1) /* Input FIFO 1 Underrun */
#define LCD_STATE_LDD		(1 << 0) /* LCD Disabled */

/* OSD Configure Register */
#define LCD_OSDC_SOFM1		(1 << 15) /* Start of frame interrupt mask for foreground 1 */
#define LCD_OSDC_EOFM1		(1 << 14) /* End of frame interrupt mask for foreground 1 */
#define LCD_OSDC_REM1		(1 << 13) /* Real end of frame mask for foreground 1 */
#define LCD_OSDC_SOFM0		(1 << 11) /* Start of frame interrupt mask for foreground 0 */
#define LCD_OSDC_EOFM0		(1 << 10) /* End of frame interrupt mask for foreground 0 */
#define LCD_OSDC_REM0		(1 << 9) /* Real end of frame mask for foreground 0 */
#define LCD_OSDC_REMB		(1 << 7) /* Real end of frame mask for background */
#define LCD_OSDC_F1EN		(1 << 4) /* enable foreground 1 */
#define LCD_OSDC_F0EN		(1 << 3) /* enable foreground 0 */
#define LCD_OSDC_ALPHAEN		(1 << 2) /* enable alpha blending */
#define LCD_OSDC_ALPHAMD		(1 << 1) /* alpha blending mode */
#define LCD_OSDC_OSDEN		(1 << 0) /* OSD mode enable */

/* OSD Controll Register */
#define LCD_OSDCTRL_IPU		(1 << 15) /* input data from IPU */
#define LCD_OSDCTRL_RGB565	(0 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCD_OSDCTRL_RGB555	(1 << 4) /* foreground 1, 16bpp, 0-RGB565, 1-RGB555 */
#define LCD_OSDCTRL_CHANGES	(1 << 3) /* Change size flag */
#define LCD_OSDCTRL_OSDBPP_BIT	0 	 /* Bits Per Pixel of OSD Channel 1 */
#define LCD_OSDCTRL_OSDBPP_MASK	(0x3<<LCD_OSDCTRL_OSDBPP_BIT) 	 /* Bits Per Pixel of OSD Channel 1's MASK */
  #define LCD_OSDCTRL_OSDBPP_16	(4 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCD_OSDCTRL_OSDBPP_15_16	(4 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 15,16 bit*/
  #define LCD_OSDCTRL_OSDBPP_18_24	(5 << LCD_OSDCTRL_OSDBPP_BIT) /* RGB 18,24 bit*/

/* OSD State Register */
#define LCD_OSDS_SOF1		(1 << 15) /* Start of frame flag for foreground 1 */
#define LCD_OSDS_EOF1		(1 << 14) /* End of frame flag for foreground 1 */
#define LCD_OSDS_SOF0		(1 << 11) /* Start of frame flag for foreground 0 */
#define LCD_OSDS_EOF0		(1 << 10) /* End of frame flag for foreground 0 */
#define LCD_OSDS_READY		(1 << 0)  /* Read for accept the change */

/* Background Color Register */
#define LCD_BGC_RED_OFFSET	(1 << 16)  /* Red color offset */
#define LCD_BGC_RED_MASK	(0xFF<<LCD_BGC_RED_OFFSET)
#define LCD_BGC_GREEN_OFFSET	(1 << 8)   /* Green color offset */
#define LCD_BGC_GREEN_MASK	(0xFF<<LCD_BGC_GREEN_OFFSET)
#define LCD_BGC_BLUE_OFFSET	(1 << 0)   /* Blue color offset */
#define LCD_BGC_BLUE_MASK	(0xFF<<LCD_BGC_BLUE_OFFSET)

/* Foreground Color Key Register 0,1(foreground 0, foreground 1) */
#define LCD_KEY_KEYEN		(1 << 31)   /* enable color key */
#define LCD_KEY_KEYMD		(1 << 30)   /* color key mode */
#define LCD_KEY_RED_OFFSET	(1 << 16)  /* Red color offset */
#define LCD_KEY_RED_MASK	(0xFF<<LCD_KEY_RED_OFFSET)
#define LCD_KEY_GREEN_OFFSET	(1 << 8)   /* Green color offset */
#define LCD_KEY_GREEN_MASK	(0xFF<<LCD_KEY_GREEN_OFFSET)
#define LCD_KEY_BLUE_OFFSET	(1 << 0)   /* Blue color offset */
#define LCD_KEY_BLUE_MASK	(0xFF<<LCD_KEY_BLUE_OFFSET)

/* IPU Restart Register */
#define LCD_IPUR_IPUREN		(1 << 31)   /* IPU restart function enable*/

/* RGB Control Register */
#define LCD_RGBC_RGBDM		(1 << 15)   /* enable RGB Dummy data */
#define LCD_RGBC_DMM		(1 << 14)   /* RGB Dummy mode */
#define LCD_RGBC_YCC		(1 << 8)    /* RGB to YCC */
#define LCD_RGBC_ODDRGB_BIT	4	/* odd line serial RGB data arrangement */
#define LCD_RGBC_ODDRGB_MASK	(0x7<<LCD_RGBC_ODDRGB_BIT)
  #define LCD_RGBC_ODD_RGB	0
  #define LCD_RGBC_ODD_RBG	1
  #define LCD_RGBC_ODD_GRB	2
  #define LCD_RGBC_ODD_GBR	3
  #define LCD_RGBC_ODD_BRG	4
  #define LCD_RGBC_ODD_BGR	5
#define LCD_RGBC_EVENRGB_BIT	0	/* even line serial RGB data arrangement */
#define LCD_RGBC_EVENRGB_MASK	(0x7<<LCD_RGBC_EVENRGB_BIT)
  #define LCD_RGBC_EVEN_RGB	0
  #define LCD_RGBC_EVEN_RBG	1
  #define LCD_RGBC_EVEN_GRB	2
  #define LCD_RGBC_EVEN_GBR	3
  #define LCD_RGBC_EVEN_BRG	4
  #define LCD_RGBC_EVEN_BGR	5

/* Vertical Synchronize Register */
#define LCD_VSYNC_VPS_BIT	16  /* VSYNC pulse start in line clock, fixed to 0 */
#define LCD_VSYNC_VPS_MASK	(0xffff << LCD_VSYNC_VPS_BIT)
#define LCD_VSYNC_VPE_BIT	0   /* VSYNC pulse end in line clock */
#define LCD_VSYNC_VPE_MASK	(0xffff << LCD_VSYNC_VPS_BIT)

/* Horizontal Synchronize Register */
#define LCD_HSYNC_HPS_BIT	16  /* HSYNC pulse start position in dot clock */
#define LCD_HSYNC_HPS_MASK	(0xffff << LCD_HSYNC_HPS_BIT)
#define LCD_HSYNC_HPE_BIT	0   /* HSYNC pulse end position in dot clock */
#define LCD_HSYNC_HPE_MASK	(0xffff << LCD_HSYNC_HPE_BIT)

/* Virtual Area Setting Register */
#define LCD_VAT_HT_BIT		16  /* Horizontal Total size in dot clock */
#define LCD_VAT_HT_MASK		(0xffff << LCD_VAT_HT_BIT)
#define LCD_VAT_VT_BIT		0   /* Vertical Total size in dot clock */
#define LCD_VAT_VT_MASK		(0xffff << LCD_VAT_VT_BIT)

/* Display Area Horizontal Start/End Point Register */
#define LCD_DAH_HDS_BIT		16  /* Horizontal display area start in dot clock */
#define LCD_DAH_HDS_MASK	(0xffff << LCD_DAH_HDS_BIT)
#define LCD_DAH_HDE_BIT		0   /* Horizontal display area end in dot clock */
#define LCD_DAH_HDE_MASK	(0xffff << LCD_DAH_HDE_BIT)

/* Display Area Vertical Start/End Point Register */
#define LCD_DAV_VDS_BIT		16  /* Vertical display area start in line clock */
#define LCD_DAV_VDS_MASK	(0xffff << LCD_DAV_VDS_BIT)
#define LCD_DAV_VDE_BIT		0   /* Vertical display area end in line clock */
#define LCD_DAV_VDE_MASK	(0xffff << LCD_DAV_VDE_BIT)

/* Foreground XY Position Register */
#define LCD_XYP_YPOS_BIT	16  /* Y position bit of foreground 0 or 1 */
#define LCD_XYP_YPOS_MASK	(0xffff << LCD_XYP_YPOS_BIT)
#define LCD_XYP_XPOS_BIT	0   /* X position bit of foreground 0 or 1 */
#define LCD_XYP_XPOS_MASK	(0xffff << LCD_XYP_XPOS_BIT)

/* PS Signal Setting */
#define LCD_PS_PSS_BIT		16  /* PS signal start position in dot clock */
#define LCD_PS_PSS_MASK		(0xffff << LCD_PS_PSS_BIT)
#define LCD_PS_PSE_BIT		0   /* PS signal end position in dot clock */
#define LCD_PS_PSE_MASK		(0xffff << LCD_PS_PSE_BIT)

/* CLS Signal Setting */
#define LCD_CLS_CLSS_BIT	16  /* CLS signal start position in dot clock */
#define LCD_CLS_CLSS_MASK	(0xffff << LCD_CLS_CLSS_BIT)
#define LCD_CLS_CLSE_BIT	0   /* CLS signal end position in dot clock */
#define LCD_CLS_CLSE_MASK	(0xffff << LCD_CLS_CLSE_BIT)

/* SPL Signal Setting */
#define LCD_SPL_SPLS_BIT	16  /* SPL signal start position in dot clock */
#define LCD_SPL_SPLS_MASK	(0xffff << LCD_SPL_SPLS_BIT)
#define LCD_SPL_SPLE_BIT	0   /* SPL signal end position in dot clock */
#define LCD_SPL_SPLE_MASK	(0xffff << LCD_SPL_SPLE_BIT)

/* REV Signal Setting */
#define LCD_REV_REVS_BIT	16  /* REV signal start position in dot clock */
#define LCD_REV_REVS_MASK	(0xffff << LCD_REV_REVS_BIT)

/* DMA Command Register */
#define LCD_CMD_SOFINT		(1 << 31)
#define LCD_CMD_EOFINT		(1 << 30)
#define LCD_CMD_CMD		(1 << 29) /* indicate command in slcd mode */
#define LCD_CMD_PAL		(1 << 28)
#define LCD_CMD_LEN_BIT		0
#define LCD_CMD_LEN_MASK	(0xffffff << LCD_CMD_LEN_BIT)

/* DMA Offsize Register 0,1 */

/* DMA Page Width Register 0,1 */

/* DMA Command Counter Register 0,1 */

/* Foreground 0,1 Size Register */
#define LCD_DESSIZE_HEIGHT_BIT	16  /* height of foreground 1 */
#define LCD_DESSIZE_HEIGHT_MASK	(0xffff << LCD_DESSIZE_HEIGHT_BIT)
#define LCD_DESSIZE_WIDTH_BIT	0  /* width of foreground 1 */
#define LCD_DESSIZE_WIDTH_MASK	(0xffff << LCD_DESSIZE_WIDTH_BIT)

/*************************************************************************
 * USB Device
 *************************************************************************/
#define USB_BASE  UDC_BASE

#define USB_REG_FADDR		(USB_BASE + 0x00) /* Function Address 8-bit */
#define USB_REG_POWER		(USB_BASE + 0x01) /* Power Managemetn 8-bit */
#define USB_REG_INTRIN		(USB_BASE + 0x02) /* Interrupt IN 16-bit */
#define USB_REG_INTROUT		(USB_BASE + 0x04) /* Interrupt OUT 16-bit */
#define USB_REG_INTRINE		(USB_BASE + 0x06) /* Intr IN enable 16-bit */
#define USB_REG_INTROUTE	(USB_BASE + 0x08) /* Intr OUT enable 16-bit */
#define USB_REG_INTRUSB		(USB_BASE + 0x0a) /* Interrupt USB 8-bit */
#define USB_REG_INTRUSBE	(USB_BASE + 0x0b) /* Interrupt USB Enable 8-bit */
#define USB_REG_FRAME		(USB_BASE + 0x0c) /* Frame number 16-bit */
#define USB_REG_INDEX		(USB_BASE + 0x0e) /* Index register 8-bit */
#define USB_REG_TESTMODE	(USB_BASE + 0x0f) /* USB test mode 8-bit */

#define USB_REG_CSR0		(USB_BASE + 0x12) /* EP0 CSR 8-bit */
#define USB_REG_INMAXP		(USB_BASE + 0x10) /* EP1-2 IN Max Pkt Size 16-bit */
#define USB_REG_INCSR		(USB_BASE + 0x12) /* EP1-2 IN CSR LSB 8/16bit */
#define USB_REG_INCSRH		(USB_BASE + 0x13) /* EP1-2 IN CSR MSB 8-bit */
#define USB_REG_OUTMAXP		(USB_BASE + 0x14) /* EP1 OUT Max Pkt Size 16-bit */
#define USB_REG_OUTCSR		(USB_BASE + 0x16) /* EP1 OUT CSR LSB 8/16bit */
#define USB_REG_OUTCSRH		(USB_BASE + 0x17) /* EP1 OUT CSR MSB 8-bit */
#define USB_REG_OUTCOUNT	(USB_BASE + 0x18) /* bytes in EP0/1 OUT FIFO 16-bit */

#define USB_FIFO_EP0		(USB_BASE + 0x20)
#define USB_FIFO_EP1		(USB_BASE + 0x24)
#define USB_FIFO_EP2		(USB_BASE + 0x28)

#define USB_REG_EPINFO		(USB_BASE + 0x78) /* Endpoint information */
#define USB_REG_RAMINFO		(USB_BASE + 0x79) /* RAM information */

#define USB_REG_INTR		(USB_BASE + 0x200) /* DMA pending interrupts */
#define USB_REG_CNTL1		(USB_BASE + 0x204) /* DMA channel 1 control */
#define USB_REG_ADDR1		(USB_BASE + 0x208) /* DMA channel 1 AHB memory addr */
#define USB_REG_COUNT1		(USB_BASE + 0x20c) /* DMA channel 1 byte count */
#define USB_REG_CNTL2		(USB_BASE + 0x214) /* DMA channel 2 control */
#define USB_REG_ADDR2		(USB_BASE + 0x218) /* DMA channel 2 AHB memory addr */
#define USB_REG_COUNT2		(USB_BASE + 0x21c) /* DMA channel 2 byte count */


/* Power register bit masks */
#define USB_POWER_SUSPENDM	0x01
#define USB_POWER_RESUME	0x04
#define USB_POWER_HSMODE	0x10
#define USB_POWER_HSENAB	0x20
#define USB_POWER_SOFTCONN	0x40

/* Interrupt register bit masks */
#define USB_INTR_SUSPEND	0x01
#define USB_INTR_RESUME		0x02
#define USB_INTR_RESET		0x04

#define USB_INTR_EP0		0x0001
#define USB_INTR_INEP1		0x0002
#define USB_INTR_INEP2		0x0004
#define USB_INTR_OUTEP1		0x0002

/* CSR0 bit masks */
#define USB_CSR0_OUTPKTRDY	0x01
#define USB_CSR0_INPKTRDY	0x02
#define USB_CSR0_SENTSTALL	0x04
#define USB_CSR0_DATAEND	0x08
#define USB_CSR0_SETUPEND	0x10
#define USB_CSR0_SENDSTALL	0x20
#define USB_CSR0_SVDOUTPKTRDY	0x40
#define USB_CSR0_SVDSETUPEND	0x80

/* Endpoint CSR register bits */
#define USB_INCSRH_AUTOSET	0x80
#define USB_INCSRH_ISO		0x40
#define USB_INCSRH_MODE		0x20
#define USB_INCSRH_DMAREQENAB	0x10
#define USB_INCSRH_DMAREQMODE	0x04
#define USB_INCSR_CDT		0x40
#define USB_INCSR_SENTSTALL	0x20
#define USB_INCSR_SENDSTALL	0x10
#define USB_INCSR_FF		0x08
#define USB_INCSR_UNDERRUN	0x04
#define USB_INCSR_FFNOTEMPT	0x02
#define USB_INCSR_INPKTRDY	0x01
#define USB_OUTCSRH_AUTOCLR	0x80
#define USB_OUTCSRH_ISO		0x40
#define USB_OUTCSRH_DMAREQENAB	0x20
#define USB_OUTCSRH_DNYT	0x10
#define USB_OUTCSRH_DMAREQMODE	0x08
#define USB_OUTCSR_CDT		0x80
#define USB_OUTCSR_SENTSTALL	0x40
#define USB_OUTCSR_SENDSTALL	0x20
#define USB_OUTCSR_FF		0x10
#define USB_OUTCSR_DATAERR	0x08
#define USB_OUTCSR_OVERRUN	0x04
#define USB_OUTCSR_FFFULL	0x02
#define USB_OUTCSR_OUTPKTRDY	0x01

/* Testmode register bits */
#define USB_TEST_SE0NAK		0x01
#define USB_TEST_J		0x02
#define USB_TEST_K		0x04
#define USB_TEST_PACKET		0x08

/* DMA control bits */
#define USB_CNTL_ENA		0x01
#define USB_CNTL_DIR_IN		0x02
#define USB_CNTL_MODE_1		0x04
#define USB_CNTL_INTR_EN	0x08
#define USB_CNTL_EP(n)		((n) << 4)
#define USB_CNTL_BURST_0	(0 << 9)
#define USB_CNTL_BURST_4	(1 << 9)
#define USB_CNTL_BURST_8	(2 << 9)
#define USB_CNTL_BURST_16	(3 << 9)

/*************************************************************************
 * BCH
 *************************************************************************/
#define	BCH_CR         	(BCH_BASE + 0x00) /* BCH Control register */
#define	BCH_CRS       	(BCH_BASE + 0x04) /* BCH Control Set register */
#define	BCH_CRC       	(BCH_BASE + 0x08) /* BCH Control Clear register */
#define	BCH_CNT    	(BCH_BASE + 0x0C) /* BCH ENC/DEC Count register */
#define	BCH_DR     	(BCH_BASE + 0x10) /* BCH data register */
#define	BCH_PAR0    	(BCH_BASE + 0x14) /* BCH Parity 0 register */
#define	BCH_PAR1    	(BCH_BASE + 0x18) /* BCH Parity 1 register */
#define	BCH_PAR2    	(BCH_BASE + 0x1C) /* BCH Parity 2 register */
#define	BCH_PAR3    	(BCH_BASE + 0x20) /* BCH Parity 3 register */
#define	BCH_INTS    	(BCH_BASE + 0x24) /* BCH Interrupt Status register */
#define	BCH_ERR0        (BCH_BASE + 0x28) /* BCH Error Report 0 register */
#define	BCH_ERR1        (BCH_BASE + 0x2C) /* BCH Error Report 1 register */
#define	BCH_ERR2        (BCH_BASE + 0x30) /* BCH Error Report 2 register */
#define	BCH_ERR3        (BCH_BASE + 0x34) /* BCH Error Report 3 register */
#define	BCH_INTE        (BCH_BASE + 0x38) /* BCH Interrupt Enable register */
#define	BCH_INTES       (BCH_BASE + 0x3C) /* BCH Interrupt Set register */
#define	BCH_INTEC       (BCH_BASE + 0x40) /* BCH Interrupt Clear register */

#define	REG_BCH_CR      REG32(BCH_CR)
#define	REG_BCH_CRS     REG32(BCH_CRS)
#define	REG_BCH_CRC     REG32(BCH_CRC)
#define	REG_BCH_CNT     REG32(BCH_CNT)
#define	REG_BCH_DR      REG8(BCH_DR)
#define	REG_BCH_PAR0    REG32(BCH_PAR0)
#define	REG_BCH_PAR1    REG32(BCH_PAR1)
#define	REG_BCH_PAR2    REG32(BCH_PAR2)
#define	REG_BCH_PAR3    REG32(BCH_PAR3)
#define	REG_BCH_INTS    REG32(BCH_INTS)
#define	REG_BCH_ERR0    REG32(BCH_ERR0)
#define	REG_BCH_ERR1    REG32(BCH_ERR1)
#define	REG_BCH_ERR2    REG32(BCH_ERR2)
#define	REG_BCH_ERR3    REG32(BCH_ERR3)
#define	REG_BCH_INTE    REG32(BCH_INTE)
#define	REG_BCH_INTEC   REG32(BCH_INTEC)
#define	REG_BCH_INTES   REG32(BCH_INTES)

/* BCH Control Register*/
#define	BCH_CR_DMAE              (1 << 4)  /* BCH DMA Enable */
#define	BCH_CR_ENCE              (1 << 3)  /* BCH Encoding Select */
#define	BCH_CR_DECE              (0 << 3)  /* BCH Decoding Select */
#define	BCH_CR_BSEL8             (1 << 2)  /* 8 Bit BCH Select */
#define	BCH_CR_BSEL4             (0 << 2)  /* 4 Bit BCH Select */
#define	BCH_CR_BRST              (1 << 1)  /* BCH Reset */
#define	BCH_CR_BCHE              (1 << 0)  /* BCH Enable */

/* BCH Interrupt Status Register */
#define	BCH_INTS_ERRC_BIT        28
#define	BCH_INTS_ERRC_MASK       (0xf << BCH_INTS_ERRC_BIT)
#define	BCH_INTS_ALL0            (1 << 5)
#define	BCH_INTS_ALLf            (1 << 4)
#define	BCH_INTS_DECF            (1 << 3)
#define	BCH_INTS_ENCF            (1 << 2)
#define	BCH_INTS_UNCOR           (1 << 1)
#define	BCH_INTS_ERR             (1 << 0)

/* BCH ENC/DEC Count Register */
#define BCH_CNT_DEC_BIT          16
#define BCH_CNT_DEC_MASK         (0x3ff << BCH_CNT_DEC_BIT)
#define BCH_CNT_ENC_BIT          0
#define BCH_CNT_ENC_MASK         (0x3ff << BCH_CNT_ENC_BIT)

/* BCH Error Report Register */
#define BCH_ERR_INDEX_ODD_BIT    16
#define BCH_ERR_INDEX_ODD_MASK   (0x1fff << BCH_ERR_INDEX_ODD_BIT)
#define BCH_ERR_INDEX_EVEN_BIT   0
#define BCH_ERR_INDEX_EVEN_MASK  (0x1fff << BCH_ERR_INDEX_EVEN_BIT)

/*************************************************************************
 * OWI (One-wire Bus Controller )
 *************************************************************************/
#define OWI_CFG (OWI_BASE + 0x00) /* OWI Configure Register */
#define OWI_CTL (OWI_BASE + 0x04) /* OWI Control Register */
#define OWI_STS (OWI_BASE + 0x08) /* OWI Status Register */
#define OWI_DAT (OWI_BASE + 0x0c) /* OWI Data Register */
#define OWI_DIV (OWI_BASE + 0x10) /* OWI Clock Divide Register */

#define REG_OWI_CFG  REG8(OWI_CFG)
#define REG_OWI_CTL  REG8(OWI_CTL)
#define REG_OWI_STS  REG8(OWI_STS)
#define REG_OWI_DAT  REG8(OWI_DAT)
#define REG_OWI_DIV  REG8(OWI_DIV)

/* OWI Configure Register */
#define OWI_CFG_MODE      (1 << 7) /*  0: Regular speed mode  1: Overdrive  speed mode */
#define OWI_CFG_RDDATA    (1 << 6) /* 1: receive data from one-wire bus and stored in OWDAT*/
#define OWI_CFG_WRDATA    (1 << 5) /* 1: transmit the data in OWDAT */
#define OWI_CFG_RDST      (1 << 4) /* 1: was sampled during a read */
#define OWI_CFG_WR1RD     (1 << 3) /* 1: generate write 1 sequence on line */
#define OWI_CFG_WR0       (1 << 2) /* 1: generate write 0 sequence on line */
#define OWI_CFG_RST       (1 << 1) /* 1: generate reset pulse and sample slaves presence pulse*/
#define OWI_CFG_ENA       (1 << 0) /* 1: enable the OWI operation */

/* OWI Control Register */
#define OWI_CTL_EBYTE     (1 << 2) /* enable byte write/read interrupt */
#define OWI_CTL_EBIT      (1 << 1) /* enable bit write/read interrupt */
#define OWI_CTL_ERST      (1 << 0) /* enable reset sequence finished interrupt */

/* OWI Status Register */
#define OWI_STS_PST       (1 << 7) /* 1: one-wire bus has device on it */
#define OWI_STS_BYTE_RDY  (1 << 2) /* 1: have received or transmitted a data */
#define OWI_STS_BIT_RDY   (1 << 1) /* 1: have received or transmitted a bit */
#define OWI_STS_PST_RDY   (1 << 0) /* 1: have finished a reset pulse */

/* OWI Clock Divide Register */
#define OWI_DIV_CLKDIV_BIT  5


#endif /* __JZ4750_REGS_H__ */
