/*******************************************************************************
 * Description   :
 *
 * DO-438995-TM-1-Vx185 Silicon Details
 *
 *              /--  12M USB
 *     /- pll0 -|-- 125M GMII                  /-- pll1 -- 35.328M VCXO -- IFE7
 *     |        \-- 250M SynthesizerReference -|                            |
 *     |                                       \-- RMII 25/50/125M          |
 *     |                                                                    |
 *     |        /-- 600/250M -- clk_switch -- host_MIPS, system/AHB, MAC    |
 *     |        |-- 500M all APs (0.5 x System)     |                       |
 *     |- pll2 -|-- 500/250M DDR (1 or 2 x System)  \-- RS232/UART          |
 * 25M |        |--  80M SPI                                                |
 * OSC |        \--  40M SPI slow                                          /
 *     |                                                             AFE_RXCLK_I 
 *     |        /-- 500M BME_MIPS -- (2M~10M)4.4M AFE_SCK,AFE_SDA         |
 *     |- pll3 -|                    control 2.2M AFE_CSi          AFE_TXCLK70_O
 *     |        \-- 214M Frequence Domain
 *     |
 *     |        /-- 300M DSP   --> 2.048M or 4.096M PCM/SPORT (<-- External)
 *     \- pll4 -|
 *              \-- 104M SPI fast
 *
 * host/BME/APs clock: 25M *    1*(feedback divider in 0x1900_002C)/(FD1*FD2)
 *  SYStem(AHB) clock: 25M *  0.5*(feedback divider in 0x1900_002C)/(FD1*FD2)
 *          DSP clock: 25M *10.01*(feedback divider in 0x1900_002C)/(FD1*FD2)
 *
 * Author :
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"


/*==============================================================================
* (fixed)PLL0
*/
void init_pll0_fixed(void)
{
    /* configure analog: nom VCO speed (11111, default) */
    SCU->pll_fixed_analog = 0x003fc442;

    /* NO sigma-delta */

    /* enable clocks for GIGE, USB */
    SCU->pll_post_divider3 |=  (ENABLE_USB_CLOCK | ENABLE_GIGE_CLOCK);
    
    /* calibrate: NO sigma-delta; NO post-divider */
    SCU->pll_pwr_rst &= ~(0x3f << 0);
    SCU->pll_pwr_rst |=  (PLL_FIXED_CALIBRATION | PLL_FIXED_LOCKDETECTOR_DOWN);
    SCU->pll_pwr_rst |=  (PLL_FIXED_POWER_UP);
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_CALIBRATION_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
    /* lock-detector up */
    SCU->pll_pwr_rst &= ~(0x3f << 0);
    SCU->pll_pwr_rst |=  (PLL_FIXED_POWER_UP);
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_LOCKED);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
}

