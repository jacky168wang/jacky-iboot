/*******************************************************************************
 * Description   : CSRs for DDR2 SDRAM Controller and PHY
 * 
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_DDR_H_
#define _REG_DDR_H_

#define DRAM_BUS_CLK_250MHZ 4    /* 4nsec */
#define DRAM_BUS_CLK_200MHZ 5    /* 5nsec */
#define SYS_DRAM_BUS_CLK DRAM_BUS_CLK_250MHZ

#define DRAM_TYPE_DDR1    0
#define DRAM_TYPE_DDR2    1
#define SYS_DRAM_TYPE     DRAM_TYPE_DDR2
#if (SYS_DRAM_TYPE == DRAM_TYPE_DDR2)
#define DRAM_DDR2_ODT 0
#endif

#define SYS_MEM_BASE     (A_K0BASE)
#define SYS_MEM_MAX_SIZE    (1<<29) /* 512MiB at most for KSEG0 */
#ifndef DDR_AUTO_DETECT
#define SYS_MEM_SIZE ((1 << (22+DRAM_CHIP_1024MBIT)) * (32/16))
#endif
#define SYS_MEM_TEST_SIZE   (1<<20) /* 1MiB */
//#define SYS_MEM_TEST_SIZE   SYS_MEM_SIZE

/*******************************************************************************
 * 'DDR Controller' MemoryConfig Registers and Port Control/Status Registers
*/

#define DDR_CTL_BASE 0xB9100000 

#ifndef __ASSEMBLY__
typedef struct ddr_ctl_reg_s
{
    H_D32 mem_cfg0;     //0x00
    H_D32 mem_cfg1;     //0x04
    H_D32 mem_cfg2;     //0x08
    H_D32 mem_cfg3;     //0x0c
    H_D32 mem_cfg4;     //0x10
    H_D32 mem_cfg5;     //0x14
    H_D32 mem_cfg6;     //0x18
    H_D32 mem_cfg7;     //0x1c
    H_D32 rsvd0[24]; //0x20, 24*4 
    H_D32 prt_arb1;     //0x80
    H_D32 pri_cnt0;     //0x84
    H_D32 pri_cnt1;     //0x88
    H_D32 pri_prd0;     //0x8c
    H_D32 pri_prd1;     //0x90
    H_D32 prt_arb2;     //0x94
    H_D32 prf_ctrl;     //0x98
    H_D32 rsvd1[14]; //0x9C, 14*4
    H_D32 status_bw_util;    //0xD4
    H_D32 status_px_cntr[8]; //0xD8 ~ 0x0F4
    H_D32 status_plat;       //0xF8
    H_D32 status_perf;       //0xFC
} ddr_ctl_reg_t;

#define DDR_CTL ((ddr_ctl_reg_t *)DDR_CTL_BASE)
#endif


/* 0x1910_0000, MEMORY_CTRL_0, 0x35A4_8820
--------------------------------------------------------------------------------
    [31:28]tRRD(active-to-active delay of different banks)
                                x4/x8: 2 =7.5/4ns, x16: 3 = 10/4ns
    [27:23]tRAS(active to precharge)  10 = 40/4ns,     11 = 45ns/4ns
    [22:19]tRP(precharge to active)    4 = 15/4ns
    [18:13]tRFC(refresh to active)    
                      32   127.5/4ns for   1Gbits
                      26   105  /4ns for 512Mbits
                      19    75  /4ns for 256Mbits
                      49   197.5/4ns for   2Gbits
    [12: 0]tREFI(refresh to active) 1952 = 64ms/Rows(64000000/8192/4ns)
*/

#define TIMING_RRD      ((12/SYS_DRAM_BUS_CLK) << 28) /* DRAM_CHIP_WIDTH relative */

/* tRAS Min is 40 for most chips, but 45 for some(Sumsang, ISSI) */
#define TIMING_RAS      ((45/SYS_DRAM_BUS_CLK) << 23) /* DRAM_CHIP_WIDTH relative */

