/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
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

/*------------------------------------------------------------------------------
    char *SoC_str[] = { "Vx185", "Vx175", "Vx183", "Vx173", \
        "Vx185SE", "N/A", "Vx185HP", "N/A" };
   000: 185   - 500 MHz
   001: 175   - 500 MHz
   010: 183   - 400 MHz
   011: 173   - 400 MHz
   100: 185SE - 500 MHz
   101: N/A
   110: 185HP - 400 MHz
   111: N/A
*/
#if 0 //host_mips_is_500M
    li    t0, SCU_BASE
    lw    s4, 0x000C(t0)
    srl   s4, s4, 4
    andi  s4, s4, 0x7
    andi  s5, s4, 0x2 /* bit05 in cpu_ctl */
    
    lw    t3, 0x00EC(t0) /* read chip revison */
    sra   s6, t3, 24  /* bit24: 0/1 - b0, b1 */
#endif

/*------------------------------------------------------------------------------
    char *reset_str[] = { "power", "soft", "watchdog", "pll_drift" };
*/
/*
void detect_resetcause(void)
{
    UINT32 i;
    for (i=0;i<4;i++)
    {
        if ((1 << i) & SCU->rst_cause)
            c_put_str(reset_str[i]);
    }
}*/
detect_resetcause:
    /* return
    */
    jr	  ra
    nop   /* time slot */

/*------------------------------------------------------------------------------
*/
/*
void detect_bootmode(void)
{
    UINT32 bootmode;
    char *boot_str[] = { "NOR", "SPI", "NAND", "UART1_9600", 
                         "NOR_fix", "SPI_fix", "NAND_fix", "UART1_38400" };
    bootmode = SCU->rst_strp & 7;
    c_put_str(boot_str[bootmode]);
}*/
detect_bootmode:
    /* return
    */
    jr	  ra
    nop   /* time slot */

/*------------------------------------------------------------------------------
*/
softreset_SoC:
	la t0, SCU_BASE

	li t2, 0
	sw t2, 0x0000(t0)
	sw t2, 0x0004(t0)
    /* return
    */
    jr	  ra
    nop   /* time slot */