/*==============================================================================
* (gateway)PLL1
*/
void init_pll1_gateway(void)
{
    /* configure analog: nom VCO speed (01100, default) */
    SCU->pll_gateway_analog = 0x002cc642;

    /* NO sigma-delta */

    /* bypass sigma-delta */
    SCU->pll_post_divider3 |=  (BYPASS_SIGMADELTA_GATEWAY_PLL);
    /* div2 */
    SCU->pll_post_divider3 &= ~(BYPASS_DIV2_GATEWAY_PLL);
    
    /* enable clocks for host_mips, system, APs */
    SCU->pll_post_divider3 |=  (ENABLE_SYS_CLOCK | ENABLE_AP_CLOCK | ENABLE_HOST_MIPS_CLOCK);
    /* enable clocks for SPIs */
    SCU->pll_post_divider3 |=  (ENABLE_SPI_SLOW_CLOCK | ENABLE_SPI0_CLOCK);
    
    /* calibrate: sigma-delta down; post-divider up */
    SCU->pll_pwr_rst &= ~(0x3f << 12);
    SCU->pll_pwr_rst |=  (PLL_GATEWAY_SIGMADELTA_DOWN | PLL_GATEWAY_POSTDIVIDER_UP);
    SCU->pll_pwr_rst |=  (PLL_GATEWAY_CALIBRATION | PLL_GATEWAY_LOCKDETECTOR_DOWN);
    SCU->pll_pwr_rst |=  (PLL_GATEWAY_POWER_UP);
    c_poll_until_set(SCU->pll_gateway_status, PLL_STAT_CALIBRATION_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
    /* lock-detector up */
    SCU->pll_pwr_rst &= ~(0x3f << 12);
    SCU->pll_pwr_rst |=  (PLL_GATEWAY_SIGMADELTA_DOWN | PLL_GATEWAY_POSTDIVIDER_UP);
    SCU->pll_pwr_rst |=  (PLL_GATEWAY_POWER_UP);
    c_poll_until_set(SCU->pll_gateway_status, PLL_STAT_LOCKED);

    /* must re-initialize DLAB before any more print-out */
    c_uart_init();

#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
}

#if !defined(UARTBOOT_CODE)
/*==============================================================================
* (VCXO)PLL2
*/
void init_pll2_vcxo(void)
{
    /* configure analog: [25:24]11: Pad Strength, [28]1: CMOS Pad Enable */
#if (VCXO_SE_MODE == 1)
    SCU->pll_vcxo_analog = 0x131cca88;
#else
    SCU->pll_vcxo_analog = 0x001cca88;
#endif
    /* configure sigma-delta: */
    SCU->pll_vcxo_sigmadelta = 0x001654d6;

    /* enable clock for Synthesizer Reference to VCXO */
    SCU->pll_post_divider3 |=  (ENABLE_SYN_REF_CLOCK);
    /* enable clock for VCXO to AFE */
    SCU->pll_post_divider3 |=  (ENABLE_VCXO_CLOCK);

    /* Strap input = AMC_DATA_B_O[10] */
    if (SCU->rst_strp & SCU_RST_STRAP_AFE6_ENABLE) /* 0x80 ? */
    {
        /* calibrate: sigma-delta up; post-divider down */
        SCU->pll_pwr_rst &= ~(0x3f << 6);
        SCU->pll_pwr_rst |=  (PLL_VCXO_CALIBRATION | PLL_VCXO_LOCKDETECTOR_DOWN);
        SCU->pll_pwr_rst |=  (PLL_VCXO_POWER_UP);
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_CALIBRATION_DONE);
#if (DEBUG_LEVEL > 2)
        debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
        /* lock-detector up */
        SCU->pll_pwr_rst &= ~(0x3f << 6);
        SCU->pll_pwr_rst |=  (PLL_VCXO_POWER_UP);
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_LOCKED);
#if (DEBUG_LEVEL > 2)
        debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
    }
    else
    {
        /* Feeds the clock from the VCXO_I pad through to the BME and back out
        on the differential VCXO_P/N_O pads */
        SCU->cpu_ctl_mask &= ~(SCU_CTRL_VCXO_COMMON_CLOCK);
        SCU->cpu_ctl      |=  (SCU_CTRL_VCXO_COMMON_CLOCK);
        SCU->cpu_ctl_mask |=  (SCU_CTRL_VCXO_COMMON_CLOCK);

    	/* Power-up the synth PLL: sigma-delta down; post-divider down */
        SCU->pll_pwr_rst &= ~(0x3f << 6);
        SCU->pll_pwr_rst |=  (PLL_VCXO_SIGMADELTA_DOWN);
        SCU->pll_pwr_rst |=  (PLL_VCXO_LOCKDETECTOR_DOWN);
        SCU->pll_pwr_rst |=  (PLL_VCXO_POWER_UP);
    }
}

/*==============================================================================
* (BME)PLL3
*/
void init_pll3_bme(void)
{
    /* configure analog: nom VCO speed (01100, default) */
    SCU->pll_bme_analog = 0x002cc642;

    /* NO sigma-delta */

    /* bypass sigma-delta */
    SCU->pll_post_divider3 |=  (BYPASS_SIGMADELTA_BME_PLL);
    /* div2 */
    SCU->pll_post_divider3 &= ~(BYPASS_DIV2_BME_PLL);
    
    /* enable clock for bme_mips, freq_domain */
    SCU->pll_post_divider3 |= (ENABLE_BME_MIPS_CLOCK | ENABLE_BME_FD_CLOCK);

    /* calibrate: sigma-delta down; post-divider up */
    SCU->pll_pwr_rst &= ~(0x3f << 18);
#if 1
    SCU->pll_pwr_rst |=  (PLL_BME_SIGMADELTA_DOWN | PLL_BME_POSTDIVIDER_UP);
#else
    SCU->pll_pwr_rst |=  (PLL_BME_SIGMADELTA_DOWN);
#endif
    SCU->pll_pwr_rst |=  (PLL_BME_CALIBRATION | PLL_BME_LOCKDETECTOR_DOWN);
    SCU->pll_pwr_rst |=  (PLL_BME_POWER_UP);
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_CALIBRATION_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
    /* lock-detector up */
    SCU->pll_pwr_rst &= ~(0x3f << 18);
    SCU->pll_pwr_rst |=  (PLL_BME_SIGMADELTA_DOWN | PLL_BME_POSTDIVIDER_UP);
    SCU->pll_pwr_rst |=  (PLL_BME_POWER_UP);
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_LOCKED);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
}