#define TIMING_RP       ((15/SYS_DRAM_BUS_CLK) << 19)

#define TIMING_RFC      ((128/SYS_DRAM_BUS_CLK) <<13) /* DRAM_CHIP_SIZE relative*/

#define REFI_64MSEC     ((64000000/8192)/SYS_DRAM_BUS_CLK)
#define REFI_32MSEC     ((32000000/8192)/SYS_DRAM_BUS_CLK)

/* 0x1910_0004, MEMORY_CTRL_1, 0x2216_3424
--------------------------------------------------------------------------------
    [31:28]tWTR(Write-to-Read)         2 = 10/4ns
    [27:24]tRTP(Read-to-precharge)     2 = 7.5/4ns
    [23:21]reserved
    [20:18]size=0b101/1Gbits, 0b100/512Mbits, 0b011/256Mbits, 0b110/2Gbits
    [17:16]width=0b10/16bit, 0b01/8bit, 0b00/4bit
    [15:14]00: 1 external bank (CS0), 1 module
    [13:12](RO) double of BUS width: 0b00/128bit, 0b10/32bit, 0b11/64bit
    [11: 8]tWR(Write Recovery)         4 = 15/4ns
    [ 7: 4]tMRD(Mode Register Set)     2 Clk
    [ 3: 0]tRCD(RAS# to CAS# delay)    4 = 15/4ns
*/
#define TIMING_WTR      ((8/SYS_DRAM_BUS_CLK) << 28)

#define TIMING_RTP      ((8/SYS_DRAM_BUS_CLK) << 24)

#define DRAM_CHIP_64MBIT   1
#define DRAM_CHIP_128MBIT  2
#define DRAM_CHIP_256MBIT  3
#define DRAM_CHIP_512MBIT  4
#define DRAM_CHIP_1024MBIT 5
#define DRAM_CHIP_2048MBIT 6
#ifndef DDR_AUTO_DETECT
#define DRAM_CHIP_SIZE   (DRAM_CHIP_1024MBIT << 18)
#endif

#define DRAM_CHIP_4BIT  0
#define DRAM_CHIP_8BIT  1
#define DRAM_CHIP_16BIT 2
#ifndef DDR_AUTO_DETECT
#define DRAM_CHIP_WIDTH  (DRAM_CHIP_16BIT << 16)
#endif

#define DRAM_SYS_1BANK_1MODULE 0
#define DRAM_SYS_2BANK_1MODULE 1
#define DRAM_SYS_4BANK_2MODULE 2
#define DRAM_SYS_2BANK_2MODULE 3
#define DRAM_SYS_BANKS (DRAM_SYS_1BANK_1MODULE << 14)
#define DRAM_SYS_CS_BITMASK 1 /* only CS0 is connected by default */

#define DRAM_SYS_BUS_64BIT 0
//#define DRAM_SYS_BUS_16BIT 1 /* NOTICE: not Reserved ! */
#define DRAM_SYS_BUS_16BIT 2
#define DRAM_SYS_BUS_32BIT 3
//#ifndef DDR_AUTO_DETECT
#define SYS_DRAM_WIDTH DRAM_SYS_BUS_32BIT /* ikanos chips support 16/32-bit only */
/* Number of byte / nibble lanes */
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
#define MEM_STRB_WIDTH 2
#else
#define MEM_STRB_WIDTH 4
#endif
#define DRAM_SYS_BUS_WIDTH  (SYS_DRAM_WIDTH << 12)
//#endif

#define TIMING_WR      ((16/SYS_DRAM_BUS_CLK) <<  8)

#define TIMING_MRD     (2 << 4)

#define TIMING_RCD     ((16/SYS_DRAM_BUS_CLK) << 0)

