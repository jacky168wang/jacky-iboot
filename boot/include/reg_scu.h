/*******************************************************************************
 * Description   : CSRs for SCU (System Configuration Unit)
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_SCU_H_
#define _REG_SCU_H_


#if (PALLADIUM == 1)
    #define SYSTEM_CLK_BASIC (250*1000000)
#else 
    #if (FPGA == 1)
        #define SYSTEM_CLK_BASIC ( 25*1000000)
    #elif defined(VX185_A1) || defined(VX185_B0)
        #define SYSTEM_CLK_BASIC (250*1000000)
    #elif defined(VX183)
        #define SYSTEM_CLK_BASIC (200*1000000)
    #endif
#endif

/*
 * LMEM Address
*/
#define  LMEM_BASE                  0xB91D0000


/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

/*
 * SCU Address
*/
#define SCU_BASE 0xB9000000

#define SCU_RST_VECT_MASK      (SCU_BASE + 0x00)
#define SCU_RST_VECT           (SCU_BASE + 0x04)
#define SCU_SYS_NRESET         (SCU_BASE + 0x04)
#define SCU_CTRL_MASK          (SCU_BASE + 0x08)
#define SCU_CTRL_CTRL          (SCU_BASE + 0x0C)
#define SCU_RESET_CAUSE        (SCU_BASE + 0x10)
#define SCU_PART_REV_NUM       (SCU_BASE + 0x18)
#define SCU_CLOCK_GATE_CTRL    (SCU_BASE + 0x1C)
#define SCU_CLK_ENABLE         (SCU_BASE + 0x1C)

#define SCU_BME_MAP0           (SCU_BASE + 0x54)
#define SCU_BME_MASK0          (SCU_BASE + 0x58)
#define SCU_BME_MAP1           (SCU_BASE + 0x5C)
#define SCU_BME_MASK1          (SCU_BASE + 0x60)

#define SCU_SPARE0_REG         (SCU_BASE + 0xEC)
#define SCU_SPARE1_REG         (SCU_BASE + 0xF0)
#define SCU_SPARE2_REG         (SCU_BASE + 0xF4)
#define SCU_SPARE3_REG         (SCU_BASE + 0xF8)
#define SCU_SPARE4_REG         (SCU_BASE + 0xFC)

#ifndef __ASSEMBLY__

typedef struct scu_reg_s {
    H_D32 rst_mask;     //0x000
    H_D32 rst_vec;      //0x004
    H_D32 cpu_ctl_mask; //0x008
    H_D32 cpu_ctl;      //0x00C
    H_D32 rst_cause;    //0x010
    H_D32 rst_strp;     //0x014
    H_D32 rev_num;      //0x018
    H_D32 clkgate;      //0x01C
    H_D32 pll_pwr_rst;  //0x020
    H_D32 pll_vcxo_sigmadelta;  //0x024
    H_D32 pll_dsp_sigmadelta;   //0x028
    H_D32 pll_post_divider1;  //0x02C
    H_D32 pll_post_divider2;  //0x030
    H_D32 pll_post_divider3;  //0x034
    H_D32 pll_fixed_analog;    //0x038
    H_D32 pll_vcxo_analog;    //0x03C
    H_D32 pll_gateway_analog; //0x040
    H_D32 pll_bme_analog;    //0x044
    H_D32 pll_dsp_analog;    //0x048
    H_D32 pll_test1;    //0x04C
    H_D32 pll_test2;    //0x050
    H_D32 bme_map0;     //0x054
    H_D32 bme_mask0;    //0x058
    H_D32 bme_map1;     //0x05C
    H_D32 bme_mask1;    //0x060
    H_D32 mailbox_h2b_intst;  //0x064
    H_D32 mailbox_h2b_intcl;  //0x068
    H_D32 mailbox_h2b_inten;   //0x06C
    H_D32 mailbox_h2b[8];     //0x070-0x08C
    H_D32 mailbox_b2h_intst;  //0x090
    H_D32 mailbox_b2h_intcl;  //0x094
    H_D32 mailbox_b2h_inten;   //0x098
    H_D32 mailbox_b2h[8];     //0x09C-0xB8
    H_D32 nandctrl_status;     //0x0BC
    H_D32 por_status;      //0x0C0
    H_D32 pvt_status;      //0x0C4
    H_D32 pll_fixed_status;    //0x0C8
    H_D32 pll_vcxo_status;    //0x0CC
    H_D32 pll_gateway_status;    //0x0D0
    H_D32 pll_bme_status;    //0x0D4
    H_D32 pll_dsp_status;    //0x0D8
    H_D32 pll_gateway_sigmadelta;    //0x0DC
    H_D32 pll_bme_sigmadelta;   //0x0E0
    H_D32 rsvd[2];      //0x0E4-0xE8
    H_D32 spare0;       //0x0EC
    H_D32 pcie_rc0;     //0x0F0
    H_D32 pcie_rc1;     //0x0F4
    H_D32 spare3;       //0x0F8
    H_D32 spare4;       //0x0FC
} scu_reg_t;