/*==============================================================================
* (DSP)PLL4
*/
void init_pll4_dsp(void)
{
    /* configure analog: nom VCO speed (00110, default) */
    SCU->pll_dsp_analog = 0x0026ce20;

    /* configure sigma-delta: [0x003e2d62]294.912, [0x001c9f73]212.992, [0x00012b52]229.376 MHz */
    SCU->pll_dsp_sigmadelta = 0x00012b52;

    /* does NOT bypass sigma-delta */
    SCU->pll_post_divider3 &= ~(BYPASS_SIGMADELTA_DSP_PLL);
    /* div2 */
    SCU->pll_post_divider3 &= ~(BYPASS_DIV2_DSP_PLL);
    
    /* enable clock for voice_dsp */
    SCU->pll_post_divider3 |= (ENABLE_DSP_CLOCK);

    /* calibrate: sigma-delta up; post-divider up */
    SCU->pll_pwr_rst &= ~(0x3f << 24);
    SCU->pll_pwr_rst |=  (PLL_DSP_POSTDIVIDER_UP);
    SCU->pll_pwr_rst |=  (PLL_DSP_CALIBRATION | PLL_DSP_LOCKDETECTOR_DOWN);
    SCU->pll_pwr_rst |=  (PLL_DSP_POWER_UP);
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_CALIBRATION_DONE);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
    /* lock-detector up */
    SCU->pll_pwr_rst &= ~(0x3f << 24);
    SCU->pll_pwr_rst |=  (PLL_DSP_POSTDIVIDER_UP);
    SCU->pll_pwr_rst |=  (PLL_DSP_POWER_UP);
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_LOCKED);
#if (DEBUG_LEVEL > 2)
    debugDumpVolatileReg((UINT32)&SCU->pll_pwr_rst, 1);
#endif
}
#endif

