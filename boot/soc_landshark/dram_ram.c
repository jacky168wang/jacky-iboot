/*******************************************************************************
 * Description   : DDR (Double Data Rate) SDRAM Controller & PHY
 *
 * # GLOBAL #
 * ------------
 *   initialise "DDR SDRAM Controller"(EUREKA) to setup "DDR SDRAM chip"
 * power-on sequence and calibrate the timing between them. include
 *   0, "Chip" power-on, clock is applied, at the same time "Controller" reset
 *   1, "Controller" issue EMRS/MRS configuration and then enter into IDLE
 *   2, "Self Calibrating Logic" runs algorithm which defined by "SCL" registers
 *      to automatically program "PHY" timing to "Chip"
 *
 * # Commands from "Controller" to SDRAM #
 * ----------------------------------------
 * "Controller" Auto Refresh Mode : 
 *   SDRAM requires to be refreshed within a certain interval to prevent
 *   loss of data. "Controller" has an internal counter to count the refresh
 *   period. When the counter expires, "Controller" PRECHARGEs all the banks
 *   and then issues an REFRESH command to SDRAM if SDRAM in idle state.
 * (CAS# before RAS# (CBR)) REFRESH : CS#/RAS#/CAS# L; WE# H
 * DESELECT/INHIBIT  : CS# H
 *   prevents new commands from being executed by SDRAM.
 *   Operations already in progress are not affected.
 * NO OPERATION(NOP) : CS# L; RAS#/CAS#/WE# H
 *   prevents unwanted commands from being registered during idle/wait states.
 *   Operations already in progress are not affected.
 * LOAD MODE (LM)    : CS# L; RAS#/CAS#/WE# L; BA[1:0]=00/01 for MRS/EMRS
 *   internal configuration registers ie.MRS/EMRS of SDRAM will be programmed. 
 *   only be issued when all banks are idle, and a subsequent executable
 *   command cannot be issued until tMRD is met.
 * ACTIVATE          : CS#/RAS# L; CAS#/WE# H; 
 *   open an internal bank(specified by BA[1:0]) in a row (specified by row
 *   address) which is referred to as the open page, for a subsequent access.
 *   This row will remains active/open for accesses until a PRECHARGE command
 *   is issued to that/same bank.
 * PRECHARGE:
 *   deactivate the open row in a particular bank or all banks. in other words,
 *   close a specific or all internal banks in the active page.
 * READ / WRITE :
 *   initiate a burst read/write access starting from one column address to
 *   an active row. if A10 H, AUTO-PRECHARGE, the row will be precharged at
 *   the end; else, the row will remain open for subsequent accesses.
 *
 * # Standard SDRAM Terms #
 * ------------------------------
 * DataWidth: number of DQ lines, 4/8/16-bit
 * DDR1/2/3: internally 1/2/4-n Prefetch
 * DataRate: externally transfers 2 bits on each data pin per clock cycle,
 *           no matter DDR1/2/3.
 * BurstLength: DDR1:2/4/8; DDR2:4/8; DDR3:8
 * BurstType: sequential('column address' + 'counter')
 *            interleave('column address' exclusive or 'counter')
 * CAS Latency (CL): DDR1:1.5/2/2.5/3; DDR2:3,4,5,(6,7)
 * DM/DQM: DataMask, provide a byte(8bit) masking capability on 8/16-bit
 *   width writes to SDRAM. DM for 8-bit devices, LDM/HDM for16-bit devices.
 *   If LOW/HIGH, the corresponding data will be written/ignored.
 * feature - AL: Posted CAS Additive Latency
 *   allows a READ/WRITE command to be issued prior to tRCD(Min) by delaying
 *   the actual registration of the READ/WRITE command to the internal device
 *   by AL clock cycles.
 * feature - DLL: Delay Locked Loop, align DQ/DQS transitions with CLK
 *   1, Enabling DLL should be followed by "resetting DLL" using LM command,
 *   and 200 clock cycles must occur to ensure the internal clock have been
 *   synchronized with the external clock.
 *   2, DLL enable is required during power-up initialization, and is disable
 *   automatically when finished.
 *   3, DLL is automatically disabled when entering SELF REFRESH operation and
 *   is automatically re-enabled and reset upon exit of SELF REFRESH operation.
 * feature - DQS#/RDQS Enable
 * feature - OCD: Off-Chip Driver Impedance Calibration
 * feature - ODT: On-Die Termination
 * feature - Output Drive Strength Adjustment: 60% SSTL_18 rather than 100%
 *
 * # features of Vx185/Vx183 #
 * ------------------------------
 * supported Data Width   :  8/16-bit
 * supported Bus  Width   : 16/32-bit for Vx185/175, 16-bit only for Vx183/173
 * supported Burst Length : 4 only
 * supported Burst Type   : sequential
 * "Controller" supported features : all
 *    "PHY" SCL supported features : all

# 2 # SCL Algorithm
 3.1 After the controller completes performing the initialization sequence,
     SCL of PHY must be run before data integrity is guaranteed.
 3.2 bit0 of DDR_PERF indicates PHY initialization is completed when becomes 1.
     then PHY is ready to accept any DRAM commands that are sent on the address
     and control pins of the DDR 
 3.3.1 all PHY registers are set to default value.
 3.3.2 ddr_phy_init()
 3.3.3 ddr_scl_until_done()
 3.3.4 ddr_ctl_init()
 3.3.5 wait for some 200 clocks
 3.3.6 ddr_scl_run()
 *
 *     Registers that control PHY timing are automatically programmed when
 * Self Calibrating Logic program is run
 *
 * Author :
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"

#if 0
/* the following codes do NOT function */
/* codes from DO-437058-DG-Programmer Reference Manual */