#define SCU ((scu_reg_t *)SCU_BASE)

#endif /* __ASSEMBLY__ */

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/

/* 0x1900_0000, System Controller Reset Vector AND Clock Gate Mask, 0xFFFF_FFFF
--------------------------------------------------------------------------------
   0x1900_0004, System Controller Reset Vector Set , 0x0000_0003
--------------------------------------------------------------------------------
*/
#define RST_CLKGATE_MSK_SYS         (1 <<  0)
#define RST_CLKGATE_MSK_MIPS        (1 <<  1)
#define RST_CLKGATE_MSK_GIGE0_AP    (1 <<  2)
#define RST_CLKGATE_MSK_GIGE1_AP    (1 <<  3)
#define RST_CLKGATE_MSK_GIGE2_AP    (1 <<  4)
#define RST_CLKGATE_MSK_SPA_AP      (1 <<  5)
#define RST_CLKGATE_MSK_BMU_AP      (1 <<  6)
#define RST_CLKGATE_MSK_CLASS_AP    (1 <<  7)
/* reserved 8 */
#define RST_CLKGATE_MSK_HOST_AP     (1 <<  9)
#define RST_CLKGATE_MSK_DSP         (1 << 10)
#define RST_MSK_DSP_AHB             (1 << 11)
#define RST_CLKGATE_MSK_PCIE        (1 << 12)
#define RST_CLKGATE_MSK_SATA        (1 << 13)
#define RST_CLKGATE_MSK_USB         (1 << 14)
/* reserved 15 */
#define RST_MSK_GIGE0               (1 << 16)
#define RST_MSK_GIGE1               (1 << 17)
#define RST_MSK_GIGE2               (1 << 18)
#define RST_MSK_SPA                 (1 << 19)
#define RST_MSK_BMU                 (1 << 20)
#define RST_MSK_CLASS               (1 << 21)
/* reserved 22 */
#define RST_MSK_HOST_AP             (1 << 23)
#define RST_MSK_BME_VCXO            (1 << 24)
#define RST_MSK_BME_FREQ_DM         (1 << 25)
#define RST_MSK_BME_MIPS            (1 << 26)
#define RST_CLKGATE_MSK_DDR_CTRL    (1 << 27)
#define RST_MSK_DDR_CTRL_AHB        (1 << 28)
#define RST_MSK_BME_AHB             (1 << 29)
/* reserved 30 */
#define RST_MSK_EXT_PINS            (1 << 31)

