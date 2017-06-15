/*******************************************************************************
 * Description   : CSRs for GPIO
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_GPIO_H_
#define _REG_GPIO_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define  GPIO_BASE          0xB9030000

#if 1

#define  GPIO_INPUT         (GPIO_BASE + 0x00)
#define  GPIO_OUTPUT_SET    (GPIO_BASE + 0x04)
#define  GPIO_OUTPUT_CLR    (GPIO_BASE + 0x08)
#define  GPIO_MODE_REG1     (GPIO_BASE + 0x0C)
#define  GPIO_MODE_REG2     (GPIO_BASE + 0x10)
#define  GPIO_ALT_SELECT    (GPIO_BASE + 0x14)
#define  GPIO_ALT_DEFAULT   (GPIO_BASE + 0x18)

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

#define  GPIO_NORMAL_INPUT  0
#define  GPIO_NORMAL_OUTPUT 1
#define  GPIO_OPEN_SOURCE   2
#define  GPIO_OPEN_DRAIN    3
#define  GPIO_NORMAL        0
#define  GPIO_ALT           1

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

#endif /* _REG_GPIO_H_ */
