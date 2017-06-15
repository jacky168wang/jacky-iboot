/*******************************************************************************
 * Description :
 *
 *
 * Author :
 *     spasumar@ikanos.com, 2009/06/25, create
 *       jiwang@ikanos.com, 2011/03/16, correct 'erase sector' error if image
 *                          which is used to be burn is bigger than one sector
 *                                      
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"
#include "dram_boot.h"


extern unsigned long xmodem_load(unsigned long addr);

/*------------------------------------------------------------------------------
                      D e f i n e s
------------------------------------------------------------------------------*/

#define MAX_UBOOT_SIZE (512 << 10) /* 4*0x20000=0x80000, 512KBytes */

#define RAM_DOWNLOAD_BASE (A_K1BASE + 0x00800000)

#define RAM_STARTUP_BASE  (A_K0BASE + 0x00800000)

/*------------------------------------------------------------------------------
    Public variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    Static variables
------------------------------------------------------------------------------*/

/*1* [0x00]rodata.str1.4, [0x20]data even 'static' or [0x20]rodata if 'const'
char str_RomDwld[]="\nDownload uboot !";
*/
/*2* [0x14]rodata.str1.4, [0x10]data or [0x10]rodata if 'static'
char *str_RomDwld="\nDownload uboot !";
*/
/*3* [0x14]rodata.str1.4 only
*/
#define str_RomDwld "\r\n\r\nLoad 'uboot'..."

#define str_TooBig  "\r\nfile > 512KB"

/*------------------------------------------------------------------------------
   Implementation: Static functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   Implementation: Public functions
------------------------------------------------------------------------------*/

int main(void)
{
    int status;
    UINT32 length;

    c_put_str("\r\n\r\n");
    gChipType = detect_SoC();

#if (DEBUG_LEVEL > 1)
    c_put_str("\r\n");
    detect_resetcause();
    
    c_put_str("\r\n");
    detect_bootmode();
#endif

#if defined(UARTBOOT_CODE)
    c_put_str("\r\nUartBoot 1.00 running inside CPU");
#else
    c_put_str("\r\nNandBoot 1.00 running inside CPU");
#endif
    c_put_str("\r\n  Jacky.ikanos, "__DATE__","__TIME__);

    c_put_str("\r\nPLL start");
    init_plls();
    c_put_str("\r\nPLL   end");
    
    //reset_peripheral();

    c_put_str("\r\nDDR start");
    status = c_auto_ddr();
    c_put_str("\r\nDDR   end");
    if (status)
    {
        // TODO : reset
        while(1);
    }

#if defined(UARTBOOT_CODE)
#if 0
    RemapCS0_NOR();
    c_put_str("\r\nNOR ");
    status = nor_flash_probe();
    if (0 != status)
    {
        c_put_str(str_NorDwld);
        if ('y' == cget_char_timout(0))
        {
            length = xmodem_load(RAM_DOWNLOAD_BASE);
            if (length > MAX_UBOOT_SIZE)
            {
                c_put_str(str_TooBig);
                while (1);
            }
            
            if (nor_flash_erase())
                while (1);
            if (nor_flash_burn(length))
                while (1);

            c_put_str(str_NorBoot);
            while (1);
        }
    }
    
    RemapCS0_ROM();
#endif
    c_put_str(str_RomDwld);
    length = xmodem_load(RAM_DOWNLOAD_BASE);
    if (length > MAX_UBOOT_SIZE)
    {
        c_put_str(str_TooBig);
        // TODO : reset
        while (1);
    }
    
#else

    read_uboot();

#endif

    ((VOID_FUNC)RAM_STARTUP_BASE)();

    return 0;
}
