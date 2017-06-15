/*******************************************************************************
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
LEAF(init_ddr_hw)
    la    t0, DDR_CTL_BASE

    /*---------------------------
    * ddr_phy_init
    */
    li    t2, 0x30001000
    sw    t2, 0x8124(t0)

    li    t2, 0x01000000
    sw    t2, 0x815C(t0)

    li    t2, 0x220
    sw    t2, 0x8120(t0)

    li    t2, 0x01004342
    sw    t2, 0x8118(t0)

    li    t2, 0x301
    sw    t2, 0x811C(t0)

    li    t2, 0x43
    sw    t2, 0x810C(t0)

    /*---------------------------
    * ddr_scl_polling_done
    */
    
1:  lw    t2, 0x00FC(t0)
    andi  t2, t2, DDR_SCL_DONE
    bnez  t2, 1b
    nop   /* delay slot */

    /*---------------------------
    * ddr_ctl_init
    */

    li    t2, 0xd
    sw    t2, 0x0010(t0)

    li    t2, 0x352407a0
    sw    t2, 0x0000(t0)

#if (DDRMODE == 1) /* 16Bit mode */
    li    t2, 0x22161424
    sw    t2, 0x0004(t0)

    li    t2, 0x03208067
    sw    t2, 0x0098(t0)
#else
    li    t2, 0x22163424
    sw    t2, 0x0004(t0)
#endif

    li    t2, 0xc6001542
    sw    t2, 0x0008(t0)

    li    t2, 0xc6001442
    sw    t2, 0x0008(t0)

    li    t2, 0x18
    sw    t2, 0x000C(t0)

    li    t2, 0x0
    sw    t2, 0x001C(t0)

    /*---------------------------
    * ddr_ctl_polling_modeset / Delay to finish "MODE set"
    */
    nop_loop(0xff)

    /*---------------------------
    * ddr_scl_run
    */

    /*---------------------------
    * ddr_scl_read_read: hardware automatically/dynamically calibrate PHY timing
    */
    li    t2, SCL_START_CONTINUOUS_R | SCL_START_GO_DONE
    sw    t2, 0x8100(t0)

    /*---------------------------
    * ddr_scl_polling_start: calibration is done
    */

1:  lw    t3, 0x8100(t0)
    and   t3, t3, SCL_START_GO_DONE
    bnez  t3, 1b /* 'go' done if 0 */
    nop   /* delay slot */
    
    /*---------------------------
    * ddr_scl_write_read: hardware automatically/dynamically calibrate PHY timing
    */

    li    t2, SCL_START_GO_DONE
    sw    t2, 0x8100(t0)

    /*---------------------------
    * ddr_scl_polling_start: calibration is done
    */

1:  lw    t3, 0x8100(t0)
    and   t3, t3, SCL_START_GO_DONE
    bnez  t3, 1b /* 'go' done if 0 */
    nop   /* delay slot */

    /*---------------------------
    * ddr_scl_is_ok ?
    */

    lw    t3, 0x8100(t0)
    andi  s0, t3, SCL_CUML_SCL_RSLT /* success: 0x3/0xf if 16/32-bit */
    xori  s0, s0, SCL_CUML_SCL_RSLT
    
    /* return
    */
    jr	  ra
    nop   /* time slot */
ENDF(init_ddr_hw)

/*==============================================================================
*  ---- NO stack ----
*  input: NONE
*   used: t0:  SYS_MEM_BASE ~ SYS_MEM_TEST_SIZE
*         t2: pattern (1<<0)~(1<<31) to be written into
*         t3: acutal data value which has been read out
* return: s0: 0 - success
*/
LEAF(ram_bitflip_verify)
    move  s0, zero
    li    t2, 0x00000001
label_next_pattern:
    li    t0, SYS_MEM_BASE
label_ram_write:
    sw    t2, 0x0000(t0)
	addi  t0, t0, 4
	bne   t0, SYS_MEM_TEST_SIZE, label_ram_write
    nop   /* delay slot */
    li    t0, SYS_MEM_BASE
label_ram_read:
    lw    t3, 0x0000(t0)
    bne   t3, t2, label_exit
    li    s0, 1 /* delay slot */
	addi  t0, t0, 4
	bne   t0, SYS_MEM_TEST_SIZE, label_ram_read
    nop   /* delay slot */
lab_verify_ok:
	sll   t2, t2, 1
	beqz  t2, label_next_pattern
    nop   /* delay slot */
label_exit:
    /* return
    */
    jr	  ra
    nop   /* time slot */
ENDF(ram_bitflip_verify)

/*==============================================================================
*  ---- NO stack ----
*/
NON_LEAF(init_ddr)
	move  t1, zero
    la    t9, init_ddr_hw
    jalr  t9
    nop   /* time slot */
    bnez  s0, 1f
    nop
    la    t9, ram_bitflip_verify
    jalr  t9
    nop
    bnez  s0, 1f
    nop
1:
	addi  t1, t1, 1
	beq   t6, 0x64, softreset_SoC /* reboot if fail 100 time */
    /* return
    */
    jr	  ra
    nop   /* time slot */
ENDF(init_ddr)
