/*******************************************************************************
 * Description   : CSRs for 4 GPTs(General Purpose Timer) & WDT(WatchDog Timer)
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_TIMER_H_
#define _REG_TIMER_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define  TIMER_BASE         0xB9050000
#define  TIMER_TOP_COUNT_0  (TIMER_BASE + 0x00)
#define  TIMER_TOP_COUNT_1  (TIMER_BASE + 0x08)
#define  TIMER_TOP_COUNT_2  (TIMER_BASE + 0x10)
#define  TIMER_TOP_COUNT_3  (TIMER_BASE + 0x18)
#define  TIMER_CONTROL_0    (TIMER_BASE + 0x04)
#define  TIMER_CONTROL_1    (TIMER_BASE + 0x0C)
#define  TIMER_CONTROL_2    (TIMER_BASE + 0x14)
#define  TIMER_CONTROL_3    (TIMER_BASE + 0x1C)
// Bit positions for Control register
#define  TIMER_GPIO_EN          0
#define  TIMER_ENABLE           1
#define  TIMER_PERIODIC         2
#define  TIMER_CHAIN_SELECT     3
#define  TIMER_PRESCALAR        5
#define  TIMER_PRESCALAR_DIS    6

#define  TIMER_PRESCALAR_0  (TIMER_BASE + 0x20)
#define  TIMER_PRESCALAR_1  (TIMER_BASE + 0x24)
#define  TIMER_INT_CONTROL  (TIMER_BASE + 0x38)
//Bit definition for Timer interrupt control
#define  TIMER_INT_ENABLE   1

#define  TIMER_INT_STATUS   (TIMER_BASE + 0x3C)
#define  WDT_CONTROL        (TIMER_BASE + 0x28)
// Bit definitions for WDT Control reg
#define  WDT_ENABLE         0
#define  WDT_INT_MASK       1
#define  WDT_RESET_MASK     2
#define  WDT_TEST_MODE      3
#define  LOCK_VAL           16

#define  WDT_PET            (TIMER_BASE + 0x2C)
#define  WDT_TOP_COUNT      (TIMER_BASE + 0x30)
#define  WDT_COUNT          (TIMER_BASE + 0x34)
#define  WDT_LOCK_VAL       0x6BAD0000

#endif /* _REG_TIMER_H_ */