/* 0x1900_001C, Clock Gate Control, RO, 0x0FFF_FFFC
--------------------------------------------------------------------------------
*/
/* reserved 1:0 */
#define GIGE0_AP_CLK_GATE      (1 <<  2)
#define GIGE1_AP_CLK_GATE      (1 <<  3)
#define GIGE2_AP_CLK_GATE      (1 <<  4)
#define SPA_AP_CLK_GATE        (1 <<  5)
#define BMU_AP_CLK_GATE        (1 <<  6)
#define CLASS_AP_CLK_GATE      (1 <<  7)
/* reserved 8 */
#define HOST_AP_CLK_GATE       (1 <<  9)
#define DSP_CLK_GATE           (1 << 10)
#define GMII_CLK_GATE          (1 << 11)
#define PCIE_CLK_GATE          (1 << 12)
#define SATA_CLK_GATE          (1 << 13)
#define USB_CLK_GATE           (1 << 14)
#define SPI0_SLOW_CLK_GATE     (1 << 15)
#define SPI_CLK_GATE           (1 << 16)
/* reserved 17 */
#define GIGE2_MAC_CLK_GATE     (1 << 18)
#define RMII_2P5MHZ_CLK_GATE   (1 << 19)
#define RMII_25MHZ_CLK_GATE    (1 << 20)
#define RMII_50MHZ_CLK_GATE    (1 << 21)
/* reserved 23:22 */
#define BME_VCXO_CLK_GATE      (1 << 24)
#define BME_FREQ_DM_CLK_GATE   (1 << 25)
#define BME_MIPS_CLK_GATE      (1 << 26)
#define DDR_CTRL_CLK_GATE      (1 << 27)
#define DSP_REF_CLK_GATE       (1 << 28) /* 0/1: 25/35.328 MHz */
#define DDR_CTRL_CLK_SRC       (1 << 29) /* 0/1: System/AP Clock */
/* reserved 31:30 */

/* 0x1900_0008, System Controller Control Mask, 0xFFFF_FF00
--------------------------------------------------------------------------------
   0x1900_000C, System Controller Control Set , 0xC6C0_0000
--------------------------------------------------------------------------------
*/
#define SCU_CTRL_STUS_BOOTMODE      (7 <<  0)
#define BOOTMODE_NOR    0
#define BOOTMODE_SERIAL 1
#define BOOTMODE_NAND   2
#define BOOTMODE_UART1  3

/* reserved 3 */
#define SCU_CTRL_STUS_CHIP_TYPE     (7 <<  4)
    /* CHIP Type : 185/Vx183/Vx175/Vx173 */
#define CHIP_TYPE ((SCU->cpu_ctl >> 4) & 0x07)
#define CHIP_7185 0
#define CHIP_7175 1
#define CHIP_7183 2
#define CHIP_7173 3
#define CHIP_7185SE 4
#define CHIP_7185HP 6

/* reserved 7 */
#define SCU_CTRL_AMC_CS0_MAP        (1 <<  8)
/* 00: CS0, CS1, CS2, CS3 = 16 MiB
   01: CS0, CS2 = 32 MiB
   10: CS0 = 64 MiB
   11: Not valid */
#define SCU_CTRL_AMC_MAPPING        (3 <<  9)
#define SCU_CTRL_MIPS_HSPORT_EN     (1 << 11)
#define SCU_CTRL_GIGE0_XMII         (3 << 12)
#define SCU_CTRL_GIGE1_XMII         (3 << 14)
#define SCU_CTRL_GIGE2_XMII         (3 << 16)
#define XMIIMODE_MII   0
#define XMIIMODE_RMII  1
#define XMIIMODE_GMII  2
#define XMIIMODE_RGMII 3
#define SCU_CTRL_EXTERNAL_SYNC      (1 << 18)
#define SCU_CTRL_USB_AHB_ERR        (1 << 19)
/* must be programmed before bringing BME out of reset */
#define SCU_CTRL_XDSL_BONDING_EN    (1 << 20)
#define SCU_CTRL_VCXO_COMMON_CLOCK  (1 << 21)
#define SCU_CTRL_PCIE_REFCLK_O25M   (1 << 22)
#define SCU_CTRL_SATA_CLK_SCALE     (3 << 23)
#define SATA_CLK_25M   0
#define SATA_CLK_50M   1
#define SATA_CLK_12M5  2
#define SCU_CTRL_POWER_OPTIMIZE_CLK (1 << 25)
#define SCU_CTRL_POWER_OPTIMIZE_MEM (1 << 26)
#define SCU_CTRL_POWER_OPTIMIZE_EN  (1 << 25)
#define SCU_CTRL_DONOT_DISTURB      (0xF<<27)
#define SCU_CTRL_SATA_REFCLK_INNER  (1 << 31)

