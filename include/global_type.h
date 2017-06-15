/*******************************************************************************
 * Description :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _GLOBAL_TYPE_H_
#define _GLOBAL_TYPE_H_

#ifndef __ASSEMBLY__

typedef unsigned char       UINT8;
typedef   signed char       SINT8;
typedef unsigned short     UINT16;
typedef   signed short     SINT16;
typedef unsigned int       UINT32;
typedef   signed int       SINT32;
typedef unsigned long long UINT64;
typedef   signed long long SINT64;

typedef          int        SBOOL;

typedef volatile UINT8     H_D8;
typedef volatile UINT8 *   H_A8;
typedef volatile UINT16   H_D16;
typedef volatile UINT16 * H_A16;
typedef volatile UINT32   H_D32;
typedef volatile UINT32 * H_A32;
typedef volatile UINT64   H_D64;
typedef volatile UINT64 * H_A64;

#define H_RD8(x)         (*(H_A8)(x))
#define H_WR8(x,y)  do { *(H_A8)(x)  = y; } while (0)
#define H_RD16(x)        (*(H_A16)(x))
#define H_WR16(x,y) do { *(H_A16)(x) = y; } while (0)
#define H_RD32(x)        (*(H_A32)(x))
#define H_WR32(x,y) do { *(H_A32)(x) = y; } while (0)
#define H_RD64(x)        (*(H_A64)(x))
#define H_WR64(x,y) do { *(H_A64)(x) = y; } while (0)

#define c_poll32_until_clr(addr, bitmsk) while (H_RD32(addr) & (bitmsk))
#define c_poll32_until_set(addr, bitmsk) while ((bitmsk) != (H_RD32(addr) & (bitmsk)))

#define c_poll_until_clr(member_read, bitmsk) while ((member_read) & (bitmsk))
#define c_poll_until_set(member_read, bitmsk) while ((bitmsk) != ((member_read) & (bitmsk)))

#define FALSE 0
#define TRUE  (!FALSE)

#define NULL  ((void *)0)

#define nul  '\0'

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

typedef void (*VOID_FUNC)(void);

typedef int (*STD_FUNC)(int argc, char** argv);

typedef struct { const UINT32 value; const char *str; } CONSTYPE;

#define INC_WRAP(ptr, size) (ptr) = ((ptr)+1) % (size)
#define DEC_WRAP(ptr, size) (ptr) = (((ptr) == 0) ? ((size) - 1) : ((ptr) - 1))

#if 0
#define MAX_UINT(w) (\
        ((w) == sizeof(UINT8))  ? 0xFFU :\
        ((w) == sizeof(UINT16)) ? 0xFFFFU :\
        ((w) == sizeof(UINT32)) ? 0xFFFFFFFFU :\
        ((w) == sizeof(UINT64)) ? 0xFFFFFFFFFFFFFFFFU : 0)
#else
#define MAX_UINT8  0xFFU
#define MAX_UINT16 0xFFFFU
#define MAX_UINT32 0xFFFFFFFFU
#define MAX_UINT64 0xFFFFFFFFFFFFFFFFU
#endif

#if 0
#define MAX_SINT(w) (\
        ((w) == sizeof(SINT8))  ? 0x7F :\
        ((w) == sizeof(SINT16)) ? 0x7FFF :\
        ((w) == sizeof(SINT32)) ? 0x7FFFFFFF :\
        ((w) == sizeof(SINT64)) ? 0x7FFFFFFFFFFFFFFF : 0)
#else
#define MAX_SINT8  0x7FU
#define MAX_SINT16 0x7FFFU
#define MAX_SINT32 0x7FFFFFFFU
#define MAX_SINT64 0x7FFFFFFFFFFFFFFFU
#endif

#if !defined(_SIZE_T_)
#define _SIZE_T_ __typeof(sizeof(int))
typedef _SIZE_T_ size_t;
#endif

#endif /* __ASSEMBLY__ */

#if !defined(BYTE)
    #define BYTE(d,n) (((d) >> ((n) << 3)) & 0xff)
#endif

#define SWAPEND32(d)    ((BYTE(d,0) << 24) | (BYTE(d,1) << 16) |\
                         (BYTE(d,2) <<  8) | (BYTE(d,3) << 0))

#define SWAPEND64(d)    ((BYTE(d,0) << 56) | (BYTE(d,1) << 48) |\
                         (BYTE(d,2) << 40) | (BYTE(d,3) << 32) |\
                         (BYTE(d,4) << 24) | (BYTE(d,5) << 16) |\
                         (BYTE(d,6) << 8)  | (BYTE(d,7) << 0))

#endif /* _GLOBAL_TYPE_H_ */
