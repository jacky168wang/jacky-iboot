
#if 0
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"


	.text
	.set noreorder
	.set mips32

/*==============================================================================
*  ---- NO stack ----
*  input: NONE
*   used: t0:  SYS_MEM_BASE ~ SYS_MEM_TEST_SIZE
*         t2: pattern (1<<0)~(1<<31) to be written into
*         t3: acutal data value which has been read out
* return: s0: 0 - success
*
LEAF()
ENDF()
*/

#if !defined(UARTBOOT_CODE)
/* (non-NOR)Bootrom boot: 
 *     default  9600 when AMC_DATA[2:0]=011
 *     default 38400 when AMC_DATA[2:0]=111
 * there is one command to change baudrate, so the second stage program
 * could read R
 * > u <divisor>               : Set UART divisor
*/
//#define UART_BAUD 38400
//#define UART_BAUD 115200

LEAF(a_uart_init)
    la t0, UART1_BASE
    li t1, (UART_LCR_DLAB_1 | UART_LCR_WLS_8) // set DLAB to Program Divisor
    sw t1, 0xc(s0)

    li t1, 0x0
    sw t1, 4(t0)
#if UART_BAUD == 115200
    li t1, 0x07 //115200 baudrate for 12.5 MHz system clock
#elif UART_BAUD == 38400
    li t1, 0x14 //38400 baudrate for 12.5 MHz system clock
#else
    li t1, 0x51 //9600 baudrate for 12.5 MHz system clock 
#endif
    sw t1, 0(t0) // Divisor 1

    li t1, UART_LCR_WLS_8 // clear DLAB to Program Divisor
    sw t1, 0xc(t0)
    j  ra
ENDF(a_uart_init)
#endif

LEAF(a_put_char)
    la    t0, UART1_BASE
1:  lw    t1, UART_LSR_OFFSET(t0)
    and   t1, t1, 0x20
    beq   t1, zero, 1b
    nop   /* time slot */
    sw    a0, 0(t0)
    j     ra
ENDF(a_put_char)

/* a0 - string */
NON_LEAF(a_put_str)
    li    t0, UART1_BASE
0:  lb    t1, 0(a0)
    beq   t1, zero, 2f
    addiu a2, 1
1:  lw    a1, UART_LSR_OFFSET(t0)
    and   a1, a1, 0x20
    beq   a1, zero, 1b
    nop   /* time slot */
    b     82b
    sw    a3, 0(a0)
    ssnop     /* optimize ruby prefetch */
    ssnop
    ssnop
    ssnop
    ssnop
    ssnop
    ssnop
    ssnop
2:  j     ra

#endif