/* 0x1910_0008, MEMORY_CTRL_2, 0x4300_1642    * Mode Register Set *
--------------------------------------------------------------------------------
    [   31]Tctrl_delay is 2 or 1
    [   30]DDR2 mode, not DDR mode
    [29:27]reserved
    [26:24]TPHY_RDLAT(must be MAIN_CLK_DLY+2)
    [23:13]reserved
    [   12]Slow/fast active power down exit time
    [11: 9]Auto precharge write recovery
    [    8]Normal Operation with/without DLL reset
    [    7]Test Mode Operation or Normal Operation
    [ 6: 4]CAS Latency ***CL*** (support 3,4,5 - 0b011/100/101)
    [    3]Burst type Interleaved or Sequential, sequential only during power-up
    [ 2: 0]Burst length is 2/4/8 (0b001/010/011), words in a single write/read
*/

/* 1: external register delay is inserted in the control and address signals
 * 0: no external register delay is needed when register mode SDRAM DIMM is used
*/
#define WITH_EXT_REG_DELAY  (1 << 31)

#define MRS_DDR_TYPE   (SYS_DRAM_TYPE << 30)

#define DDRPHY_RDLAT_STG1   ((4+2) << 24)

/* enable slow active power down exit time, WITH_DLL_RESET at the same time */
#define SLOW_POWERDOWN_EXIT (1 << 12)

// default value is 3
//#define AUTO_PRECHARGE_WR   (3 << 9)
#define AUTO_PRECHARGE_WR   (2 << 9)

/* M8: DLLRESET, self-clearing, is required during power-up initialization
 * 200 clock delay is a MUST to ensure DLL is locked
*/
#define WITH_DLL_RESET      (1 << 8)

/* for DDR1, only , , 2, 2.5, 3,  , 1.5 are supported */
/* for DDR2, only , ,  ,    , 3, 4, 5   are supported */
#define CAS_LATENCY 4
#define TIMING_CAS          (CAS_LATENCY << 4)

#define BURST_MODE_SEQUENTIAL 0
#define BURST_MODE_INTERLEAVE 1
/* can not be changed for Vx185/183 */
#define BURST_MODE          (BURST_MODE_SEQUENTIAL << 3)

#define BURST_LENGTH_2WORD 1
#define BURST_LENGTH_4WORD 2
#define BURST_LENGTH_8WORD 3
/* can not be changed for Vx185/183 */
#define BURST_LENGTH        (BURST_LENGTH_4WORD << 0)

/* 0x1910_000C, MEMORY_CTRL_3, 0x0000_0418    * Extended Mode Register Set *
--------------------------------------------------------------------------------
    [31:13]reserved
    [   12]Disables Output Buffer
    [   11]Enables     redundant DQS, ie. RDQS
    [   10]Disables differential DQS, ie. DQS#
    [ 9: 7]Enables the OCD function
    [    6]DDR2_ODT_RTT1
    [ 5: 3]Additive Latency (typically tRCD-1, 0b011 = 3 Clk)
    [    2]DDR2_ODT_RTT0  -- 00/01/10/11: DisablesODT/75ohm/150ohm/(50ohm-ISSI)
    [    1]60%/100% drive strength
    [    0]Disables DLL
*/
#define DISABLE_OUTPUT_BUF  (0 << 12) /*  enable output buffer by default */

#define ENABLE_RDQS         (0 << 11) /* disable redundant DQS by default */

#define DISABLE_nDQS        (1 << 10) /* disable differential DQS by default */
#define ENABLE_nDQS         (0 << 10) /* enable differential DQS */

#define OCD_CALIBRATION_EXIT 0
#define OCD_NO_CHANGE 0
#define OCD_DRIVE1 1
#define OCD_DRIVE0 2
#define OCD_ADJUST 4 /* AL from previously set value must be applied */
#define OCD_CALIBRATION_DEFAULT 7 /* needs to be exited latter */
#define OCD_MODE            (OCD_NO_CHANGE << 7)
#if (DRAM_DDR2_ODT == 1)
/* 75ohm/150/50ohm for single/multi/"TBD" chip(s) by default */
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
#define DDR2_ODT_RTT1       (0 << 6)
#define DDR2_ODT_RTT0       (1 << 2)
#else
#define DDR2_ODT_RTT1       (1 << 6)
#define DDR2_ODT_RTT0       (0 << 2)
#endif
#else
#define DDR2_ODT_RTT1       (0 << 6)
#define DDR2_ODT_RTT0       (0 << 2)
#endif