static inline void ddr_phy_init(void)
{
   H_WR32 (0xB9108124, 0x30001000); // PHY_DLL_RECALIB  0x49
   H_WR32 (0xB910815C, 0x01000000); // UNQ_ANALOG_DLL_1 0x57
   H_WR32 (0xB9108120, 0x00000200); // PHY_PAD_CTRL     0x48
   H_WR32 (0xB9108118, 0x01004242); // CFG1: add lat 3, rd cas lat 4
   H_WR32 (0xB910810C, 0x00000043); // SCL_LATENCY
   H_WR32 (0xB910811C, 0x0000030F); // CFG2: wr cas lat 3
}

/* wait to make sure PHY initialization is completed  */
static inline void ddr_phy_polling_idle(void)
{
    volatile UINT32 rdata;
    do {
        rdata = H_RD32(0xB91000FC);
    } while((rdata & 0x00000001) == 0);
}

static inline void ddr_ctl_init(void)
{
    //H_WR32 (0xB9100000, 0x35A68820); // EUREKA CTRL0
    H_WR32 (0xB9100000, 0x352407a0);
    H_WR32 (0xB9100004, 0x22163424); // EUREKA CTRL1
    //H_WR32 (0xB9100008, 0xC6001632); // EUREKA CTRL2
    H_WR32 (0xB9100008, 0xC6001432);
    H_WR32 (0xB910000C, 0x00000018); // EUREKA CTRL3
    //H_WR32 (0xB9100010, 0x0000000A); // EUREKA CTRL4
    H_WR32 (0xB9100010, 0x0000000d); // EUREKA CTRL4
    H_WR32 (0xB9100014, 0x00000000); // EUREKA CTRL5
    H_WR32 (0xB9100018, 0x00000000); // EUREKA CTRL6
    H_WR32 (0xB910001C, 0x00000000); // EUREKA CTRL7: Initialise powr sequence
}

/* wait for 200 clocks to make sure that Eureka registres are updated and 
   DDR memory chip's MR and EMR registers are updated */
static inline void ddr_ctl_polling_idle(void)
{
    volatile UINT32 rdata;
#if 0
    do {
        rdata = H_RD32(0xB91000FC);
    } while((rdata & 0x0000fffe) != 0);
    c_put_str("\r\n $DDR IDLE state");
#else
    mdelay(1);
#endif
}

static int ddr_scl_run(void)
{
    volatile UINT32 rdata;
    int error_count, i;

    // SCL continuous-read test
    H_WR32 (0xB9108100, 0x30000000); // SCL_START 0x40 //was okay
    do {
        rdata = H_RD32(0xB9108100);
    } while((rdata & 0x10000000) != 0);
    
    //write phy lane select
    H_WR32 (0xB910812C, 0x00000000);
    //write phy lane select
    H_WR32 (0xB910812C, 0x00000007);
    //write phy lane select
    H_WR32 (0xB910812C, 0x0000000E);
    //write phy lane select
    H_WR32 (0xB910812C, 0x00000015);
    
    // SCL write-read test
    H_WR32 (0xB9108100, 0x10000000); // SCL_START 0x40 //was okay
    do {
        rdata = H_RD32(0xB9108100);
    } while((rdata & 0x10000000) != 0);
    
    if ((rdata & 0x0000000F) != 0x0000000F)
        error_count++;
    
    /* data integrity test
    for(i=0; i<11; i++)
        *(H_D32 *)(0x80000000 + i*4) = i;
    for(i=0; i<11; i++)
        if(*(H_D32 *)(0x80000000+ i*4) != i)
            error_count++;*/
    return error_count;
}

