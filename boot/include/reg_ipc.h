/*******************************************************************************
 * Description   : CSRs for IPC (Interrupt Controller)
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_IPC_H_
#define _REG_IPC_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define  IPC_BASE           0xB9040000

#if 1

#define  IPC_SRC_CONTROL_0  (IPC_BASE + 0x00)
#define  IPC_SRC_CONTROL_1  (IPC_BASE + 0x04)
#define  IPC_SRC_CONTROL_2  (IPC_BASE + 0x08)
#define  IPC_SRC_CONTROL_3  (IPC_BASE + 0x0C)
#define  IPC_SRC_CONTROL_4  (IPC_BASE + 0x10)
#define  IPC_SRC_CONTROL_5  (IPC_BASE + 0x14)
#define  IPC_SRC_CONTROL_6  (IPC_BASE + 0x18)
#define  IPC_SRC_CONTROL_7  (IPC_BASE + 0x1C)

#define  IPC_DST_SELECT_0   (IPC_BASE + 0x20)
#define  IPC_DST_SELECT_1   (IPC_BASE + 0x24)
#define  IPC_DST_SELECT_2   (IPC_BASE + 0x28)
#define  IPC_DST_SELECT_3   (IPC_BASE + 0x2C)
#define  IPC_DST_SELECT_4   (IPC_BASE + 0x30)
#define  IPC_DST_SELECT_5   (IPC_BASE + 0x34)
#define  IPC_DST_SELECT_6   (IPC_BASE + 0x38)
#define  IPC_DST_SELECT_7   (IPC_BASE + 0x3C)

#define  IPC_STATUS_LOW_0   (IPC_BASE + 0x40)
#define  IPC_STATUS_HIGH_0  (IPC_BASE + 0x80)

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

/* Interrupt source mapping */
#define TIMER0_INT          10
#define TIMER1_INT          11
#define TIMER2_INT          12
#define TIMER3_INT          13

/************************************************************************
 *  Public "Variables"
 ************************************************************************/

#if defined(__ASSEMBLY__)
/************************************************************************
 *  Public "Functions" for 'assemble' routines
 ************************************************************************/

#else
/************************************************************************
 *  Public "Functions" for 'c' routines
 ************************************************************************/
#endif /* __ASSEMBLY__ */

#endif /* _REG_IPC_H_ */