#define DRIVE_STRENGTH_NOT_FULL (0 << 1)

/* DDR1/2 always enable SDRAM's DLL to DQS synchronization */
#define DISABLE_DLL         (0 << 0)

/* Normal timing: RowActive--tRCD--Read--tCL--data_out,
 *        the 2nd RowActive is delayed 1clock due to Read use address also
 * Posted CAS   : RowActive--1clock--Read--tAL--RowActive--tCL--data_out
 *
 * "AL=0": disable "Posted CAS", this will get better performance if 
 *         continually access the same page.
 * otherwize, enable "Posted CAS", then it will get bad performance if
 *         continually access the same page, but higher performance if
 *         access 'random' location/page.
 */
#if (DRAM_DDR2_AL == 1)
#define ADDITIVE_LATENCY ((TIMING_RCD >> 0) - 1)
#else
//#define ADDITIVE_LATENCY 0
#define ADDITIVE_LATENCY 3
#endif
#define DDRCTL_ADDITIVE_LATENCY (ADDITIVE_LATENCY << 3)

/*  Read Latency: tAL+tCL */
//#define RD_CAS_LATENCY (ADDITIVE_LATENCY + CAS_LATENCY)
#define RD_CAS_LATENCY 4

/* Write Latency: Read Latency - 1 */
#define WR_CAS_LATENCY (RD_CAS_LATENCY - 1)

/* 0x1910_0010, MEMORY_CTRL_4, 0x0000_0008
--------------------------------------------------------------------------------
    [31: 5]Reserved
    [ 4: 0]tFAW(Four Bank Activate period) MIN 50/4ns, ignored in 4-bank devices
*/

#define FAW 0xd

/* 0x1910_0098, DDR_PERF_CNTR_CTRL, 0x0320_8027
--------------------------------------------------------------------------------
    [31:17]ARB_2MICRO_TC   R/W Terminal count for 2 ms counters. 0x0190
    [16   ]PERF_COUNTER_EN R/W Enables the performance counters. 0x0
    [15: 9]PERF_COUNT_SAMPLE_TIME R/W Time period to sample counters. 0x40
    [ 8: 6]RD_LAT_STG2     R/W Stage2 Read Latency. 0x0
                           0/2/4/6: no latency, 1/3/5/7:1/2/3/4 clocks.
    [ 5: 3]CMD_FIFO_LWM R/W Command FIFO low watermark, performance related. 0x4
    [ 2: 0]CMD_FIFO_HWM R/W Command FIFO high watermark. 0x7
*/
#define ARB_2MICRO_TC          (0x0190 << 17)

#define PERF_COUNTER_EN        (0 << 16)

#define PERF_COUNT_SAMPLE_TIME (0x40 << 9)

#define DDRPHY_RDLAT_STG2_0CLK (0x0 << 6)
#define DDRPHY_RDLAT_STG2_1CLK (0x1 << 6)
#define DDRPHY_RDLAT_STG2_2CLK (0x3 << 6)
#define DDRPHY_RDLAT_STG2_3CLK (0x5 << 6)

#define CMD_FIFO_LWM           (0x4 << 3)
#define CMD_FIFO_HWM           (0x7 << 0)

