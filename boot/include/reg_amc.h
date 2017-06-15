/*******************************************************************************
 * Description   : CSRs for AMC (Asynchronous Memory Controller)
 *
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_AMC_H_
#define _REG_AMC_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define  AMC_CTL_BASE               0xB9080000

#define  AMC_BANK0_BASE             0xBC000000
#define  AMC_BANK1_BASE             0xBD000000
#define  AMC_BANK2_BASE             0xBE000000
#define  AMC_BANK3_BASE             0xBF000000

#if 1

#define  AMC_AMGCTL                 (AMC_CTL_BASE + 0)
#define  AMC_AMBCTL0                (AMC_CTL_BASE + 4)
#define  AMC_AMBCTL1                (AMC_CTL_BASE + 8)

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/


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

#endif /* _REG_AMC_H_ */