/*==============================================================================
*
*/
#if 1
void init_plls(void)
{
	/* Program the BME window for 256 MB flash at (256-8)=248 MB */
    SCU->bme_map0 = 0xAF800000;

	/* Configure GMII2 bit to 01 */
    SCU->cpu_ctl_mask = 0xfffcfeff;
    SCU->cpu_ctl      = 0x00020100;
    SCU->cpu_ctl_mask = 0xffffffff;

	/* Enable clocks and bring modules out of reset */
    SCU->rst_mask = 0xC7DCF370;
    SCU->clkgate  = 0x08FFFFFC; //bit #28 0-25MHz, 1-35.328MHz
    SCU->rst_vec  = 0x18230C8F;

    /* Internal POR VDD monitoring threshold should be set to 20% BZ:23348 */
    SCU->por_status = 0x20;
    
    /* Skip programming PLL if "SOFT RESET" */
    if (LAST_RESET_WAS_SOFT)
        return;
    
    /* Vx175 and Vx185HP: BME PLL will be source for the Host MIPS 600MHz */
    if ((CHIP_7175 == gChipType) || (CHIP_7185HP == gChipType))
        SCU->spare0 = 0x4009;
    
    /* DSP: 25*10.01*11/(6*2)=229M */
    /* BME, FD: 25*1*96/(3*2)=400M, 25*1*96/(6*2)=200M */
    /* 183/173/185HP: host, system: 25*1*96/(3*2)=400M, 25*0.5*96/(3*2)=200M */
    /* 185/175/185SE: host, system: 25*1*80/(2*2)=500M, 25*0.5*80/(2*2)=250M */
    if ((CHIP_7173 == gChipType) || (CHIP_7183 == gChipType) || (CHIP_7185HP == gChipType))
    {
        SCU->pll_post_divider1 = 0x180b6060;
        SCU->pll_post_divider2 = 0x06063033;
        gSysClock = 200*1000000;
    }
    else
    {
        SCU->pll_post_divider1 = 0x180b6050;
        SCU->pll_post_divider2 = 0x06063022;
        gSysClock = 250*1000000;
    }

    init_pll0_fixed();
    init_pll1_gateway();

#if !defined(UARTBOOT_CODE)
    init_pll2_vcxo();
    init_pll3_bme();
    init_pll4_dsp();
#endif

#if (DEBUG_LEVEL > 2)
    debugDumpRegs(SCU_BASE,64);
#endif
}
#else
void init_plls(void)
{
	/* Program the BME window for 256 MB flash at (256-8)=248 MB */
    /* BME Window_1 Base register to used to convey the BME window start information */
    SCU->bme_map0 = 0xAF800000;

	/* Configure GMII2 bit to 01 */
    SCU->cpu_ctl_mask = 0xfffcfeff;
    SCU->cpu_ctl = 0x00020100;
    SCU->cpu_ctl_mask = 0xffffffff;

	/* Enable clocks and bring modules out of reset */
    SCU->rst_mask = 0xC7DCF370;
    SCU->clkgate  = 0x08FFFFFC; //bit #28 0-25MHz, 1-35.328MHz
    SCU->rst_vec  = 0x18230C8F;
	
   	/* BZ:23348 Changing Internal POR threshold to 20% */
    SCU->por_status = 0x20;

	/* Need to check if the Chip is 185 or 183 */
    if (LAST_RESET_WAS_SOFT)
       return;

	/* nom fixed PLL0 VCO speed (11111, default) */
    SCU->pll_fixed_analog = 0x003fc442;
#if (VCXO_SE_MODE == 1)
    SCU->pll_vcxo_analog = 0x131cca88;
#else
    SCU->pll_vcxo_analog = 0x001cca88;
#endif
   	/* nom VCO speed (01100, default) for gateway pll */
	SCU->pll_gateway_analog = 0x002cc642;

   	/* nom VCO speed (00110, default) for dsp pll */
    SCU->pll_dsp_analog = 0x0026ce20;

   	/* nom VCO speed (01100, default) for bme pll */
    SCU->pll_bme_analog = 0x002cc642;

   	/* Calibrate GW PLL - sys: /2+/4 (250 MHz), MIPS: /2+/2 (500 MHz),
      	DSP: /5+/2 (196 MHz), BME: /2+/2 (500 MHz), FD: /5+/2 (196 MHz) */
    if ((CHIP_7173 == gChipType) || (CHIP_7183 == gChipType) || (CHIP_7185HP == gChipType))
    {
        SCU->pll_post_divider1 = 0x180b6060;
        SCU->pll_post_divider2 = 0x06063033;
        gSysClock = 200*1000000;
    }
    else
    {
        SCU->pll_post_divider1 = 0x180b6050;
        SCU->pll_post_divider2 = 0x06063022;
        gSysClock = 250*1000000;
    }

    //SCU->pll_dsp_sigmadelta = 0x003E2D62; //for 294.912 MHz
    SCU->pll_dsp_sigmadelta = 0x00012B52; //for 229.376 MHz with 25 MHz Ref
    //SCU->pll_dsp_sigmadelta = 0x001C9F73; //for 212.992 MHz

	/* VCXO sigma delta control */
    SCU->pll_vcxo_sigmadelta = 0x001654D6;

	/* All clocks on (sigma delta disabled for gateway and bme) */
    SCU->pll_post_divider3 = 0x4D600FFF;

	/* Fixed PLL calibration */
    //SCU->pll_pwr_rst = 0x0A28B287;
    SCU->pll_pwr_rst &= ~0xf;
    SCU->pll_pwr_rst |=  0x7;
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A28B281;
    SCU->pll_pwr_rst &= ~0xf;
    SCU->pll_pwr_rst |=  0x1;
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_LOCKED);

    /* Check whether to enable Common Clock or not */
    if (SCU->rst_strp & 0x80) /* 0/1: Common/differential Clock */
    {
        /* Synth PLL */
        //SCU->pll_pwr_rst = 0x0A28B1C1;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x1c0;
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_CALIBRATION_DONE);
        //SCU->pll_pwr_rst = 0x0A28B041;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x040;
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_LOCKED);
    }
    else
    {
    	/* Enable VCXO for Common Clock */
        SCU->cpu_ctl_mask &= ~0x200000;
        SCU->cpu_ctl      |=  0x200000;
        SCU->cpu_ctl_mask |=  0x200000;

    	/* Power up the synth PLL */
        //SCU->pll_pwr_rst = 0x0A28B1C1;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x2c0;
    }
    
	/* BME PLL */
    //SCU->pll_pwr_rst = 0x0A3CB041;
    SCU->pll_pwr_rst &= ~0xfc0000;
    SCU->pll_pwr_rst |=  0x3c0000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A64B041;
    SCU->pll_pwr_rst &= ~0xfc0000;
    SCU->pll_pwr_rst |=  0x640000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_LOCKED);

   	/* Gateway PLL */
    //SCU->pll_pwr_rst = 0x0A65F041;
    SCU->pll_pwr_rst &= ~0x1f000;
    SCU->pll_pwr_rst |=  0x1f000;
    c_poll_until_set(SCU->pll_gateway_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A659041;
    SCU->pll_pwr_rst &= ~0x1f000;
    SCU->pll_pwr_rst |=  0x19000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_LOCKED);

    /* must re-initialize DLAB before any more print-out */
    c_uart_init();

   	/* DSP PLL */
    //SCU->pll_pwr_rst = 0x17659041;
    SCU->pll_pwr_rst &= ~0xff000000;
    SCU->pll_pwr_rst |=  0x17000000;
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x11659041;
    SCU->pll_pwr_rst &= ~0xff000000;
    SCU->pll_pwr_rst |=  0x11000000;
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_LOCKED);
}
#endif

