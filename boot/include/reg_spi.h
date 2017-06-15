/*******************************************************************************
 * Description   : CSRs for SPI0/SPI1 Controllers
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_SPI_H_
#define _REG_SPI_H_

#define  SPI_BASE               0xB9020000

#define  SPI_GLOB_CONFIG        (SPI_BASE + 0x00)
#define  SPI_PORT_CONFIG        (SPI_BASE + 0x04)
#define  SPI_BLOCK_SIZE         (SPI_BASE + 0x08)
#define  SPI_INSTR              (SPI_BASE + 0x0C)
#define  SPI_ADDRESS            (SPI_BASE + 0x10)
#define  SPI_DEV_MODE_BYTE      (SPI_BASE + 0x14)
#define  SPI_FIFO_BNDR          (SPI_BASE + 0x18)
#define  SPI_DEV_RSR            (SPI_BASE + 0x1C)
#define  SPI_DEV_RCR            (SPI_BASE + 0x20)
#define  SPI_STATUS             (SPI_BASE + 0x24)
#define  SPI_INT                (SPI_BASE + 0x28)
#define  SPI_INT_EN             (SPI_BASE + 0x2C)
#define  SPI_RX_FIFO            (SPI_BASE + 0xBFC)
#define  SPI_TX_FIFO            (SPI_BASE + 0xAFC)

// Bit definitions
/* GLOB_CONFIG register */
#define  SPI_CE                 0
#define  SPI_CLK_POL            1
#define  SPI_PD                 2
#define  SPI_SR_COLD            3
#define  SPI_SR_WORM            4
#define  SPI_AHB_RESP_CTRL      5
#define  SPI_AHB_WAIT_MAX       7
#define  SPI_FIFO_RST           11
#define  SPI_INTR_PORT0         26
#define  SPI_AHB_RESP_ACTIVE    30

/* Port Config Register */
#define  SPI_IE                 0
#define  SPI_START              1
#define  SPI_STOP               2
#define  SPI_HOLD               3
#define  SPI_DEV_SEL            4
#define  SPI_CLK_SEL            7
#define  SPI_RF_RST             12
#define  SPI_TF_RST             13
#define  SPI_WRR_SIZE           14
#define  SPI_RES_INSTR_TYPE     16

/* FIFO BNDR Register */
#define  SPI_RF_ALMOST_FULL_BNDR    0
#define  SPI_RF_ALMOST_EMPTY_BNDR   8
#define  SPI_TF_ALMOST_FULL_BNDR    16
#define  SPI_TF_ALMOST_EMPTY_BNDR   24

/* DEV RSR Register */
#define  SPI_WIP                    0
#define  SPI_WEL                    1
#define  SPI_BP0                    2
#define  SPI_BP1                    3
#define  SPI_BP2                    4
#define  SPI_E_ERR                  5
#define  SPI_P_ERR                  6
#define  SPI_SRWD                   7

/* DEV RCR Register */
#define  SPI_FREEZE                 0
#define  SPI_QUAD                   1
#define  SPI_TBPARM                 2
#define  SPI_BPNV                   3
#define  SPI_LOCK                   4
#define  SPI_TBPROT                 5

/* STATUS Register */
#define  SPI_BUSY                   0
#define  SPI_RF_RST_DONE            1
#define  SPI_TF_RST_DONE            2
#define  SPI_CLK_STABLE             3
#define  SPI_CLK_POL_DONE           4
#define  SPI_HOLD_RESUME_DONE       5
#define  SPI_STOP_DONE              6
#define  SPI_CLK_CHG_DONE           3

/* INT EN register */
#define  SPI_RF_FULL                0
#define  SPI_TF_FULL                1
#define  SPI_RF_EMPTY               2
#define  SPI_TF_EMPTY               3
#define  SPI_RF_ALMOST_FULL         4
#define  SPI_TF_ALMOST_FULL         5
#define  SPI_RF_ALMOST_EMPTY        6
#define  SPI_TF_ALMOST_EMPTY        7
#define  SPI_TRNF_END               8
#define  SPI_TRNF_ERR               9
#define  SPI_DONE                   10

/* SPI Commands */
// Read
#define  SPI_CMD_READ               0x03        // Read data bytes
#define  SPI_CMD_FAST_READ          0x0B        // Read data bytes at higher speed
#define  SPI_CMD_DOR                0x3B        // Dual Output Read
#define  SPI_CMD_QOR                0x6B        // Quad Output Read
#define  SPI_CMD_DIOR               0xBB        // Dual I/O High Performance Read
#define  SPI_CMD_QIOR               0xEB        // Quad I/O High Performance Read
#define  SPI_CMD_RDID               0x9F        // Read Identification
#define  SPI_CMD_READ_ID            0x90        // Read Manufacturer Device Identification

// Write Control
#define  SPI_CMD_WREN               0x06        // Write Enable
#define  SPI_CMD_WRDI               0x04        // Write Disable

// Erase
#define  SPI_CMD_P4E                0x20        // 4KB Parameter Sector erase
#define  SPI_CMD_P8E                0x40        // 8KB Parameter Sector erase
#define  SPI_CMD_SE                 0xD8        // Sector Erase
#define  SPI_CMD_BE                 0xC7        // Bulk Erase

// Program
#define  SPI_CMD_PP                 0x02        // Page Program
#define  SPI_CMD_QPP                0x32        // Quad Page Program

// Status Register
#define  SPI_CMD_RDSR               0x05        // Read Status Register
#define  SPI_CMD_WRR                0x01        // Write to Status register
#define  SPI_CMD_RCR                0x35        // Read Configuration register
#define  SPI_CMD_CLSR               0x30        // Reset the erase and Program fail flag

// Power Saving
#define  SPI_CMD_DP                 0xB9        // Deep Power Down
#define  SPI_CMD_RES                0xAB        // Release from Deep power down mode

// OTP
#define  SPI_CMD_OTPP               0x42        // Program One byte of data in OTP memory space
#define  SPI_CMD_OTPR               0x4B        // Read data in the OTP memory space

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

#endif /* _REG_SPI_H_ */
