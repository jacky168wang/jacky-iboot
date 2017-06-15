/*******************************************************************************
 * Description   :
 *
 * DO-438995-TM-1-Vx185 Silicon Details
 *
 *              /--  12M USB
 *     /- pll0 -|-- 125M GMII                  /-- pll1 -- 35.328M VCXO -- IFE7
 *     |        \-- 250M SynthesizerReference -|                            |
 *     |                                       \-- RMII 25/50/125M          |
 *     |                                                                    |
 *     |        /-- 600/250M -- clk_switch -- host_MIPS, system/AHB, MAC    |
 *     |        |-- 500M all APs (0.5 x System)     |                       |
 *     |- pll2 -|-- 500/250M DDR (1 or 2 x System)  \-- RS232/UART          |
 * 25M |        |--  80M SPI                                                |
 * OSC |        \--  40M SPI slow                                          /
 *     |                                                             AFE_RXCLK_I 
 *     |        /-- 500M BME_MIPS -- (2M~10M)4.4M AFE_SCK,AFE_SDA         |
 *     |- pll3 -|                    control 2.2M AFE_CSi          AFE_TXCLK70_O
 *     |        \-- 214M Frequence Domain
 *     |
 *     |        /-- 300M DSP   --> 2.048M or 4.096M PCM/SPORT (<-- External)
 *     \- pll4 -|
 *              \-- 104M SPI fast
 *
 * host/BME/APs clock: 25M *    1*(feedback divider in 0x1900_002C)/(FD1*FD2)
 *  SYStem(AHB) clock: 25M *  0.5*(feedback divider in 0x1900_002C)/(FD1*FD2)
 *          DSP clock: 25M *10.01*(feedback divider in 0x1900_002C)/(FD1*FD2)
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
*   used: t0: DDR_CTL_BASE
*         t2: data to be written into
*         t3: data which has been read out
* return: s0: 0 - success
*/
LEAF(init_pll)
    la    t0, SCU_BASE

    /* Program the BME window for 256 MB flash at (256-8)=248 MB */
    /* BME Window_1 Base register to used to convey the BME window start information */
    li    t2, 0xAF800000
    sw    t2, 0x0054(t0)

    /* Configure GMII2 bit to 01 */
    li    t2, ~0x00030100
    sw    t2, 0x0008(t0)

    li    t2, 0x00020100
    sw    t2, 0x000C(t0)

    li    t2, 0xffffffff
    sw    t2, 0x0008(t0)

    /* Enable clocks and bring modules out of reset */
    li    t2, 0xc7dcf370
    sw    t2, 0x0000(t0)

    li    t2, 0x08fffffc //bit #28 0-25MHz, 1-35.328MHz
    sw    t2, 0x001C(t0)

    li    t2, 0x18230c8f
    sw    t2, 0x0004(t0)
	
  	/* Disabling Internal POR */
   	li    t2, 0x00000020 /* BZ:23348 Changing Internal POR threshold to 20% */
	sw    t2, 0x00c0(t0)

    /* Check if the Reset cause is SOFT RESET */
    /* If Soft Reset, Skip the PLL recalibration */
    lw t3, 0x10(t0)
    andi t3, t3, 0x2
    bnez t3, PLL_Calib_Done
    nop   /* time slot */
    
    host_mips_is_500M()

    beqz  s4, label_host_500M
    nop   /* time slot */
    
label_host_400M:
    li t2, 0x180b6060 /* 0x18096050, [23:16] -> DSP PLL DIV128 */ 
    sw t2, 0x002C(t0)

    li t2, 0x06063033 /* [27:24] -> DSP DIV8, [29:28] -> DSP PD2 */
    sw t2, 0x0030(t0)
    
    b   cfg_pll0_fixed
    nop   /* time slot */

label_host_500M:
    /* FB dividers: last 2 hex, 'h60 = 'd96 (default) */
    li t2, 0x180b6050 /* 0x18096050, [23:16] -> DSP PLL DIV128 */ 
    sw t2, 0x002C(t0)

    li t2, 0x06063022 /* [27:24] -> DSP DIV8, [29:28] -> DSP PD2 */
    sw t2, 0x0030(t0)

cfg_pll0_fixed:
    /* nom fixed PLL0 VCO speed (11111, default) */
    li t2, 0x003fc442
    sw t2, 0x0038(t0)

    /* nom VCXO PLL1  (01100, default) w/gain boost */
#if (CLK_SE_MODE == 1)
    li t2, 0x131cca88
#else
    li t2, 0x001cca88
#endif
    sw t2, 0x003C(t0)

    /* nom VCO speed (01100, default) for gateway pll */
    li t2, 0x002cc642
    sw t2, 0x0040(t0)

    /* nom VCO speed (01100, default) for bme pll */
    li t2, 0x002cc642
    sw t2, 0x0044(t0)

    /* nom VCO speed (00110, default) for dsp pll */
    li t2, 0x0026ce20
    sw t2, 0x0048(t0)
    
    /* Program sigma deltas.  DSP sigma delta control */
    //li t2, 0x003e2d62 //for 294.912 MHz
    li t2, 0x00012b52 //for 229.376 MHz with 25 MHz Ref
    //li t2, 0x001c9f73 //for 212.992 MHz
    sw t2, 0x0028(t0)

    /* VCXO sigma delta control */
    li t2, 0x001654d6
    sw t2, 0x0024(t0)

    /* All clocks on (sigma delta disabled for gateway and bme) */
    li t2, 0x4d600fff
    sw t2, 0x0034(t0)

	/* Fixed PLL calibration */
	lw    t2, 0x0020(t0)
	and   t2, t2, ~0x3f
	ori   t2, t2,  0x07
	sw    t2, 0x0020(t0)

    /* Check for calibration done */
