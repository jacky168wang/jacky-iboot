/*******************************************************************************
 * Description   : CSRs for HSPORT (Host Synchronous Serial Port)
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_HSPORT_H_
#define _REG_HSPORT_H_

/**************************************************************
 * Host Sport Registers
*/
#define HSPORT_FIFTHRLD            (0xB90D0000)
#define HSPORT_RXCTRL              (0xB90D0014)
#define HSPORT_TXCTRL              (0xB90D0030)
#define HSPORT_RXDIV               (0xB90D0010)
#define HSPORT_TXDIV               (0xB90D002C)
#define HSPORT_RFSDIVMODE          (0xB90D000C)
#define HSPORT_MISCTRL             (0xB90D0028)
#define HSPORT_MCMRXENBL           (0xB90D0020)
#define HSPORT_MCMRXENBH           (0xB90D0024)
#define HSPORT_MCMTXENBL           (0xB90D0018)
#define HSPORT_MCMTXENBH           (0xB90D001C)
#define HSPORT_STATUS              (0xB90D003C)
#define HSPORT_FIFLVL              (0xB90D0040)
#define HSPORT_RXPTR               (0xB90D0044)
#define HSPORT_TXPTR               (0xB90D0048)
#define HSPORT_RXFIFO              (0xB90D0100)
#define HSPORT_TXFIFO              (0xB90D0200)

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#if 1

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

#endif /* _REG_HSPORT_H_ */
