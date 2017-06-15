/*******************************************************************************
 * Description :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _GENERAL_ENDIAN_H_
#define _GENERAL_ENDIAN_H_

#if !defined(BYTE)
    #define BYTE(d,n) (((d) >> ((n) << 3)) & 0xff)
#endif

/* Most/Least significant 32 bit from 64 bit double word */
#define HI32(data64) ((UINT32)((data64 >> 32) & 0xffffffff))
#define LO32(data64) ((UINT32)((data64 >>  0) & 0xffffffff))

#define HI16(data32) ((UINT16)((data32 >> 16) & 0xffff))
#define LO16(data32) ((UINT16)((data32 >>  0) & 0xffff))

/* Endian related macros */
#define SWAP_BYTEADDR32( addr )   ( ((addr) & ~0x3) | (3 - ((addr) & 0x3)) )
#define SWAP_UINT16ADDR32( addr ) ( ((addr) & ~0x2) | (2 - ((addr) & 0x2)) )

#ifdef EL
    #define REGW32LE(addr, data) do { H_RD32(addr) = (data); } while(0)
    #define REGR32LE(addr, data) do { (data) = H_RD32(addr); } while(0)
#else
    #define REGW32LE(addr, data) do { H_RD32(addr) = SWAPEND32(data); } while(0)
    #define REGR32LE(addr, data) do { (data) = H_RD32(addr); (data) = SWAPEND32(data); } while(0)
#endif

#ifdef EB
    #define REGW32BE(addr, data) do { H_RD32(addr) = (data); } while(0)
    #define REGR32BE(addr, data) do { (data) = H_RD32(addr); } while(0)
#else
    #define REGW32BE(addr, data) do { H_RD32(addr) = SWAPEND32(data); } while(0)
    #define REGR32BE(addr, data) do { (data) = H_RD32(addr); (data) = SWAPEND32(data); } while(0)
#endif

/* Set of 'LE'-macros, convert by BE: */
#ifdef EL
    #define CPU_TO_LE32( value ) (value)
    #define LE32_TO_CPU( value ) (value)
    #define CPU_TO_LE16( value ) (value)
    #define LE16_TO_CPU( value ) (value)
#else
    #define CPU_TO_LE32( value ) ( (            ((UINT32)value)  << 24) |   \
                               ((0x0000FF00UL & ((UINT32)value)) <<  8) |   \
                               ((0x00FF0000UL & ((UINT32)value)) >>  8) |   \
                               (                ((UINT32)value)  >> 24)   )
    #define LE32_TO_CPU( value ) CPU_TO_LE32( value )
    #define CPU_TO_LE16( value ) ( ((UINT16)(((UINT16)value)  << 8)) |   \
                                   ((UINT16)(((UINT16)value)  >> 8))   )
    #define LE16_TO_CPU( value ) CPU_TO_LE16( value )
#endif

/* Set of 'BE'-macros, convert by LE: */
#ifdef EB
    #define CPU_TO_BE32( value ) (value)
    #define BE32_TO_CPU( value ) (value)
    #define CPU_TO_BE16( value ) (value)
    #define BE16_TO_CPU( value ) (value)
#else
    #define CPU_TO_BE32( value ) ( (            ((UINT32)value)  << 24) |   \
                               ((0x0000ff00UL & ((UINT32)value)) <<  8) |   \
                               ((0x00ff0000UL & ((UINT32)value)) >>  8) |   \
                               (                ((UINT32)value)  >> 24)   )
    #define BE32_TO_CPU( value ) CPU_TO_BE32( value )
    #define CPU_TO_BE16( value ) ( ((UINT16)(((UINT16)value)  << 8)) |   \
                                   ((UINT16)(((UINT16)value)  >> 8))   )
    #define BE16_TO_CPU( value ) CPU_TO_BE16( value )
#endif

#endif /* _GENERAL_ENDIAN_H_ */
