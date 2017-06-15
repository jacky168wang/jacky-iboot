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

