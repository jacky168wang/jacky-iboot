/*******************************************************************************
 * Description   : CSRs for NAND Controller
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_NAND_H_
#define _REG_NAND_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

/* KSEG1 uncached address of EP501M3 NAND controller */
#define EP501M3_BASE        0xB9090000

#if 1

#define EP501M3_RD(a)       H_RD32(EP501M3_BASE + (a))
#define EP501M3_WR(a, d)    H_WR32(EP501M3_BASE + (a), d)

#define EP501M3_DATA            0x0000
#define EP501M3_COMMAND         0x8000
#define EP501M3_INDEX           0x8004
#define EP501M3_STATUS_0        0x8008
#define EP501M3_UNUSED_0        0x800C
#define EP501M3_ID_0            0x8010
#define EP501M3_UNUSED_1        0x8014
#define EP501M3_ID_2            0x8018
#define EP501M3_UNUSED_2        0x801C
#define EP501M3_ID_4            0x8020
#define EP501M3_UNUSED_3        0x8024
#define EP501M3_ID_6            0x8028
#define EP501M3_UNUSED_4        0x802C
#define EP501M3_INT_ENABLE      0x8030
#define EP501M3_INT_STATUS      0x8034
#define EP501M3_INT             0x8038
#define EP501M3_GENERIC_CMD     0x803C
#define EP501M3_GENERIC_STATUS  0x8040
#define EP501M3_GENERIC_DATA    0x8044
#define EP501M3_EXT_INDEX       0x8048
#define EP501M3_TIMING_0        0x804C
#define EP501M3_TIMING_1        0x8050
#define EP501M3_CONFIG          0x8054
#define EP501M3_DMA_ADDR        0x8058
#define EP501M3_BCH_STATUS      0x807C

#define EP501M3_SECT0_SYNDROME12    0x8080
#define EP501M3_SECT0_SYNDROME34    0x8084
#define EP501M3_SECT0_SYNDROME56    0x8088
#define EP501M3_SECT0_SYNDROME78    0x808C

#define EP501M3_SECT1_SYNDROME12    0x8090
#define EP501M3_SECT1_SYNDROME34    0x8094
#define EP501M3_SECT1_SYNDROME56    0x8098
#define EP501M3_SECT1_SYNDROME78    0x809C

#define EP501M3_SECT2_SYNDROME12    0x80A0
#define EP501M3_SECT2_SYNDROME34    0x80A4
#define EP501M3_SECT2_SYNDROME56    0x80A8
#define EP501M3_SECT2_SYNDROME78    0x80AC

#define EP501M3_SECT3_SYNDROME12    0x80B0
#define EP501M3_SECT3_SYNDROME34    0x80B4
#define EP501M3_SECT3_SYNDROME56    0x80B8
#define EP501M3_SECT3_SYNDROME78    0x80BC

#define EP501M3_SECT4_SYNDROME12    0x80C0
#define EP501M3_SECT4_SYNDROME34    0x80C4
#define EP501M3_SECT4_SYNDROME56    0x80C8
#define EP501M3_SECT4_SYNDROME78    0x80CC

#define EP501M3_SECT5_SYNDROME12    0x80D0
#define EP501M3_SECT5_SYNDROME34    0x80D4
#define EP501M3_SECT5_SYNDROME56    0x80D8
#define EP501M3_SECT5_SYNDROME78    0x80DC

#define EP501M3_SECT6_SYNDROME12    0x80E0
#define EP501M3_SECT6_SYNDROME34    0x80E4
#define EP501M3_SECT6_SYNDROME56    0x80E8
#define EP501M3_SECT6_SYNDROME78    0x80EC

#define EP501M3_SECT7_SYNDROME12    0x80F0
#define EP501M3_SECT7_SYNDROME34    0x80F4
#define EP501M3_SECT7_SYNDROME56    0x80F8
#define EP501M3_SECT7_SYNDROME78    0x80FC

#define EP501M3_SECTx_SYNDROMEy(x, y) (0x8080 + ((x) * 0x10) + ((y) * 4))

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

#define EP501M3_COMMAND_CE_OFF  0xEE

#define NR_SYNDROMES            (8)
#define MAX_FIXABLE_ERRS        (NR_SYNDROMES / 2)

/* Bit fields for EP501M3_TIMING_0 register */
#define T0_TS(x)  (((x) & 0xff) <<  0)
#define T0_TWP(x) (((x) & 0xff) <<  8)
#define T0_TRP(x) (((x) & 0xff) << 16)
#define T0_TH(x)  (((x) & 0xff) << 24)

/* Bit fields for EP501M3_TIMING_1 register */
#define T1_TWH(x) (((x) & 0xff) <<  0)
#define T1_TRH(x) (((x) & 0xff) <<  8)
#define T1_TRR(x) (((x) & 0xff) << 16)
#define T1_TWB(x) (((x) & 0xff) << 24)

/* Bit fields for EP501M3_CONFIG register */
#define C_CSZ(x) (((x) & 0x7) <<  0)
#define C_BNK(x) (((x) & 0x3) <<  6)
#define C_PSZ(x) (((x) & 0x3) <<  8)
#define C_WPR(x) (((x) & 0x1) << 10)
#define C_RAC(x) (((x) & 0x1) << 12)

#define LANDSHARK_BCH_BOOTSTRAP_MAGIC       0x4C424348

#define	NAND_RESET_1GB_4KB	0x17	// 0b'10= 4KB  0b'111 = 8Gb - other bit status set to default
#define	NAND_RESET_1GB_4KB_MASK	0x17	// 0b'10= 4KB  0b'111 = 8Gb - other bit status set to default

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

#endif /* _REG_NAND_H_ */