/* 0x1910_00FC, DDR_PERF, 0x0000_0000
--------------------------------------------------------------------------------
    [31:16]DDR_DWORD_CNTR  R Number of DWORDS accessed from DDR SDRAM.
    [15: 1]DDR_IDLE_CNTR   R DDR SDRAM idle count.
    [0]    DDR_SCL_DONE    R Indicated DDR SDRAM SCL is done.
*/
#define DDR_DWORD_CNTR_M    ((1 << 17)-1)
#define DDR_DWORD_CNTR_S    16
#define DDR_IDLE_CNTR_M     ((1 << 16)-1)
#define DDR_IDLE_CNTR_S     1
#define DDR_SCL_DONE     (1 << 0)


/*******************************************************************************
 * 'DDR PHY' Registers
*/

#define DDR_PHY_BASE (0xB9108100)

#ifndef __ASSEMBLY__
typedef struct ddr_phy_reg_s
{
    H_D32 scl_start;         //0x00
    H_D32 scl_data0;         //0x04
    H_D32 scl_data1;         //0x08
    H_D32 scl_latency;       //0x0c
    H_D32 scl_rd_addr;       //0x10
    H_D32 scl_rd_data;       //0x14
    H_D32 scl_cnfg1;         //0x18
    H_D32 scl_cnfg2;         //0x1c
    H_D32 pad_ctrl;          //0x20
    H_D32 dll_recalib;       //0x24
    H_D32 rsvd[13];
    H_D32 analog_dll1;       //0x5c
    H_D32 analog_dll2;       //0x60
    H_D32 dll_incr_trim;     //0x64
} ddr_phy_reg_t;

#define DDR_PHY ((ddr_phy_reg_t *)DDR_PHY_BASE)
#endif

/* 0x1910_8100, SCL_START, 0x0000_0000
--------------------------------------------------------------------------------
    [31] SAVE_RESTORE_DATA    RW "SCL related"
    [30] WRLVL                RW "SCL related"
    [29] CONTINUOUS_RDS       RW "SCL related"
    [28] SET_DDR_SCL_GO_DONE  RW "SCL related"
    [25] RD_ONLY               W "SCL related"
    [24] WR_ONLY               W "SCL related"
    [23:20] CYCLE_CNT          R "SCL related"
    [MEM_STRB_WIDTH-1:0] CUML_SCL_RSLT   R "SCL related"
*/

#define SCL_START_SAVE_DATA    (1<<31)
#define SCL_START_RESTORE_DATA (0<<31)

#define SCL_START_WRLVL        (1<<30) /* DDR3 only */

#define SCL_START_CONTINUOUS_R (1<<29)

#define SCL_START_GO_DONE      (1<<28)

#define SCL_START_RD_ONLY      (1<<25)

#define SCL_START_WR_ONLY      (1<<24)

#define SCL_CUML_SCL_RSLT      ((1 << MEM_STRB_WIDTH) - 1)


/* 0x1910_8104, SCL_DATA_0, 0x789B_3DE0
--------------------------------------------------------------------------------
   0x1910_8108, SCL_DATA_1, 0xF10E_4A56
*/

/* 0x1910_810C, SCL_LATENCY, 0x0000_0000
--------------------------------------------------------------------------------
    [8] EXT_LOOPBACK          RW Set to place the PHY in external loopback mode
    [7:4] MAIN_CLK_DLY        RW the largest possible value of the read latency
          taking worst case round trip timing into account
          main_clk_dly = (3 + MAX_LATENCY) - reduce_cmd_latency - reduce_rd_latency
          MAX_LATENCY is the maximum number of clock cycles of latency 
          introduced by IO, package and board latencies
    [3:0] CAPTURE_CLK_DLY     RW set to MAIN_CLK_DLY-1 for debugging only
*/
#define SCL_EXT_LOOPBACK (1 << 8)

#define DRAM_MAX_LATENCY 1
#define SCL_MAIN_CLK_DLY ((3+DRAM_MAX_LATENCY-REDUCE_CMD_LATENCY-REDUCE_RD_LATENCY) << 4)

#define SCL_CAPTURE_CLK_DLY ((SCL_MAIN_CLK_DLY >> 4) - 1)