1:  lw t3, 0x00C8(t0)
    andi t3, t3, 0x1
    beqz t3, 1b
    nop   /* time slot */
    a_poll_until_set(0xB900, 0x00C8, PLL_STAT_CALIBRATION_DONE)

    li t3, ~0xf
    lw t2, 0x0020(t0)
    and t2, t2, t3
    ori t2, t2, 0x1
    sw t2, 0x0020(t0)

    a_poll_until_set(0xB900, 0x00C8, PLL_STAT_LOCKED)

    /* Check whether to enable Common Clock or not */
    lw t3, 0x0014(t0)
    andi t3, t3, 0x80
    beqz t3, Common_Clock_Enable
    nop   /* time slot */

    /* Synth PLL */
    li t3, 0xFFFFF00F
    lw t2, 0x20(t0)
    and t2, t2, t3
    ori t2, t2, 0x1C0
    sw t2, 0x20(t0)

1:  lw t3, 0xCC(t0)
    andi t3, t3, 0x1
    beqz t3, 1b
    nop   /* time slot */

    li t3, 0xFFFFF00F
    lw t2, 0x20(t0)
    and t2, t2, t3
    ori t2, t2, 0x040
    sw t2, 0x20(t0)

1:  lw t3, 0xCC(t0)
    andi t3, t3, 0x200
    beqz t3, 1b
    nop   /* time slot */

    b Synth_PLL_Prog_Done
    nop   /* time slot */
Common_Clock_Enable:
    /* Enable VCXO for Common Clock */
    lw t2, 0x8(t0)
    li t3, 0xffdfffff
    and t2, t2, t3
    sw t2, 0x8(t0)

    lw t2, 0xC(t0)
    li t3, 0x200000
    or t2, t2, t3
    sw t2, 0xC(t0)

    lw t2, 0x8(t0)
    or t2, t2, t3
    sw t2, 0x8(t0)

    /* Power up the synth PLL */
    li t3, 0xFFFFF00F
    lw t2, 0x20(t0)
    and t2, t2, t3
    ori t2, t2, 0x2C0
    sw t2, 0x20(t0)

Synth_PLL_Prog_Done:
    /* BME PLL */
    li t3, 0xFF03FFFF
    lw t2, 0x20(t0)
    and t2, t2, t3
    li t3, 0x003C0000
    or t2, t2, t3
    sw t2, 0x20(t0)

1:  lw t3, 0xd4(t0)
    andi t3, t3, 0x1
    beqz t3, 1b
    nop   /* time slot */

   	li t3, 0xFF03FFFF
   	lw t2, 0x20(t0)
   	and t2, t2, t3
   	li t3, 0x00640000
   	or t2, t2, t3
   	sw t2, 0x20(t0)
1: 	lw t3, 0xd4(t0)
   	andi t3, t3, 0x200
   	beqz t3, 1b
    nop   /* time slot */

   	/* Gateway PLL */
   	li t3, 0xFFFE0FFF
   	lw t2, 0x20(t0)
   	and t2, t2, t3
   	li t3, 0x0001F000
   	or t2, t2, t3
   	sw t2, 0x20(t0)

1:  lw t3, 0xd0(t0)
    andi t3, t3, 0x1
    beqz t3, 1b
    nop   /* time slot */

    li t3, 0xFFFE0FFF
    lw t2, 0x20(t0)
    and t2, t2, t3
    li t3, 0x00019000
    or t2, t2, t3
    sw t2, 0x20(t0)

1:  lw t3, 0xd0(t0)
    andi t3, t3, 0x200
    beqz t3, 1b
    nop   /* time slot */

    /* DSP PLL */
    li t3, 0x00FFFFFF
    lw t2, 0x20(t0)
    and t2, t2, t3
    li t3, 0x17000000
    or t2, t2, t3
    sw t2, 0x20(t0)

1:  lw t3, 0xd8(t0)
    andi t3, t3, 0x1
    beqz t3, 1b
    nop   /* time slot */

    li t3, 0x00FFFFFF
    lw t2, 0x20(t0)
    and t2, t2, t3
    li t3, 0x11000000
    or t2, t2, t3
    sw t2, 0x20(t0)

1:  lw t3, 0xd8(t0)
    andi t3, t3, 0x200
    beqz t3, 1b
    nop   /* time slot */

PLL_Calib_Done:
    /* Delay - This is generating  ms LOW pulse */
    li t3, 0xA000
1:  addi t3, t3, -1
    bnez t3, 1b
    nop   /* time slot */

    lw t2,0x04(t0)
    li t3,0x80000000
    or t2,t2,t3    /* Bringing peripherals out of reset */
    sw t2,0x4(t0)
    b pll_complete
    nop   /* time slot */

pll_complete:
	jr ra
    nop   /* time slot */
ENDF(init_pll)