int init_ddr(void)
{
    ddr_phy_init();
    ddr_phy_polling_idle();
    ddr_ctl_init();
    ddr_ctl_polling_idle();
    return ddr_scl_run();
}
#endif

/*==============================================================================
* (DDR) SDRAM System("Controller" & "PHY" & "Chip") Power-On Initialization:
* 1.The power-on sequence for most SDRAMs requires that the system power 
*   be applied with the clock running for 100 us to 200 us.
* 2.During this time "Controller" RESET must be asserted to prevent any accessing
*   to SDRAM.
* 3.user requires to control the duration of the RESET input.
* 4.Once RESET is de-asserted, user can program Controller registers
*   to set tCAS, tRAS, tWTR, DDR Mode, External Banks, Burst Length etc.
*   then Controller returns to idle state and is ready for the normal operation.
* 5.Any requests received during initialization sequence are delayed by 
*   withholding RDY_RD/WR until the controller returns to idle state.
*/

static inline void ddr_system_reset(void) 
{
    volatile UINT32 i;

    /*  enable write */
    SCU->rst_mask &= ~(RST_CLKGATE_MSK_DDR_CTRL | RST_MSK_DDR_CTRL_AHB);

#if 0 /* default upon reset */
    /*  stop clock to DDR Controller */
    SCU->clkgate  &= ~(DDR_CTRL_CLK_GATE);
#endif
    /* select System clock rather than AP clock */
    SCU->clkgate  &= ~(DDR_CTRL_CLK_SRC);
    /* active clock to DDR Controller */
    SCU->clkgate  |=  (DDR_CTRL_CLK_GATE);

#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  reset_ddr_hold");
#endif
#if 0 /* default upon reset */
    /* assert DDR Controller's RESET */
    SCU->rst_vec  &= ~(RST_CLKGATE_MSK_DDR_CTRL | RST_MSK_DDR_CTRL_AHB);
#endif

    /* delay 100~200 us */
    mdelay(1);
    
    /* bring DDR Controller out of reset */
    SCU->rst_vec  |=  (RST_CLKGATE_MSK_DDR_CTRL | RST_MSK_DDR_CTRL_AHB);
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  reset_ddr_free");
#endif

    /* disable write */
    SCU->rst_mask |=  (RST_CLKGATE_MSK_DDR_CTRL | RST_MSK_DDR_CTRL_AHB);
}

/*---------------------------------------------------------------
      H_MIPS/B_MIPS/AHB  ClocK  DataRate
        Vx185            250MHz 500MHz/4ns
        Vx183            200MHz 400MHz/5ns
      So generally DDR2 400 (3-3-3 [CL tRCD tRP]) SDRAM should be used.
  ---------------------------------------------------------------
    Micron MT47H64M16-xxx    (8M x 16bit x 8bank)
                      CL=3  CL=4  CL=5  CL=6  CL=7  tRC
                 -25E 400*  533   800   800   n/a   55ns
    Hynix H5PS5162GFR-xxC    (8M x 16bit x 4bank)
                      tCK(ns) CL tRCD tRP Unit  DataRate
                  E3     5    3   3    3   Clk    400
                  S5   2.5    5   5    5   Clk    800    ZTE NanJing used
    Elite M14D5121632A-xBG   (8M x 16bit x 4bank)
                  tCK(ns) CL tRCD tRP Unit  DataRate
                  3BG    3    5   5    5   Clk    667
                2.5BG  2.5    5/6 5/6  5/6 Clk    800    ZTE NanJing used
*/

/*
    DDR_PHY->scl_latency  = 0x00000043;
// AL=3, RL=4
    DDR_PHY->scl_cnfg1    = 0x01004342;
// WL=RL-1
    DDR_PHY->scl_cnfg2    = 0x00000301;
    DDR_PHY->pad_ctrl     = 0x00000220;
    DDR_PHY->dll_recalib  = 0x30001000;
    
    DDR_PHY->analog_dll1  = 0x01000000;
*/
static inline int ddr_phy_init(void)
{
    DDR_PHY->scl_latency  = SCL_MAIN_CLK_DLY | SCL_CAPTURE_CLK_DLY;

    DDR_PHY->scl_cnfg1    = SCL_DDR_ODT_CTRL_WR | SCL_DDR_ODT_CTRL_RD | \
                            SCL_LOCAL_ODT_CTRL | SCL_ADDITIVE_LATENCY | \
                            SCL_RD_CAS_LATENCY | SCL_DDR_TYPE | SCL_BURST_LENGTH_4;
    DDR_PHY->scl_cnfg2    = SCL_WR_CAS_LATENCY | SCL_TEST_CS;
    DDR_PHY->pad_ctrl     = CAL_NO_EXTERNAL_DLL | SCL_DQ_DQS_DRIVE;
    DDR_PHY->dll_recalib  = SCL_CTR_START_VAL | SCL_RECALIB_CNT;
    
    DDR_PHY->analog_dll1  = SCL_NO_ANALOG_DLL_WRCLK;

#if (DEBUG_LEVEL > 2)
    debugDumpRegs(DDR_PHY_BASE, 12);
#endif
}