/* 0x1900_0010, System Controller Reset Cause, RO, 0x0000_0001
--------------------------------------------------------------------------------
*/
#define SCU_RST_CAUSE_POWER     (1 << 0)
#define SCU_RST_CAUSE_SOFT      (1 << 1)
#define SCU_RST_CAUSE_WDT       (1 << 2)
#define SCU_RST_CAUSE_PLL_DRIFT (1 << 3)

#define LAST_RESET_WAS_SOFT (SCU->rst_cause & SCU_RST_CAUSE_SOFT)

/* 0x1900_0014, Reset Strap, RO, 0x0003_FFFF
--------------------------------------------------------------------------------
Strap input is AMC_DATA_B_O[2:0]: BOOTMODE 
Strap input is AMC_DATA_B_O[5:3]: NAND Memory Size
Strap input is AMC_DATA_B_O[7:6]: NAND Page Size
Strap input is AMC_DATA_B_O[  8]: NAND Row Address Cycles (large block only)
Strap input is AMC_DATA_B_O[  9]: JTAG RTCK Bypass
Strap input is AMC_DATA_B_O[ 10]: 1/0: Enables AFE6/AFE7
Strap input is AMC_DATA_B_O[ 11]: 1/0: RGMII pads is 2.5/3.3V
Strap input is AMC_DATA_B_O[ 12]: 1/0: PowerOnReset/EFuse reset is selected
*/

#define SCU_RST_STRAP_BOOTMODE          (3 << 0)

#define SCU_RST_STRAP_NAND_CAPACITY     (7 << 3)
#define CAPACITY_S_64MB  0
#define CAPACITY_S_128MB 1
#define CAPACITY_S_256MB 2
#define CAPACITY_L_16GB  0
#define CAPACITY_L_32GB  1
#define CAPACITY_L_64GB  2
#define CAPACITY_512MB   3
#define CAPACITY_1GB     4
#define CAPACITY_2GB     5
#define CAPACITY_4GB     6
#define CAPACITY_8GB     7

#define SCU_RST_STRAP_NAND_PAGE         (3 << 6)
#define PAGE_512B 0
#define PAGE_2KB  1
#define PAGE_4KB  2

#define SCU_RST_STRAP_NAND_ROWADDR_CYCLE (1 << 8)
#define NAND_ROW_ADDR_TWO_CYCLES   0
#define NAND_ROW_ADDR_THREE_CYCLES 1

#define SCU_RST_STRAP_JTAG_RTCK_BYPASS  (1 << 9)
/* NOTE: This strap bit needs to be read by software and
 * the VCXO bypass bit in the System Controller Control
 * Register needs to be set accordingly.
 */
#define SCU_RST_STRAP_AFE6_ENABLE       (1 << 10)
#define SCU_RST_STRAP_RGMII_PADS_LOW    (1 << 11)
#define SCU_RST_STRAP_EFUSE_RST_BYPASS  (1 << 12)

/* 0x1900_0018, Chip Revision Number, RO, 0x0003_0200
--------------------------------------------------------------------------------
Reserved 31:24 R/O Reserved
Chip ID 23:16 R/O Fusiv Vx185 and Fusiv Vx183= 3
Major Revision Number 15:8 R/O Major Revision number
Minor Revision Number 7:0 R/O Minor Revision number
*/
#define SCU_CHIP_VER1_MSK (0x0f <<  7)
#define SCU_CHIP_VER0_MSK (0x0f <<  8)
#define SCU_CHIP_ID_MSK   (0x0f << 16)


/* 0x1900_0020, PLL Power and Reset, R/W, 0x0A28_B282
--------------------------------------------------------------------------------
*/

