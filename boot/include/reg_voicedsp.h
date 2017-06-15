/*******************************************************************************
 * Description   : CSRs for Voice DSP0/DSP1 
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_VOICEDSP_H_
#define _REG_VOICEDSP_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#if 1

#define DSP0_IDMA_BASE              0xB90B0000
#define DSP0_PMEM_REG               0xB90B0004
#define DSP0_DMEM_REG               0xB90B0008
/* 16bit */
#define DSP0_DMEM_REG_HW            0xB90B0008
#define DSP1_IDMA_BASE              0xB90B0010
#define DSP1_PMEM_REG               0xB90B0014
#define DSP1_DMEM_REG               0xB90B0018
/* 16bit */
#define DSP1_DMEM_REG_HW            0xB90B0018

#define DSP_IDMA_INTEN              0xB90B800C
#define DSP_IDMA0_ERR               0xB90B8014
#define DSP_ADDR_IDMA0_ERR          0xB90B8018
#define DSP_IDMA1_ERR               0xB90B8030
#define DSP_ADDR_IDMA1_ERR          0xB90B8034

#define DSP_CPE_PERINT              0xB90B803C
#define DSP_CPE_FLINT               0xB90B8040
#define DSP_CPE_PFLINT              0xB90B8044
#define DSP_ADDR_IACKREG            0xB90B8048
#define DSP_APU_DSP_FLAGREG         0xB90B804C
#define DSP_ADDR_DSP_CTRL           0xB90B8050

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

#endif /* _REG_VOICEDSP_H_ */