/* wait to make sure PHY initialization is completed  */
static inline void ddr_phy_polling_idle(void)
{
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_CTL->status_perf, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_phy_polling_idle start");
#endif
    c_poll_until_set(DDR_CTL->status_perf, DDR_SCL_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_CTL->status_perf, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_phy_polling_idle exit");
#endif
}

/*
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
    DDR_CTL->prf_ctrl = 0x03208067;
#else
    DDR_CTL->prf_ctrl = 0x03208027;
#endif
    DDR_CTL->mem_cfg0 = 0x352407A0;
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
    DDR_CTL->mem_cfg1 = 0x22161424;
#else
    DDR_CTL->mem_cfg1 = 0x22163424;
#endif
    DDR_CTL->mem_cfg2 = 0xC6001542;
    DDR_CTL->mem_cfg2 = 0xC6001442;
    DDR_CTL->mem_cfg3 = 0x00000018;
    DDR_CTL->mem_cfg4 = 0x0000000d;
*/
static inline void ddr_ctl_init(UINT32 cS, UINT32 cW, UINT32 bW)
{
#ifdef DDR_AUTO_DETECT
    UINT32 a_tRFC[]={0x13, 0x1a, 0x20, 0x31};/* chipsize is 256M/512M/1G/2G bit */

    if (0 == bW)
       DDR_CTL->prf_ctrl |= DDRPHY_RDLAT_STG2_1CLK;
    
    DDR_CTL->mem_cfg0 = TIMING_RRD | TIMING_RAS | TIMING_RP | (a_tRFC[cS] << 13) | REFI_64MSEC;
    
    DDR_CTL->mem_cfg1 = TIMING_WTR | TIMING_RTP | ((cS+3) << 18) | (cW << 16) | \
                        DRAM_SYS_BANKS | ((bW+2) << 12) | TIMING_WR | TIMING_MRD | TIMING_RCD;
#else
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
    DDR_CTL->prf_ctrl |= DDRPHY_RDLAT_STG2_1CLK;
#endif
    DDR_CTL->mem_cfg0 = TIMING_RRD | TIMING_RAS | TIMING_RP | TIMING_RFC | REFI_64MSEC;
    
    DDR_CTL->mem_cfg1 = TIMING_WTR | TIMING_RTP | DRAM_CHIP_SIZE | DRAM_CHIP_WIDTH | \
                        DRAM_SYS_BANKS | DRAM_SYS_BUS_WIDTH | TIMING_WR | TIMING_MRD | TIMING_RCD;
#endif

    DDR_CTL->mem_cfg2 = WITH_EXT_REG_DELAY | MRS_DDR_TYPE | DDRPHY_RDLAT_STG1 | \
                        SLOW_POWERDOWN_EXIT | AUTO_PRECHARGE_WR | WITH_DLL_RESET | \
                        TIMING_CAS | BURST_MODE | BURST_LENGTH;

    DDR_CTL->mem_cfg2 = WITH_EXT_REG_DELAY | MRS_DDR_TYPE | DDRPHY_RDLAT_STG1 | \
                        SLOW_POWERDOWN_EXIT | AUTO_PRECHARGE_WR | \
                        TIMING_CAS | BURST_MODE | BURST_LENGTH;

    DDR_CTL->mem_cfg3 = DISABLE_OUTPUT_BUF | ENABLE_RDQS | ENABLE_nDQS | \
                        OCD_MODE | DDR2_ODT_RTT1 | DDRCTL_ADDITIVE_LATENCY | \
                        DDR2_ODT_RTT0 | DRIVE_STRENGTH_NOT_FULL | DISABLE_DLL;
    
    DDR_CTL->mem_cfg4 = FAW;

    DDR_CTL->mem_cfg5 = 0;

    DDR_CTL->mem_cfg6 = 0;
    
#if (DEBUG_LEVEL > 2)
    debugDumpRegs(DDR_CTL_BASE,8);
    debugDumpRegs(DDR_CTL_BASE+0x80,7);
#endif
    /* soft-reset to re-initialize the DDR with new parameters */
    DDR_CTL->mem_cfg7 = 0; /* Initialise power sequence */
}

/* wait for 200 clocks to make sure that Eureka registres are updated and 
   DDR memory chip's MR and EMR registers are updated */
static inline void ddr_ctl_polling_idle(void)
{
    volatile UINT32 i;
    
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_CTL->status_perf, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_ctl_polling start");
#endif
#if 0
    c_poll_until_set(DDR_CTL->status_perf, DDR_IDLE_CNTR_M << DDR_IDLE_CNTR_S);
#else
    for(i=0; i<0xfff; i++)
        ;
#endif
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_CTL->status_perf, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_ctl_polling exit");
#endif
}

static inline void ddr_scl_polling_start(void)
{
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_PHY->scl_start, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_scl_start issue");
#endif
    c_poll_until_clr(DDR_PHY->scl_start, SCL_START_GO_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&DDR_PHY->scl_start, 1);
#endif
#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n  ddr_scl_start done");
#endif
}

/* Save location 0 of DRAM into internal regs */
static inline void ddr_scl_save_offset0(void)
{
    DDR_PHY->scl_start = SCL_START_SAVE_DATA | SCL_START_RD_ONLY | SCL_START_GO_DONE;
    ddr_scl_polling_start();
}

/* Restore location 0 of DRAM from internal regs */
static inline void ddr_scl_restore_offset0(void)
{
    DDR_PHY->scl_start = SCL_START_RESTORE_DATA | SCL_START_WR_ONLY | SCL_START_GO_DONE;
    ddr_scl_polling_start();
}

/* SCL automatic tuning of data capture clock delay line setting */
static inline void ddr_scl_read_read(void)
{
    DDR_PHY->scl_start = SCL_START_CONTINUOUS_R | SCL_START_GO_DONE;
    ddr_scl_polling_start();
}

/* SCL write-read test */
static inline void ddr_scl_write_read(void)
{
    DDR_PHY->scl_start = SCL_START_GO_DONE;
    ddr_scl_polling_start();
}

static inline int ddr_scl_result_ok(UINT32 bW)
{
    UINT32 column_mask;
#ifndef DDR_AUTO_DETECT
    column_mask = SCL_CUML_SCL_RSLT;
    return column_mask == (DDR_PHY->scl_start & column_mask);
#else
    column_mask = (1 << (2*bW+2))-1;
    return column_mask == (DDR_PHY->scl_start & column_mask);
#endif
}

/* ddr is supposed to auto calibrate */
static inline UINT32 ddr_scl_run(UINT32 bW)
{
    UINT32 temp;

    //ddr_scl_save_offset0();
    ddr_scl_read_read();
    ddr_scl_write_read();
    temp = ddr_scl_result_ok(bW);
    //ddr_scl_restore_offset0();
    //c_put_dec(temp);
    return (!temp); /* return 0 if SCL succeeded */
}

/*------------------------------------------------------------------------------
* Readme:
* 1, data lines are verified with (1 << i) pattern
* 2, 
*   bank address: BA0,BA1,(BA2) :  4/8   banks
* column address: A0 ~ A9,(A11) : 1K/2K  cells
*    row address: A0 ~ A12,(A13): 8K/16K cells
* Therefore, 4*1K granularity is able to verify all address lines.
* 3, A10 is used for Auto-precharge in column address timing.
*/

/* one time access BusWidth*Burst bits, ie. 16*4-bit/8-byte */
static int ram_pattern_verify(UINT32 start, UINT32 granularity,
                                  UINT32 counter, UINT32 pattern)
{
    UINT32 end = start + granularity*counter;
    /*
    assert(1 == granularity || 2 == granularity || 0 == (granularity & 0x3));
    assert(0 == start & granularity);
    assert(0 == counter & granularity);
    */
#if ALL_ACCESS_TIMING_VERIFY
    switch (cpu_word_size)
    {
    case 1:
        H_A8 p8 = (H_A8)start;
        H_D8 pattern8 = (H_D8)pattern;
        do {
            *p8 = pattern8;
            p8 += granularity;
        } while((UINT8)p8 < end);
        p8 = (H_A8)start;
        do {
            if (*p8 != pattern8) return 1;
            else p8 += granularity;
        } while((UINT32)p8 < end);
        break;
    case 2:
        H_A16 p16 = (H_A16)start;
        H_D16 pattern16 = (H_D16)pattern;
        do {
            *p16 = pattern16;
            p16 += granularity/2;
        } while((UINT32)p16 < end);
        p16 = (H_A16)start;
        do {
            if (*p16 != pattern16) return 1;
            else p16 += granularity/2;
        } while((UINT32)p16 < end);
        break;
    case 8:
        H_A64 p64 = (H_A64)start;
        H_D64 pattern64 = (H_D64)pattern;
        do {
            *p64 = pattern64;
            p64 += granularity/8;
        } while((UINT32)p64 < end);
        p64 = (H_A64)start;
        do {
            if (*p64 != pattern64) return 1;
            else p64 += granularity/8;
        } while((UINT32)p64 < end);
        break;
    default:
#endif
        H_A32 p32 = (H_A32)start;
        H_D32 pattern32 = (H_D32)pattern;
        do {
            *p32 = pattern32;
            p32 += granularity/4;
        } while((UINT32)p32 < end);
        p32 = (H_A32)start;
        do {
            if (*p32 != pattern32) {
#if (DEBUG_LEVEL > 0)
                c_put_str(" ");
                debugDumpStaticReg((UINT32)p32, 0);
#endif
                return 1;
            } else {
#if (DEBUG_LEVEL > 2)
                if ((UINT32)p32 != start)
                    c_put_str("\b\b\b\b\b\b\b\b");/* 8 */
                c_put_hex16(((UINT32)p32 >> 16));
                c_put_hex16(((UINT32)p32));
#endif
                p32 += granularity/4;
            }
        } while((UINT32)p32 < end);
#if ALL_ACCESS_TIMING_VERIFY
        break;
    }
#endif
    return 0;
}

#if DATA_VERIFY_JACKY
static int ram_random_verify(UINT32 start, UINT32 granularity, UINT32 counter)
{
    UINT32 end = start + granularity*counter;
    /*
    assert(1 == granularity || 2 == granularity || 0 == (granularity & 0x3));
    assert(0 == start & granularity);
    assert(0 == counter & granularity);
    */
#if ALL_ACCESS_TIMING_VERIFY
    switch (ram_bus_width)
    {
    case 1:
        H_A8 p8 = (H_A8)start;
        do { 
            *p8 = ((H_D8)p8 | 0xE1) & 0x5C;
            p8 += granularity;
        } while((UINT32)p8 < end);
        p8 = (H_A8)start;
        do {
            if (*p8 != (((H_D8)p8 | 0xE1) & 0x5C))
                return 1;
            else
                p8 += granularity;
        } while((UINT32)p8 < end);
        break;
    case 2:
        H_A16 p16 = (H_A16)start;
        do {
            *p16 = (((H_D16)p16 | 0x78E1) & 0xA35C);
            p16 += granularity/2;
        } while((UINT32)p16 < end);
        p8 = (H_A16)start;
        do {
            if (*p16 != (((H_D16)p16 | 0x78E1) & 0xA35C))
                return 1;
            else
                p16 += granularity/2;
        } while((UINT32)p16 < end);
        break;
    case 8:
        H_A64 p64 = (H_A64)start;
        do {
            *p64 = (((UINT64)p64 | (UINT64)0x69D278E169D278E1) & (UINT64)0x5AC3A35C5AC3A35C);
            p64 += granularity/8;
        } while((UINT32)p64 < end);
        p8 = (H_A64)start;
        do {
            if (*p64 != (((UINT64)p64 | (UINT64)0x69D278E169D278E1) & (UINT64)0x5AC3A35C5AC3A35C))
                return 1;
            else
                p64 += granularity/8;
        } while((UINT32)p64 < end);
        break;
    default:
#endif
        H_A32 p32 = (H_A32)start;
        do {
            *p32 = (((H_D32)p32 | 0x69D278E1) & 0x5AC3A35C);
            p32 += granularity/4;
        } while((UINT32)p32 < end);
        p32 = (H_A32)start;
        do {
            if (*p32 != (((H_D32)p32 | 0x69D278E1) & 0x5AC3A35C)) {
#if (DEBUG_LEVEL > 0)
                c_put_char(' ');
                debugDumpStaticReg((UINT32)p32, 0);
#endif
                return 1;
            } else {
#if (DEBUG_LEVEL > 2)
                if ((UINT32)p32 != start)
                    c_put_str("\b\b\b\b\b\b\b\b");/* 8 */
                c_put_hex16((UINT32)p32 >> 16);
                c_put_hex16((UINT32)p32);
#endif
                p32 += granularity/4;
            }
        } while((UINT32)p32 < end);
#if ALL_ACCESS_TIMING_VERIFY
        break;
    }
#endif
    return 0;
}

static int ram_verify_complex(UINT32 start, UINT32 size)
{
    UINT32 i;
    UINT32 granularity = 4096;
    UINT32 counter = size/granularity;
    UINT32 pattern_data[] = {
        0x0000ffff, ~0x0000ffff, 0x00ff00ff, ~0x00ff00ff,
        0x0f0f0f0f, ~0x0f0f0f0f, 0x33333333, ~0x33333333,
        0x55555555, ~0x55555555, 0x76543210, ~0x76543210,
    };
    char* pattern_str [] = {
        "0000ffff", "ffff0000", "00ff00ff", "ff00ff00",
        "0f0f0f0f", "f0f0f0f0", "33333333", "cccccccc",
        "55555555", "aaaaaaaa", "76543210", "89abcdef",
    };
    /*
    assert(0 == start & granularity);
    assert(0 == counter & granularity);
    */
    for (i=0; i<sizeof(pattern_data)/sizeof(pattern_data[0]); i++)
    {
#if (DEBUG_LEVEL > 1)
        c_put_str("\r\n  write-read verification with ");
        c_put_str(pattern_str[i]);
        c_put_str(": ");
#elif (DEBUG_LEVEL > 0)
        c_put_char('.');
#endif
        if (0 != ram_pattern_verify(start, granularity, counter, pattern_data[i]))
            return 1;
    }

#if (DEBUG_LEVEL > 1)
        c_put_str("\r\n  write-read verification with ");
        c_put_str("'random'");
        c_put_str(": ");
#elif (DEBUG_LEVEL > 0)
        c_put_char('~');
#endif
    if (0 != ram_random_verify (start, granularity, counter))
        return 2;
    
    return 0;
}
#else
static int ram_verify_bitflip(UINT32 start, UINT32 size)
{
    UINT32 i;
    UINT32 granularity = 4096;
    UINT32 counter = size/granularity;
    /*
    assert(0 == start & granularity);
    assert(0 == counter & granularity);
    */
    for (i=0; i<8*(granularity/4 > 0 ? 4 : granularity); i++)
    {
#if (DEBUG_LEVEL > 1)
        c_put_str("\r\n  write-read verification with ");
        c_put_hex16((UINT16)((1 << i)>>16));
        c_put_hex16((UINT16)((1 << i)));
        c_put_str(": ");
#elif (DEBUG_LEVEL > 0)
        c_put_char('.');
#else
#endif
        if (0 != ram_pattern_verify(start, granularity, counter,  (1 << i)))
            return 1;

#if (DEBUG_LEVEL > 1)
        c_put_str("\r\n  write-read verification with ");
        c_put_hex16((UINT16)((~(1 << i))>>16));
        c_put_hex16((UINT16)(~(1 << i)));
        c_put_str(": ");
#elif (DEBUG_LEVEL > 0)
        c_put_char('~');
#else
#endif
        if (0 != ram_pattern_verify(start, granularity, counter, ~(1 << i)))
            return 2;
    }
    return 0;
}
#endif

int c_auto_ddr(void)
{
    UINT32 cS, cW, bW;
#ifdef DDR_AUTO_DETECT
    UINT32 chpSize, totalSize; /* chpSize[]={1<<25, 1<<26, 1<<27; 1<<28}; */
    UINT32 chpWidth[]={4,8,16};
    UINT32 busWidth[]={16,32}; /* DRC bank width: vx185[16/32], vx183[16] */

    //for (cS=0; cS<4; cS++) /* chpSize: MUST from small to big */
    for (cS=2; cS<3; cS++)
    {
        /* Bytes in one chip */
        chpSize = 1 << (25+cS);
        //for (cW=2; cW>=0; cW--) /* chpWidth: had better from big to small */
        for (cW=2; cW<3; cW++)
        {
            //for (bW=1; bW>=0; bW--) /* busWidth: had better from big to small */
            for (bW=1; bW<2; bW++)
            {
                /* Bytes in all chips which are paralleled together */
                totalSize = chpSize*(busWidth[bW]/chpWidth[cW]);
                if (totalSize > SYS_MEM_MAX_SIZE)
                    continue;

#if (DEBUG_LEVEL > 1)
                c_put_str("\r\n Try #");
                c_put_str(" Size:");c_put_dec(1 << (8+cS));c_put_str("Mbits");
                c_put_str("(Width:");c_put_dec(chpWidth[cW]);c_put_str("-bit");
                c_put_str("), Bus:");c_put_dec(busWidth[bW]);c_put_str("-bit");
#endif
#else /* DDR_AUTO_DETECT */
    cS = 0; cW = 0;
#if (SYS_DRAM_WIDTH == DRAM_SYS_BUS_16BIT)
    bW = 0;
#else
    bW = 1;
#endif
    do {
#endif /* DDR_AUTO_DETECT */
                ddr_system_reset();

                ddr_phy_init();
                ddr_phy_polling_idle();
                
                ddr_ctl_init(cS, cW, bW);
                ddr_ctl_polling_idle();

                c_put_str("\r\n SCL calibration:");
                if (ddr_scl_run(bW))
                {
                    c_put_str("\r\n NG");
                    continue;
                }
                c_put_str("\r\n OK");

#if defined(K0SEG_CACHED)
    #if defined(KSEG0_CACHED_NONCOHERENT)
                invalidate_dcache_range(SYS_MEM_BASE, SYS_MEM_BASE+totalSize);
    #endif
#endif
                c_put_str("\r\n data integrity:");
#if 0 //DATA_VERIFY_JACKY
                if (0 != ram_verify_complex(SYS_MEM_BASE, totalSize))
#else
                if (0 != ram_verify_bitflip(SYS_MEM_BASE, totalSize))
#endif
                {
                    c_put_str("\r\n NG");
                    continue;
                }
                c_put_str("\r\n OK");
                return 0;
#ifndef DDR_AUTO_DETECT
    } while(1);
#else
            }
        }
    }
#endif
    return 1;
}

#if 0 //JACKY_OBSOLETE_CODES
static void config_SDRAM_CAW(void)
{
    UINT32 CAW, PageSize;/* column address width: 11/10 */
    UINT32 RAW;/*  row address width: 14/13 */
    UINT32 BAW;/* bank address width: 3/2 */
    /* chpSize == 1 << (RAW+BAW+CAW), PageSize = (1 << CAW)*(busWidth/8) */

    PageSize = (1 << 8)*(busWidth/8);
try_page:
    c_put_str("\r\n Try # PageSize:");c_put_dec(PageSize);c_put_str("-Byte");
    H_WR32(SYS_MEM_BASE, 0x12340000);
    
    H_WR32(SYS_MEM_BASE+PageSize, 0x12340000+PageSize);
    if (   (SYS_MEM_BASE == H_RD32(SYS_MEM_BASE))
        && (0x12340000+PageSize == H_RD32(SYS_MEM_BASE+PageSize)) )
    {
        PageSize <<= 1;
        goto try_page;
    }
    c_put_str("\r\n # TRUE SIZE");
}


/*  Row address    Column address    Bank address
   A[11/12/13:0]      A[9:0]         BA[2/1:0]
*/
static UINT32 ram_detect_size(UINT32 start, UINT32 size)
{
    int err=0;

    /* write-read the first */
    err |= ram_pattern_verify(start, 4, 1, ~0x76543210);
    err |= ram_pattern_verify(start, 4, 1, 0x76543210);
    if (0 != err) return 1;
    
    /* write-read the last +1 */
    err |= ram_pattern_verify(start+size, 4, 1, ~0xf03c5a78);
    err |= ram_pattern_verify(start+size, 4, 1, 0xf03c5a78);
    if (0 != err) return 2;

    /* is the first changed ?  */
    /* writing mem[size] is valid and does NOT change mem[0] */
    if (0x76543210 == H_RD32(start))
        continue; /* the true size could be bigger */
    if (size < SYS_MEM_MAX_SIZE)
    {
        H_WR32(start+size, 0xf03c5a78);
        /* writing mem[size] is valid and does NOT change mem[0] */
        //debugDumpStaticReg(start+size);
        if (0xf03c5a78 != H_RD32(start+size))
        {
        }
    }
    c_put_str("\r\n        TRUE");

    /* each row.0 : A[9:0] : 2048 when 16-bit */
    if (ram_pattern_verify(start, 2048, 0x76543210, size/2048))
        return 3;

    PageSize = (1 << 8)*(busWidth[bW]/8);
    c_put_str("\r\n  6, write-read row[i][0] by  position:");
    for (position=0; position<totalSize; position+=PageSize)
        H_WR32(SYS_MEM_BASE+position, SYS_MEM_BASE+position);
    for (position=0; position<totalSize; position+=PageSize)
    {
        if (SYS_MEM_BASE+position != H_RD32(SYS_MEM_BASE+position))
        {
            //debugDumpStaticReg(SYS_MEM_BASE+position);
            status = 6;
            goto next_try;
        }
    }
    c_put_str(" PASS");

    return 0;
}

#endif
