/*******************************************************************************
 * Description   : CSRs for DMA0/DMA1 Controllers
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_DMA_H_
#define _REG_DMA_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define  DMA_BASE           0xB90A0000

#if 1

#define  DMA_ID             (DMA_BASE + 0x0)
#define  DMA_CTRL           (DMA_BASE + 0x4)
#define  DMA_EN             0
#define  DMA_RST            1
#define  DMA_BURST_8        2
#define  DMA_PER_CHAN_NO    (DMA_BASE + 0x8)
#define  EXT_DMA_TX_CH_NO   4
#define  EXT_DMA_RX_CH_NO   12

#define  DMA_CPE_INT_EN (DMA_BASE + 0x120)

#define  DMA_CPE_INT        (DMA_BASE + 0x124)
#define  DMA_APU_INT_EN     (DMA_BASE + 0x128)
#define  DMA_INT_EN         31
#define  DMA_APU_INT        (DMA_BASE + 0x12C)
#define  DMA_ERR_ADDR       (DMA_BASE + 0x130)
#define  DMA_ACTIVE_CHA_NO  (DMA_BASE + 0x134)
#define  DMA_BUFFER_STATUS  (DMA_BASE + 0x138)
#define  DMA_SOURCE_ADDR    (DMA_BASE + 0x13C)
#define  DMA_DEST_ADDR      (DMA_BASE + 0x140)
#define  DMA_CRC_RESULT     (DMA_BASE + 0x144)
#define  DMA_DEBUG          (DMA_BASE + 0x148)

#define  DMA_CH_EN          24
#define  DMA_CH_START       16
#define  DMA_BD_INT_EN      15
#define  DMA_DONE_INT_EN    14
#define  DMA_INT_DST        10
#define  DMA_INT_CPE        0
#define  DMA_INT_APU        1
#define  DMA_SD             8
#define  DMA_FLY_BY_SRC     7
#define  DMA_FLY_BY_DEST    6
#define  DMA_BYTE_EN        4
#define  DMA_SRC_SYNK_MODE  3
#define  DMA_CRC_EN         2
#define  DMA_CH_RST         0
// CH0
#define  DMA_CH0_BASE  (DMA_BASE + 0x10)
#define  DMA_CH0_CTRL       (DMA_BASE + 0x14)
#define  DMA_CH0_STATUS     (DMA_BASE + 0x18)
#define  DMA_CH0_CBD_ADDR   (DMA_BASE + 0x1C)
// CH1
#define  DMA_CH1_BASE  (DMA_BASE + 0x20)
#define  DMA_CH1_CTRL       (DMA_BASE + 0x24)
#define  DMA_CH1_STATUS     (DMA_BASE + 0x28)
#define  DMA_CH1_CBD_ADDR   (DMA_BASE + 0x2C)
// CH2
#define  DMA_CH2_BASE  (DMA_BASE + 0x30)
#define  DMA_CH2_CTRL       (DMA_BASE + 0x34)
#define  DMA_CH2_STATUS     (DMA_BASE + 0x38)
#define  DMA_CH2_CBD_ADDR   (DMA_BASE + 0x3C)
// CH3
#define  DMA_CH3_BASE  (DMA_BASE + 0x40)
#define  DMA_CH3_CTRL       (DMA_BASE + 0x44)
#define  DMA_CH3_STATUS     (DMA_BASE + 0x48)
#define  DMA_CH3_CBD_ADDR   (DMA_BASE + 0x4C)
// CH4
#define  DMA_CH4_BASE  (DMA_BASE + 0x50)
#define  DMA_CH4_CTRL       (DMA_BASE + 0x54)
#define  DMA_CH4_STATUS     (DMA_BASE + 0x58)
#define  DMA_CH4_CBD_ADDR   (DMA_BASE + 0x5C)
// CH5
#define  DMA_CH5_BASE  (DMA_BASE + 0x60)
#define  DMA_CH5_CTRL       (DMA_BASE + 0x64)
#define  DMA_CH5_STATUS     (DMA_BASE + 0x68)
#define  DMA_CH5_CBD_ADDR   (DMA_BASE + 0x6C)
// CH6
#define  DMA_CH6_BASE  (DMA_BASE + 0x70)
#define  DMA_CH6_CTRL       (DMA_BASE + 0x74)
#define  DMA_CH6_STATUS     (DMA_BASE + 0x78)
#define  DMA_CH6_CBD_ADDR   (DMA_BASE + 0x7C)

// Generic
#define  DMA_CH_BASE   (DMA_BASE + 0x10)
#define  DMA_CH_CTRL        (DMA_BASE + 0x14)
#define  DMA_CH_STATUS      (DMA_BASE + 0x18)
#define  DMA_CH_CBD_ADDR    (DMA_BASE + 0x1C)

/**************************************************************
 * SYS DMA1
*/
#define DMA1_ID                 0xB90A8000
#define DMA1_CTRL               0xB90A8004
#define DMA1_PER_CHAN_NO        0xB90A8008
#define DMA1_CPE_INT_EN         0xB90A8120
#define DMA1_CPE_INT            0xB90A8124
#define DMA1_APU_INT_EN         0xB90A8128
#define DMA1_APU_INT            0xB90A812C
#define DMA1_ERR_ADDR           0xB90A8130
#define DMA1_ACTIVE_CH_NO       0xB90A8134
#define DMA1_BUFFER_STATUS      0xB90A8138
#define DMA1_SOURCE_ADDR        0xB90A813C
#define DMA1_DEST_ADDR          0xB90A8140
#define DMA1_CRC_RESULT         0xB90A8144

#define DMA1_CH0_BASE      0xB90A8010
#define DMA1_CH0_CTRL           0xB90A8014
#define DMA1_CH0_STATUS         0xB90A8018
#define DMA1_CH0_CBD_ADDR       0xB90A801C

#define DMA1_CH1_BASE      0xB90A8020
#define DMA1_CH1_CTRL           0xB90A8024
#define DMA1_CH1_STATUS         0xB90A8028
#define DMA1_CH1_CBD_ADDR       0xB90A802C

#define DMA1_CH2_BASE      0xB90A8030
#define DMA1_CH2_CTRL           0xB90A8034
#define DMA1_CH2_STATUS         0xB90A8038
#define DMA1_CH2_CBD_ADDR       0xB90A803C

#define DMA1_CH3_BASE      0xB90A8040
#define DMA1_CH3_CTRL           0xB90A8044
#define DMA1_CH3_STATUS         0xB90A8048
#define DMA1_CH3_CBD_ADDR       0xB90A804C

#define DMA1_CH4_BASE      0xB90A8050
#define DMA1_CH4_CTRL           0xB90A8054
#define DMA1_CH4_STATUS         0xB90A8058
#define DMA1_CH4_CBD_ADDR       0xB90A805C

#define DMA1_CH5_BASE      0xB90A8060
#define DMA1_CH5_CTRL           0xB90A8064
#define DMA1_CH5_STATUS         0xB90A8068

#define DMA1_CH5_CBD_ADDR       0xB90A806C
#define DMA1_CH6_BASE      0xB90A8070
#define DMA1_CH6_CTRL           0xB90A8074
#define DMA1_CH6_STATUS         0xB90A8078
#define DMA1_CH6_CBD_ADDR       0xB90A807C

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

#endif /* _REG_DMA_H_ */