/* 0x1910_8110, SCL_RD_ADDR, 0x0000_0040
--------------------------------------------------------------------------------
    [8+STRB-1: 8] DATA_PHY_SEL RW "SCL related"
    [7:4] ROTATE_CTR_STRT_VAL  RW "SCL related"
    [1:0] SCL_RD_ADDR          RW "SCL related"
*/

/* 0x1910_8114, SCL_RD_DATA, 0x0000_0000
--------------------------------------------------------------------------------
    [31:0] SCL_RD_DATA          R "SCL related"
*/

/* 0x1910_8118, SCL_CONFIG_1, 0x0000_0000
--------------------------------------------------------------------------------
    [24:+nCS]SCL_DDR_ODT_CTRL_WR   RW Turns on ODT on CS signal during SCL writes
    [16:+nCS]DDR_ODT_CTRL_RD   RW Not used
    [15:12]SCL_LOCAL_ODT_CTRL      RW dynamic termination for PHY DDR2 pads is
           1:disable           2:permanently enable
           4:+1 for read burst C:+2 for read burst
    [11:8] ADDITIVE_LATENCY    RW Additive Latency
    [7:4]  RD_CAS_LATENCY      RW Read CAS Latency
    [3:2]  Reserved            RW 00
    [1]    DDR2                RW
    [0]    BURST8              RW bust8 or bust4 mode during executing SCL
*/
#define SCL_ADDITIVE_LATENCY  (ADDITIVE_LATENCY << 8)

#define SCL_RD_CAS_LATENCY  (RD_CAS_LATENCY << 4)

#define SCL_DDR_TYPE   (SYS_DRAM_TYPE << 1)

#define SCL_BURST_LENGTH_4  (0 << 0)
#define SCL_BURST_LENGTH_8  (1 << 0)

/* "eliminate impactive reflection" is realized by external termination
 * resistance like DDR1, rather than OnDieTermination with internal 
 * termination resistance(Rtt)
 */
#define DYN_TERM_PADS_DISABLE          (1 << 0)
#define DYN_TERM_PADS_ENABLE           (1 << 1)
#define DYN_TERM_1CLK_AFTER_READ_BURST (1 << 2)
#define DYN_TERM_2CLK_AFTER_READ_BURST (3 << 2)
#if (DRAM_DDR2_ODT == 0) /* ODT must enable for SCL, but controller disable ODT, why? */
#define SCL_LOCAL_ODT_CTRL      (DYN_TERM_1CLK_AFTER_READ_BURST << 12)
#else
#define SCL_LOCAL_ODT_CTRL      (DYN_TERM_PADS_DISABLE << 12)
#endif
#define SCL_DDR_ODT_CTRL_RD     (0 << 16) /* always not used */
#define SCL_DDR_ODT_CTRL_WR     (DRAM_SYS_CS_BITMASK << 24) /* always used */

/* 0x1910_811C, SCL_CONFIG_2, 0x0000_0000
--------------------------------------------------------------------------------
    [0:+nCS] SCL_TEST_CS       RW Executes SCL on specified chip select
    [11:8]   WR_CAS_LATENCY    RW DRAM Write CAS latency
*/
#define SCL_TEST_CS     (DRAM_SYS_CS_BITMASK << 0)

#define SCL_WR_CAS_LATENCY   (WR_CAS_LATENCY << 8)

/* 0x1910_8120, PHY_PAD_CTRL, 0x0000_0200
--------------------------------------------------------------------------------
    [2:0] DQ_DQS_TERM RW DDR2 I/O pad termination value applied to DQ/DQS pads during reads.
    [3]   Reserved    RW Reserved
    [6:4]DQ_DQS_DRIVE RW DDR2 SDRAM I/O pad drive strength control for DQ/DQS pads
    [7]CAL_FAULT_SEEN R  Set if dynamic calibration fails at any time.
    [8] EXTRA_OEN_CLK RW Set to drive pad output an extra cycle after a write burst
    [9]NO_EXTERNAL_DLL  RW 1: digital DLL is used to center DQS on DQs on reads.
                           0:  analog DLL or an external DLL is used.
    [10] REDUCE_RD_LATENCY  RW Not used
    [11] REDUCE_CMD_LATENCY RW Not used
    [12] DDRAM_RESET         RW Reserved
    [13] DLY_DFI_WRCMD      RW 1: If DRAM Write CAS latency is an even number.
    [18:16] ADRCTRL_DRIVE   RW DDR2 I/O pad drive strength control for address/control pads.
    [22:20] CLK_DRIVE       RW DDR2 I/O pad drive strength control for clock pads
*/
#define SCL_DQ_DQS_TERM     0

