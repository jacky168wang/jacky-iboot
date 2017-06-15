/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"

UINT32 gChipType;
UINT32 gSysClock;

UINT32 detect_SoC(void)
{
    UINT32 SoC_type;
    char *SoC_str[] = { "Vx185", "Vx175", "Vx183", "Vx173", \
        "Vx185SE", "N/A", "Vx185HP", "N/A" };

    switch (SCU->rev_num)
    {
    case 0x00030200: /* revision number of vx185 family chips */
        SoC_type = (SCU->cpu_ctl >> 4) & 0x07;
        c_put_str(SoC_str[SoC_type]);
        
        /* Find the Chip Revision: SPARE0 Register: Bit[24]: 1/0 -- B1/B0 */
        c_put_char('-');
        c_put_char('B');
        c_put_char('0'+((SCU->spare0 >> 24) & 1));
        return SoC_type;
        break;

    default:
        c_put_str("UNKNOWN");
        return H_RD32(0xB900003C);
        break;
    }
#if 0
    switch (H_RD32(0xB900003C))
    {
    case 0x6833:
        c_put_str("Vx150");
        break;
    case 0x6834:
        c_put_str("Vx200");
        break;
    case 0x6836:
        c_put_str("Vx160");
        break;
    case 0x6850:
        c_put_str("Vx180");
        break;
    case 0x6860:
        c_put_str("Vx170");
        break;
    default:
        c_put_str("UNKNOWN");
        break;
    }
    return H_RD32(0xB900003C);
#endif
}

void detect_resetcause(void)
{
    UINT32 i;
    char *reset_str[] = { "power", "soft", "watchdog", "pll_drift" };
    for (i=0;i<4;i++)
    {
        if ((1 << i) & SCU->rst_cause)
            c_put_str(reset_str[i]);
    }
}

void detect_bootmode(void)
{
    UINT32 bootmode;
    char *boot_str[] = { "NOR", "SPI", "NAND", "UART1_9600", 
                         "NOR_fix", "SPI_fix", "NAND_fix", "UART1_38400" };
    bootmode = SCU->rst_strp & 7;
    c_put_str(boot_str[bootmode]);
}

void softreset_SoC(void)
{
    c_put_str("reboot\n");
    SCU->rst_mask = 0;
    SCU->rst_vec  = 0;
}

/* remap CS0 from Bootrom/0xA0800000 to NOR flash/0xBF000000 */
void RemapCS0_NOR(void)
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
void RemapCS0_ROM(void)
{
    H_D32 temp;

    temp = *(H_A32)SCU_CTRL_MASK;
    temp &= ~0x100;
    *(H_A32)SCU_CTRL_MASK = temp;

    temp = *(H_A32)SCU_CTRL_CTRL;
    temp &= ~0x100;
    *(H_A32)SCU_CTRL_CTRL = temp;
}

/* gpio_reset_Vx185B0_board */
void reset_peripheral(void)
{
    volatile UINT32 loop = 0xA0000;
    
    if ((SCU->spare0 >> 24) & 1) /* Vx185-B1 */
    {
        SCU->rst_mask &= ~RST_MSK_EXT_PINS;
        SCU->rst_vec  |=  RST_MSK_EXT_PINS;/* reset external pins */
        while (loop--);
    }
    else /* Vx185-B0 */
    {
        /* GPIO25, Output Mode */
        H_RD32(GPIO_MODE_REG2) &= ~(3 << (25-16)*2);
        H_RD32(GPIO_MODE_REG2) |=  (1 << (25-16)*2);

        /* DeAsserting GPIO25 to reset, Active Low */
        H_RD32(GPIO_OUTPUT_CLR) |= (1 << 25);
        while (loop--);

        /* Asserting GPIO25 to release */
        H_RD32(GPIO_OUTPUT_SET) |= (1 << 25);
        while (loop--);
    }
}

