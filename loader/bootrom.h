/*******************************************************************************
*                                                                        
*                             Vx185 Bootrom
*              Copyright (c) Ikanos Communications Inc. 2009              
*                                                                        
*   All rights are reserved.  Reproduction in whole or in part is prohibited
*   without the written consent of the copyright owner.
*
*   The information and contents of this file are the proprietary information
*   of Ikanos Communication and may not be disclosed or used without the
*   formal written approval of Ikanos Communication Inc.
*
*   This Copyright notice may not be removed or modified without prior
*   written consent of Ikanos Communications, Inc.
*
*******************************************************************************/
#ifndef _BOOTROM_H_
#define _BOOTROM_H_

/* Core clock value needed so that Xmodem code can implement protocol
*  timeouts.
*/
#ifdef VX183
    #define POST_RESET_CORE_CLK (200000000)
#elif defined(VX185_A1) || defined(VX185_B0)
    #define POST_RESET_CORE_CLK (250000000)
#endif

#define BIT_FIELD(val, x, y) (((val) >> (x)) & ((1 << (y)) - 1))

#define SYSCTRL_BASE                0xB9000000
#define SYSCTRL_RESET_MASK_OFFSET       0
#define SYSCTRL_NRESET_OFFSET           4
#define SYSCTRL_NRESET_CLASSIFIER_AP_BIT    (1 << 7)
#define SYSCTRL_NRESET_CLASSIFIER_BIT       (1 << 21)
#define SYSCTRL_CLKGATE_OFFSET          0x1C
#define SYSCTRL_CLKGATE_CLASSIFIER_BIT      (1 << 7)
#define SYSCTRL_CONTROL     (SYSCTRL_BASE + SYSCTRL_CONTROL_OFFSET)
#define SYSCTRL_CONTROL_OFFSET          0x0C
#define SYSCTRL_CLKGATE             0xB900001C
#define SYSCTRL_NANDC_STATUS_OFFSET     0xBC
#define SYSCTRL_NANDC_STATUS    (SYSCTRL_BASE + SYSCTRL_NANDC_STATUS_OFFSET)
#define SYSCTRL_NANDC_STATUS_CHIP_BUSY      (1 << 0)
#define SYSCTRL_NANDC_STATUS_PECC_BUSY      (1 << 1)
#define SYSCTRL_SPARE4              0xB90000FC

/* Get a kseg0 address from a kseg1 address */
#define KSEG0(x) (void *)((unsigned long)(x) & 0xdfffffff)

#define BMODE2          (1 << 2)
#define SPI_BMODE       (1)
#define NAND_BMODE      (2)
#define SERIAL_BMODE    (3)

#define GPIO_BASEADDR   0xB9030000
#define GPIO_RDREG(a)   H_RD32(GPIO_BASEADDR+(a))
#define GPIO_WRREG(a,d) H_WR32(GPIO_BASEADDR+(a), (d))
#define GPIO_MODE1      0x0c
#define GPIO_MODE2      0x10
#define GPIO_ALTFN_SEL  0x14
#define GPIO_ALTFN_DEF  0x18
#define GPIO_MODE_OUTPUT    0x1

/* Based on 9600 baud @ UART clock of 12.5MHz.
*  Required divisor = 12.5e6 / (16 * 9600) = 81.38,
*  Actual divisor = 81, actual baud rate = 9645, error = 0.5%.
*  Divisor can be changed with the monitor 'u' command after boot if required.
*/
#define UART_DEFAULT_DIVISOR_LOSPEED    81

/* Based on 38400 baud @ UART clock of 12.5MHz.
*  Required divisor = 12.5e6 / (16 * 38400) = 20.35,
*  Actual divisor = 20, actual baud rate = 39060, error = 1.7%.
*  Divisor can be changed with the monitor 'u' command after boot if required.
*/
#define UART_DEFAULT_DIVISOR_HISPEED    20

#ifndef __ASSEMBLY__

extern void nand_boot(unsigned long start_addr);
extern unsigned long serial_boot(unsigned long download_address);
extern void spi_boot(unsigned long start_addr);

extern void __attribute__ ((nomips16)) set_mips_cp0_count_reg(long val);
extern void __attribute__ ((nomips16)) enable_mips_cp0_count(void);
extern unsigned long __attribute__ ((nomips16)) get_mips_cp0_count_reg(void);

#define USE_ICACHE __attribute((section(".ctext")))
#define LONGCALL __attribute((longcall))

#endif /* __ASSEMBLY__ */

#endif /* _BOOTROM_H_ */
