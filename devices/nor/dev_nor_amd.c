/*******************************************************************************
 * Description :
 *     driver for AMD CommandSet NOR flash
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"

/*****************************************************************************
                        T y p e s
*****************************************************************************/

/*****************************************************************************
                      D e f i n e s
*****************************************************************************/

#define RAM_IMAGE_BASE 0xA0800000

#if defined(VX185_A1)
    #define NOR_FLASH_BASE  0xBFC00000
#elif defined(VX185_B0) || defined(VX183)
    #define NOR_FLASH_BASE  0xBF000000
#endif

//#define ZTENJ_8MbootNOR

#ifdef ZTENJ_8MbootNOR
    #define FLASH_BLOCK_SIZE1      (  8 << 10) /* 0x02000,  8KBytes */
    #define FLASH_BLOCK_SIZE       ( 64 << 10) /* 0x10000,  64KBytes */
#else
    #define FLASH_BLOCK_SIZE       (128 << 10) /* 0x20000, 128KBytes */
#endif
#define MAX_UBOOT_SIZE (512 << 10) /* 4*0x20000=0x80000, 512KBytes */

#define NOR_FLASH_RD(off)      *(H_A16)(NOR_FLASH_BASE + (off))
#define NOR_FLASH_WR(off, val) *(H_A16)(NOR_FLASH_BASE + (off)) = val

#define RAM_IMAGE_RD(off)      *(H_A16)(RAM_IMAGE_BASE + (off))
#define RAM_IMAGE_WR(off, val) *(H_A16)(RAM_IMAGE_BASE + (off)) = val

/************************************************************************
 *  Public variables
 ************************************************************************/

/************************************************************************
 *  Static variables
 ************************************************************************/

/************************************************************************
 *  Implementation : Static functions
 ************************************************************************/

/************************************************************************
 *  Implementation : Public functions
 ************************************************************************/
#define NOR_FLASH_RESET NOR_FLASH_WR(0x000, 0x00f0)
#define NOR_FLASH_AUTOSELECT \
do {\
    NOR_FLASH_WR(0xAAA, 0x00aa);\
    NOR_FLASH_WR(0x554, 0x0055);\
    NOR_FLASH_WR(0xAAA, 0x0090);\
} while(0)

#if 0
int nor_flash_probe(void)
{
    UINT16 temp;

    c_put_str("Probe: ");
    
    NOR_FLASH_RESET;
    NOR_FLASH_AUTOSELECT;
    c_put_str("id=");
    c_put_hex16(temp=NOR_FLASH_RD(0x00<<1));
    c_put_char(' ');
    c_put_hex16(NOR_FLASH_RD(0x01<<1));
    c_put_char(' ');
    c_put_hex16(NOR_FLASH_RD(0x0E<<1));
    c_put_char(' ');
    c_put_hex16(NOR_FLASH_RD(0x0F<<1));
    c_put_str(", protect=");
    c_put_hex16(NOR_FLASH_RD(0x02<<1));
    NOR_FLASH_RESET;
    if ((0!=(temp&0xff00)) | (0xff==(temp&0xff)) | (0==(temp&0xff)))
    {
        c_put_str(":AmcBusError!");
        return 1;
    }
    c_put_str(":YES?");
    
    /* TODO: nor flash unlock : is going to be added in case sectors are locked */

    return 0;
}
#endif

#if 0
/* nor flash erase : offset 0 ~ 0x80000 (512KByte) */
int nor_flash_erase(void)
{
    UINT32 temp, x;

    c_put_str("Erasing: 0x00000");
    temp = 0;
    do {
        NOR_FLASH_WR(0xAAA, 0x00aa);
        NOR_FLASH_WR(0x554, 0x0055);
        NOR_FLASH_WR(0xAAA, 0x0080);
        NOR_FLASH_WR(0xAAA, 0x00aa);
        NOR_FLASH_WR(0x554, 0x0055);
        NOR_FLASH_WR( temp, 0x0030);
#ifdef ZTENJ_8MbootNOR
        if (temp < (64<<10))
        {
            //c_put_char('.'); /* for boot sector */
            temp += FLASH_BLOCK_SIZE1;
        }
        else
        {
#endif
            //c_put_char('#'); /* outchar every sector */
            temp += FLASH_BLOCK_SIZE;
#ifdef ZTENJ_8MbootNOR
        }
#endif
        mdelay(2000);  /* sector erase timeout: typical 0.5s, max 3.5s */

        c_put_str("\b\b\b\b\b");
        x = (temp>>16) & 0x0f;
        c_put_char(x<10 ? '0'+x : 'a'+x);
        c_put_hex16(temp);

        if (0xffff != NOR_FLASH_RD(temp))
        {
            c_put_str(":Fail!");
            return 1;
        }
    } while(temp < MAX_UBOOT_SIZE);

    return 0;
}

// nor flash burn : write image from DRAM
int nor_flash_burn(UINT32 length)
{
    UINT32 temp, x;

    c_put_str("Burning: 0x00000");
    temp = 0;
    do {
        NOR_FLASH_WR(0xAAA, 0x00aa);
        NOR_FLASH_WR(0x554, 0x0055);
        NOR_FLASH_WR(0xAAA, 0x00a0);
        NOR_FLASH_WR(temp, RAM_IMAGE_RD(temp))
        mdelay(1); /* program word timeout: typical 60us */
        if (NOR_FLASH_RD(temp) != RAM_IMAGE_RD(temp))
        {
            c_put_str(":Fail!");
            return 1;
        }
        temp += 2;
        if (0 == (temp & (0x1000-1))) /* outchar every 4KByte or finished */
        {
            c_put_str("\b\b\b\b\b");
            x = (temp>>16) & 0x0f;
            c_put_char(x<10 ? '0'+x : 'a'+x);
            c_put_hex16(temp);
        }
    } while(temp < length);

    return 0;
}
#endif