#if 0
{
    /* Skip the PLL programming if the reset cause shows reason as "SOFT RESET"*/
    if (!(LAST_RESET_WAS_SOFT))
    {
   	/* BZ:23348 Changing Internal POR threshold to 20% */
    SCU->por_status = 0x20;

        /* Vx175 and Vx185HP: BME PLL will be source for the Host MIPS 600MHz */
        if ((CHIP_7175 == gChipType) || (CHIP_7185HP == gChipType))
            SCU->spare0 = 0x4009;

    /* Program the BME window for 256 MB flash at (256-8)=248 MB */
    /* BME Window_1 Base register to used to convey the BME window start information */
    SCU->bme_map0 = 0xAF800000;

	/* Configure GMII2 bit to 01 */
    SCU->cpu_ctl_mask = 0xfffcfeff;
    SCU->cpu_ctl = 0x00020100;
    SCU->cpu_ctl_mask = 0xffffffff;

    /* Enable clocks and bring modules out of reset */
    SCU->rst_mask = 0xc7dcf370;
    SCU->clkgate  = 0x08fffffc; //bit #28 0-25MHz, 1-35.328MHz
    SCU->rst_vec  = 0x18230c8f;

        /* DSP: 25*10.01*11/(6*2)=229M */
        /* BME, FD: 25*1*96/(3*2)=400M, 25*1*96/(6*2)=200M */
        /* 183/173/185HP: host, system: 25*1*96/(3*2)=400M, 25*0.5*96/(3*2)=200M */
        /* 185/175/185SE: host, system: 25*1*80/(2*2)=500M, 25*0.5*80/(2*2)=250M */
    if ((CHIP_7173 == gChipType) || (CHIP_7183 == gChipType) || (CHIP_7185HP == gChipType))
    {
        SCU->pll_post_divider1 = 0x180b6060;
        SCU->pll_post_divider2 = 0x06063033;
        gSysClock = 200*1000000;
    }
    else
    {
        SCU->pll_post_divider1 = 0x180b6050;
        SCU->pll_post_divider2 = 0x06063022;
        gSysClock = 250*1000000;
    }

#if 1   /* (fixed)PLL0 */
    /* nom VCO speed (11111, default) */
    SCU->pll_fixed_analog = 0x003fc442;

    /* Fixed PLL calibration */
    //SCU->pll_pwr_rst = 0x0A28B287;
    SCU->pll_pwr_rst &= ~(0x1f << 0);
    SCU->pll_pwr_rst |=  (0x7 << 0);
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A28B281;
    SCU->pll_pwr_rst &= ~(0x1f << 0);
    SCU->pll_pwr_rst |=  (0x1 << 0);
    c_poll_until_set(SCU->pll_fixed_status, PLL_STAT_LOCKED);
#endif

#if 1   /* (gateway)PLL1 */
   	/* nom VCO speed (01100, default) for gateway pll */
	SCU->pll_gateway_analog = 0x002cc642;

	/* All clocks on (sigma delta disabled for gateway and bme) */
    SCU->pll_post_divider3 = 0x4d600fff;

   	/* Gateway PLL */
    //SCU->pll_pwr_rst = 0x0A65F041;
    SCU->pll_pwr_rst &= ~0x1f000;
    SCU->pll_pwr_rst |=  0x1f000;
    c_poll_until_set(SCU->pll_gateway_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A659041;
    SCU->pll_pwr_rst &= ~0x1f000;
    SCU->pll_pwr_rst |=  0x19000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_LOCKED);
#endif

#if 1   /* (VCXO)PLL2 */
#if (VCXO_SE_MODE == 1)
    SCU->pll_vcxo_analog = 0x131cca88;
#else
    SCU->pll_vcxo_analog = 0x001cca88;
#endif
	/* VCXO sigma delta control */
    SCU->pll_vcxo_sigmadelta = 0x001654d6;

    /* Check whether to enable Common Clock or not */
    if (SCU->rst_strp & 0x80) /* 0/1: Common/differential Clock */
    {
        /* Synth PLL */
        //SCU->pll_pwr_rst = 0x0A28B1C1;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x1c0;
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_CALIBRATION_DONE);
        //SCU->pll_pwr_rst = 0x0A28B041;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x040;
        c_poll_until_set(SCU->pll_vcxo_status, PLL_STAT_LOCKED);
    }
    else
    {
    	/* Enable VCXO for Common Clock */
        SCU->cpu_ctl_mask &= ~0x200000;
        SCU->cpu_ctl      |=  0x200000;
        SCU->cpu_ctl_mask |=  0x200000;

    	/* Power up the synth PLL */
        //SCU->pll_pwr_rst = 0x0A28B1C1;
        SCU->pll_pwr_rst &= ~0xff0;
        SCU->pll_pwr_rst |=  0x2c0;
    }
#endif

#if 1   /* (BME)PLL3 */
   	/* nom VCO speed (01100, default) for bme pll */
    SCU->pll_bme_analog = 0x002cc642;

	/* BME PLL */
    //SCU->pll_pwr_rst = 0x0A3CB041;
    SCU->pll_pwr_rst &= ~0xfc0000;
    SCU->pll_pwr_rst |=  0x3c0000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x0A64B041;
    SCU->pll_pwr_rst &= ~0xfc0000;
    SCU->pll_pwr_rst |=  0x640000;
    c_poll_until_set(SCU->pll_bme_status, PLL_STAT_LOCKED);
#endif

#if 1   /* (DSP)PLL4 */
    /* nom VCO speed (00110, default) for dsp pll */
    SCU->pll_dsp_analog = 0x0026ce20;

    //SCU->pll_dsp_sigmadelta = 0x003e2d62; //for 294.912 MHz
    SCU->pll_dsp_sigmadelta = 0x00012b52; //for 229.376 MHz with 25 MHz Ref
    //SCU->pll_dsp_sigmadelta = 0x001c9f73; //for 212.992 MHz

	/* All clocks on (sigma delta disabled for gateway and bme) */
    SCU->pll_post_divider3 = 0x4D600FFF;

   	/* DSP PLL */
    //SCU->pll_pwr_rst = 0x17659041;
    SCU->pll_pwr_rst &= ~0xff000000;
    SCU->pll_pwr_rst |=  0x17000000;
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_CALIBRATION_DONE);
    //SCU->pll_pwr_rst = 0x11659041;
    SCU->pll_pwr_rst &= ~0xff000000;
    SCU->pll_pwr_rst |=  0x11000000;
    c_poll_until_set(SCU->pll_dsp_status, PLL_STAT_LOCKED);
#endif
    }
}
#endif