#define SCL_DQ_DQS_DRIVE    (2 << 4)

#define CAL_FAULT_SEEN      (1 << 7)

#define CAL_EXTRA_OEN_CLK   (1 << 8)

#define CAL_NO_EXTERNAL_DLL (1 << 9)

#define REDUCE_RD_LATENCY   0
#define REDUCE_CMD_LATENCY  0

#define SCL_DLY_DFI_WRCMD   ((WR_CAS_LATENCY & 1) << 13)

#define SCL_ADRCTRL_DRIVE   (0 << 16)

#define SCL_CLK_DRIVE       (0 << 20)

/* 0x1910_8124, PHY_DLL_RECALIB, 0x0400_0000
--------------------------------------------------------------------------------
[25:0] RECALIB_CNT     RW Sets the period of digital DLL recalibration.
[26]   DISABLE_RECALIB RW Prevents digital DLL from recalibrating after the first time.
[31:28]CTR_START_VAL   RW Controls the start value of digital DLL master delay line.
       Recommended value is 3.
*/
#define SCL_RECALIB_CNT         0x001000
#define SCL_DISABLE_RECALIB     (1 << 26)
#define SCL_CTR_START_VAL       (3 << 28)

/* 0x1910_815C, ANALOG_DLL_1, 0xbbbb_bbbb
--------------------------------------------------------------------------------
 [MEM_STRB_WIDTH-1:0] ANALOG_DLL_RESET RW If internal analog DLL is used, allows
                      reset control for the same on a per byte lane basis.
 [24] NO_ANALOG_DLL_WRCLK RW If set, internal analog DLL is bypassed and the 
                      digital DLL of the PHY is used instead.
*/
#define SCL_ANALOG_DLL_RESET    ((1 << MEM_STRB_WIDTH) - 1)

#define SCL_NO_ANALOG_DLL_WRCLK (1 << 24)

/* 0x1910_8160, ANALOG_DLL_2, 0xbbbb_bbbb
--------------------------------------------------------------------------------
 [MEM_STRB_WIDTH-1:0] ANALOG_DLL_LOCK R Lock state indication from internal Analog DLL.
 [30:24] MAS_DLY R Master delay line setting indication from internal Analog DLL.
*/
#define SCL_ANALOG_DLL_LOCK_MSK ((1 << MEM_STRB_WIDTH) - 1)

#define SCL_MAS_DLY_MSK (0xff << 24)

/* 0x1910_8164, DLL_INCR_TRIM_1, 0x0000_0000
--------------------------------------------------------------------------------
 [MEM_STRB_WIDTH-1:0] INCR_DLY_1 RW 1/0: used to increment/decrement the associated delay.
                                    1 bit per byte lane. (Default: 0)
 [MEM_STRB_WIDTH-1:0] INCR_DLY_3 RW 1/0: used to increment/decrement the associated delay.
                                    1 bit per byte lane. (Default: 0)
*/

/*******************************************************************************
 *  Public "Variables"
 ******************************************************************************/

/*******************************************************************************
 *  Public "Functions" for 'c'/'assembly' routines
 ******************************************************************************/
#ifndef __ASSEMBLY__

extern int init_ddr(void);

#else


#endif /* __ASSEMBLY__ */

#endif /* _REG_DDR_H_ */