#define PLL_FIXED_POWER_UP              (1 <<  0)
#define PLL_FIXED_LOCKDETECTOR_DOWN     (1 <<  1)
#define PLL_FIXED_CALIBRATION           (1 <<  2)
/* reserved  5: 3 */
#define PLL_VCXO_POWER_UP               (1 <<  6)
#define PLL_VCXO_LOCKDETECTOR_DOWN      (1 <<  7)
#define PLL_VCXO_CALIBRATION            (1 <<  8)
#define PLL_VCXO_SIGMADELTA_DOWN        (1 <<  9)
/* reserved 11:10 */
#define PLL_GATEWAY_POWER_UP            (1 << 12)
#define PLL_GATEWAY_LOCKDETECTOR_DOWN   (1 << 13)
#define PLL_GATEWAY_CALIBRATION         (1 << 14)
#define PLL_GATEWAY_SIGMADELTA_DOWN     (1 << 15)
#define PLL_GATEWAY_POSTDIVIDER_UP      (1 << 16)
/* reserved 17 */
#define PLL_BME_POWER_UP                (1 << 18)
#define PLL_BME_LOCKDETECTOR_DOWN       (1 << 19)
#define PLL_BME_CALIBRATION             (1 << 20)
#define PLL_BME_SIGMADELTA_DOWN         (1 << 21)
#define PLL_BME_POSTDIVIDER_UP          (1 << 22)
/* reserved 23 */
#define PLL_DSP_POWER_UP                (1 << 24)
#define PLL_DSP_LOCKDETECTOR_DOWN       (1 << 25)
#define PLL_DSP_CALIBRATION             (1 << 26)
#define PLL_DSP_SIGMADELTA_DOWN         (1 << 27)
#define PLL_DSP_POSTDIVIDER_UP          (1 << 28)
/* reserved 31:29 */

/* 0x1900_0024, VCXO Sigma Delta, R/W, 0x0016_54D6
   0x1900_0028,  DSP Sigma Delta, R/W, 0x003E_2D62
   0x1900_00DC,   GW Sigma Delta, R/W, 0x0000_0000
   0x1900_00E0,  BME Sigma Delta, R/W, 0x0000_0000
--------------------------------------------------------------------------------
*/
#define VCXO_SIGMADELTA_DEFAULT     ( 3 << 24)
#define VCXO_SIGMADELTA_CURRENT     (0xffffff)

#define DSP_SIGMADELTA_DEFAULT      ( 3 << 24)
#define DSP_SIGMADELTA_CURRENT      (0xffffff)

#define GATEWAY_SIGMADELTA_DEFAULT  ( 3 << 24)
#define GATEWAY_SIGMADELTA_CURRENT  (0xffffff)

#define BME_SIGMADELTA_DEFAULT      ( 3 << 24)
#define BME_SIGMADELTA_CURRENT      (0xffffff)


