/*******************************************************************************
 * Description   : CSRs for Uart0/Uart1 Controllers
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_UART_H_
#define _REG_UART_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define UART_BASE  (0xB9010000)
#define UART1_BASE (UART_BASE)
#define UART2_BASE (UART_BASE + 0x8000)

#define UART_LSR_OFFSET (0x14)
#define UART_THR_OFFSET (0x00)

#define UART1_RBR               (UART1_BASE + 0x00)
#define UART1_THR               (UART1_BASE + 0x00)
#define UART1_DLL               (UART1_BASE + 0x00)
#define UART1_DLH               (UART1_BASE + 0x04)
#define UART1_IER               (UART1_BASE + 0x04)
#define UART1_IIR               (UART1_BASE + 0x08)
#define UART1_LCR               (UART1_BASE + 0x0C)
#define UART1_MCR               (UART1_BASE + 0x10)
#define UART1_LSR               (UART1_BASE + 0x14)
#define UART1_MSR               (UART1_BASE + 0x18)
#define UART1_SCR               (UART1_BASE + 0x1C)

#define UART1_RFCR2             (UART1_BASE + 0x20)
#define UART1_RF_CHTOUT2        (UART1_BASE + 0x24)
#define UART1_RFSTAT2           (UART1_BASE + 0x28)
#define UART1_TFCR2             (UART1_BASE + 0x2C)
#define UART1_TFSTAT2           (UART1_BASE + 0x30)
#define UART1_FLOW_CTRL2        (UART1_BASE + 0x34)

#ifndef __ASSEMBLY__

typedef struct {
    union {
        H_D32 thr;
        H_D32 rbr;
        H_D32 dll;
    } u0; //0x00
    union {
        H_D32 ier;
        H_D32 dlh;
    } u1; //0x04
    H_D32 iir; //0x08
    H_D32 lcr; //0x0c
    H_D32 mcr; //0x10
    H_D32 lsr; //0x14
    H_D32 msr; //0x18
    H_D32 scr; //0x1c
} uart_reg_t;

#define UART1 ((uart_reg_t *)UART1_BASE)
#define UART2 ((uart_reg_t *)UART2_BASE)

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

/* 0x1901_0000/0x1901_8000, 0x1901_0004/0x1901_8004
--------------------------------------------------------------------------------
    THR 7:0   W Transmit Holding Registers, 0x00
    RBR 7:0 R/W Receive  Buffer  Registers, 0x00
    DLL 7:0 R/W Divisor Latch LowRegisters, 0x00
--------------------------------------------------------------------------------
    DLH 7:0 R/W Divisor Latch HighRegister, 0x00
    IER 7:0 R/W Interrupt Enable Register,  0x00
     ERBFI 0 R/W 1: Enables receive buffer full interrupt
     ETBEI 1 R/W 1: Enable transmit buffer empty interrupt
      ERSI 2 R/W 1: Enable Rx status of interrupt.generated when LSR[3:0] are set
Reserved 7:3 R   Reserved
*/
#define  IER_ERBFI              0
#define  IER_ETBEI              1
#define  IER_ERSI               2
#define  IER_ECHTOUTI           3

/* 0x1901_0008/0x1901_8008, Interrupt Identification Register, 0x00
--------------------------------------------------------------------------------
    NINT   0 R 1: No interrupt, 0: See Table 4-13.16
  STATUS 2:1 R Interrupt status. See Table 4-13.16.
Reserved 7:3 R Reserved
*/

#define  IIR_NINT               0
#define  IIR_STATUS             1
#define  IIR_CHTOUT             3

/* 0x1901_000C/0x1901_800C, Line Control Register, 0x00
--------------------------------------------------------------------------------
*/
#define  LCR_WLS                0
#define  LCR_STB                2
#define  LCR_PEN                3
#define  LCR_EPS                4
#define  LCR_SP                 5
#define  LCR_SB                 6
#define  LCR_DLAB               7

#define UART_LCR_WLS_7     (0x02)
#define UART_LCR_WLS_8     (0x03)
#define UART_LCR_STB_1     (0x00)
#define UART_LCR_STB_2     (0x04)
#define UART_LCR_PEN_0     (0x00)
#define UART_LCR_PEN_1     (0x08)
#define UART_LCR_EPS_O     (0x00)
#define UART_LCR_EPS_E     (0x10)
#define UART_LCR_SP_0      (0x00)
#define UART_LCR_SP_1      (0x20)
#define UART_LCR_SB_0      (0x00)
#define UART_LCR_SB_1      (0x40)
#define UART_LCR_DLAB_0    (0x00)
#define UART_LCR_DLAB_1    (0x80)

/* 0x1901_0010/0x1901_8010, Modem Control Register, 0x00
--------------------------------------------------------------------------------
*/
#define  MCR_DTR                0
#define  MCR_RTS                1
#define  MCR_OUT2               2
#define  MCR_OUT1               3
#define  MCR_LOOPBACK           4

/* 0x1901_0014/0x1901_8014, Line Status Register, 0x78
--------------------------------------------------------------------------------
*/
#define  LSR_DR                 0
#define  LSR_OE                 1
#define  LSR_PE                 2
#define  LSR_FE                 3
#define  LSR_BI                 4
#define  LSR_THRE               5
#define  LSR_TEMT               6
#define  LSR_RX_FIFO_ERROR      7

#define UART_LSR_BITMASK_DR          (1 << 0)
#define UART_LSR_BITMASK_BI          (1 << 4)
#define UART_LSR_BITMASK_THRE        (1 << 5)

/* 0x1901_0018/0x1901_8018, Modem Status Register, 0x00
--------------------------------------------------------------------------------
*/
#define  MSR_DCTS               0
#define  MSR_DDSR               1
#define  MSR_TERI               2
#define  MSR_DDCD               3
#define  MSR_CTS                4
#define  MSR_DSR                5
#define  MSR_RI                 6
#define  MSR_DCD                7

/* 0x1901_000C/0x1901_800C, Scratch Register, 0x00
--------------------------------------------------------------------------------
*/

#endif /* _REG_UART_H_ */
