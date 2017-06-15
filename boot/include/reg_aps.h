/*******************************************************************************
 * Description   : CSRs for APs (Accelerator Processor Units)
 *     GEMAC 1/2
 *     xDSL PHY
 *     SecurityProtocol
 *     BMU(Buffer Management)
 *     CLassification 1/2
 *     Host AP supports wire-speed PCIe-based WLAN
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_APS_H_
#define _REG_APS_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#if 1

/* GEMAC 1/2 */

#define  GEMAC1_BASE           0xB9200000
#define  GEMAC1_APU_CSR        0xB9200300
#define  GEMAC1_APU_PMEM       0xB9204000
#define  GEMAC1_APU_DMEM       0xB9208000

#define  GEMAC2_BASE           0xB9210000
#define  GEMAC2_APU_CSR        0xB9210300
#define  GEMAC2_APU_PMEM       0xB9214000
#define  GEMAC2_APU_DMEM       0xB9218000

/* xDSL PHY */

#define  XDSL_BASE             0xB9220000
#define  XDSL_APU_CSR          0xB9220300
#define  XDSL_APU_PMEM         0xB9224000
#define  XDSL_APU_DMEM         0xB9228000

/* SecurityProtocol */
 
#define  SPA_BASE              0xB9230000
#define  SPA_APU_CSR           0xB9230300
#define  SPA_APU_PMEM          0xB9234000
#define  SPA_APU_DMEM          0xB9238000

/* Buffer Management */

#define  BMU_BASE              0xB9240000
#define  BMU_APU_CSR           0xB9240300
#define  BMU_APU_PMEM          0xB9244000
#define  BMU_APU_DMEM          0xB9248000

/*  */
#define  EAP_BASE              0xB9250000
#define  EAP_APU_CSR           0xB9250300
#define  EAP_APU_PMEM          0xB9254000
#define  EAP_APU_DMEM          0xB9258000

/* CLassification 1/2 */

#define  CLASS1_BASE           0xB9260000
#define  CLASS1_APU_CSR        0xB9260300
#define  CLASS1_APU_PMEM       0xB9264000
#define  CLASS1_APU_DMEM       0xB9268000

#define  CLASS2_BASE           0xB9270000
#define  CLASS2_APU_CSR        0xB9270300
#define  CLASS2_APU_PMEM       0xB9274000
#define  CLASS2_APU_DMEM       0xB9278000

#define  CLASS_BUFFER          0xB9280300

/* Host AP supports wire-speed PCIe-based WLAN */

#define  HAP_BASE              0xB9290000
#define  HAP_APU_CSR           0xB9290300
#define  HAP_APU_PMEM          0xB9294000
#define  HAP_APU_DMEM          0xB9298000

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

#define AP_CTRL_RESET_BIT      15              // Reset AP (overrides RUN bit)
#define AP_CTRL_RUN_BIT        14              // Allow AP to run
#define DMEM_AREA_OFFSET       0x8000


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

#endif /* _REG_APS_H_ */