/* 0x1900_002C, PLL Post Divider1, R/W, 0x1809_6860
--------------------------------------------------------------------------------
*/
/* 00/01(default)/10/11: 2/4/8/invalid */
#define PLL_PD1_SPI0_PD2_MSK            (0x03 << 28)
#define PLL_PD1_SPI0_PD1_MSK            (0x0f << 24)
#define PLL_PD1_DSP_FEEDBACK_DIV_MSK    (0xff << 16)
#define PLL_PD1_BME_FEEDBACK_DIV_MSK    (0xff <<  8)
#define PLL_PD1_GATEWAY_FEEDBACK_DIV_MSK (0xff <<  0)
/* 0x1900_0030, PLL Post Divider2, R/W, 0x0406_3032
--------------------------------------------------------------------------------
*/
#define PLL_PD1_DSP_PD2_MSK         (0x03 << 28)
#define PLL_PD1_DSP_PD1_MSK         (0x0f << 24)
#define PLL_PD1_FD_PD2_MSK          (0x03 << 22)
#define PLL_PD1_BME_PD2_MSK         (0x03 << 20)
#define PLL_PD1_FD_PD1_MSK          (0x0f << 16)
#define PLL_PD1_BME_PD1_MSK         (0x0f << 12)
#define PLL_PD1_AP_SYS_PD2_MSK      (0x03 << 10)
#define PLL_PD1_HOST_PD2_MSK        (0x03 <<  8)
#define PLL_PD1_AP_SYS_PD1_MSK      (0x0f <<  4)
#define PLL_PD1_HOST_PD1_MSK        (0x0f <<  0)
/* 0x1900_0034, PLL Post Divider3, R/W, 0x4D00_0FFF
--------------------------------------------------------------------------------
*/
#define ENABLE_GIGE_CLOCK       (1 <<  0)
#define ENABLE_USB_CLOCK        (1 <<  1)
#define ENABLE_SYN_REF_CLOCK    (1 <<  2) /* 250M Synthesizer Ref Clock */
#define ENABLE_HOST_MIPS_CLOCK  (1 <<  3)
#define ENABLE_BME_MIPS_CLOCK   (1 <<  4)
#define ENABLE_DSP_CLOCK        (1 <<  5)
#define ENABLE_AP_CLOCK         (1 <<  6)
#define ENABLE_BME_FD_CLOCK     (1 <<  7)
#define ENABLE_SYS_CLOCK        (1 <<  8)
#define ENABLE_VCXO_CLOCK       (1 <<  9)
#define ENABLE_SPI0_CLOCK       (1 << 10)
#define ENABLE_SPI_SLOW_CLOCK   (1 << 11)
#define BYPASS_DIV2_GATEWAY_PLL (1 << 12)
#define BYPASS_DIV2_BME_PLL     (1 << 13)
#define BYPASS_DIV2_DSP_PLL     (1 << 14)
/* reserved 20:15 */
#define BYPASS_SIGMADELTA_GATEWAY_PLL (1 << 21)
#define BYPASS_SIGMADELTA_BME_PLL   (1 << 22)
#define BYPASS_SIGMADELTA_DSP_PLL   (1 << 23)
/* reserved 31:24 */

/* 0x1900_0038, PLL0 Analog Control, R/W, 0x003F_C442
   0x1900_003C, PLL1 Analog Control, R/W, 0x000C_CA88
   0x1900_0040, PLL2 Analog Control, R/W, 0x0020_C642
   0x1900_0044, PLL3 Analog Control, R/W, 0x002D_C642
   0x1900_0048, PLL4 Analog Control, R/W, 0x0026_CE20
--------------------------------------------------------------------------------
*/

/* 0x1900_004C, PLL Test1, R/W, 0x0000_0000
   0x1900_0050, PLL Test2, R/W, 0x2082_0820
--------------------------------------------------------------------------------
*/

/* 0x1900_0054, BME  Map0, R/W, 0x0000_0000
   0x1900_0058, BME Mask0, R/W, 0x0000_0000
   0x1900_005C, BME  Map1, R/W, 0x0000_0000
   0x1900_0060, BME Mask1, R/W, 0x0000_0000
--------------------------------------------------------------------------------
   0x1900_0064, Host to BME Mailbox Interrupt Set, R/W, 0x0000_0000
   0x1900_0068, Host to BME Mailbox Interrupt Clr, R/W, 0x0000_0000
   0x1900_006C, Host to BME Mailbox Interrupt ENA, R/W, 0x0000_0000
   0x1900_0070 - 0x1900_008C : H2B Mailbox[0:7]
--------------------------------------------------------------------------------
   0x1900_0090, Host to BME Mailbox Interrupt Set, R/W, 0x0000_0000
   0x1900_0094, Host to BME Mailbox Interrupt Clr, R/W, 0x0000_0000
   0x1900_0098, Host to BME Mailbox Interrupt ENA, R/W, 0x0000_0000
   0x1900_009C - 0x1900_00B8 : B2H Mailbox[0:7]
*/

/* 0x1900_00BC, NAND controller Status, RO, 0x0000_0000
--------------------------------------------------------------------------------
*/
#define NAND_CTRL_CHIP_BUSY (1 <<  0)
#define NAND_CTRL_PECC_BUSY (1 <<  1)

/* 0x1900_00C0, POR Control/Status, R/W, 0x0000_0180
--------------------------------------------------------------------------------
*/
#define POR_CTRL_POWER_DOWN         (1 <<  0)
#define POR_CTRL_IO_VOLT_MONITOR    (1 <<  1)
#define POR_CTRL_CORE_VOLT_MONITOR  (1 <<  2)
/* 0/1/2/3: -10%/-5%/-16%/-12%, 2.97/3.14/2.75/2.90V */
#define POR_CTRL_IO_VOLT_THRESHOLD  (3 <<  3)
/* 0/1/2/3: -10%/-20%/-30%/-40%, 0.90/0.80/0.70/0.60V */
#define POR_CTRL_CORE_VOLT_THRESHOLD  (3 <<  5)
#define POR_STAT_IO_VOLT_NOT_FAILED   (1 <<  7)
#define POR_STAT_CORE_VOLT_NOT_FAILED (1 <<  8)

/* 0x1900_00C8, Fixed PLL Status, R, 0x0000_0000
   0x1900_00CC,  VCXO PLL Status, R, 0x0000_0000
   0x1900_00D0,    GW PLL Status, R, 0x0000_0000
   0x1900_00D4,   BME PLL Status, R, 0x0000_0000
   0x1900_00D8,   DSP PLL Status, R, 0x0000_0000   
--------------------------------------------------------------------------------
*/
#define PLL_STAT_CALIBRATION_DONE     (1 <<  0)
#define PLL_STAT_CALIBRATION_RUNING   (1 <<  1)
#define PLL_STAT_CALIBRATION_VALUE (0x3f <<  2)
#define PLL_STAT_LOCKED               (1 <<  9)
#define PLL_STAT_BYPASSED             (1 << 10)

/* 0x1900_00EC, Spare0, R/W, 0x0000_0000
   0x1900_00F8, Spare3, R/W, 0x0000_0000
--------------------------------------------------------------------------------
RGMII mode: 0xC04_892_00
 GMII mode: 0xC04_96D_00
*/

/* 0x1900_00F0, PCIe Root Complex0 MSI Address, R/W, 0x0000_0000
   0x1900_00F4, PCIe Root Complex1 MSI Address, R/W, 0x0000_0000
--------------------------------------------------------------------------------
*/

/************************************************************************
 *  Public "Variables"
 ************************************************************************/

/************************************************************************
 *  Public "Functions" for 'assemble' routines
 ************************************************************************/
#if defined(__ASSEMBLY__)

#endif

/************************************************************************
 *  Public "Functions" for 'c' routines
 ************************************************************************/
#ifndef __ASSEMBLY__

extern void init_plls(void);

#if 0
/* remap CS0 from Bootrom/0xA0800000 to NOR flash/0xBF000000 */
static inline void RemapCS0_NOR(void)
{
    H_D32 temp;

    temp = *(H_A32)SCU_CTRL_MASK;
    temp &= ~0x100;
    *(H_A32)SCU_CTRL_MASK = temp;

    temp = *(H_A32)SCU_CTRL_CTRL;
    temp |=  0x100;
    *(H_A32)SCU_CTRL_CTRL = temp;
}

/* remap CS0 from NOR flash/0xBF000000 to Bootrom/0xA0800000 */
static inline void RemapCS0_ROM(void)
{
    H_D32 temp;

    temp = *(H_A32)SCU_CTRL_MASK;
    temp &= ~0x100;
    *(H_A32)SCU_CTRL_MASK = temp;

    temp = *(H_A32)SCU_CTRL_CTRL;
    temp &= ~0x100;
    *(H_A32)SCU_CTRL_CTRL = temp;
}
#else
extern void RemapCS0_ROM(void);
extern void RemapCS0_ROM(void);
#endif

#endif /* __ASSEMBLY__ */

#endif /* _REG_SCU_H_ */
