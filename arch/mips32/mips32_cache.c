

#if defined(K0SEG_CACHED)
    #if defined(KSEG0_CACHED_NONCOHERENT)

#include "arch_isa.h"
#include "arch_esr.h"
#include "arch_asm.h"
#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>

#define cache_op(op,addr)   \
    __asm__ __volatile__(    \
    "   .set    push                    \n" \
    "   .set    noreorder               \n" \
    "   .set    mips3\n\t               \n" \
    "   cache   %0, %1                  \n" \
    "   .set    pop                 \n" \
    :                               \
    : "i" (op), "R" (*(unsigned char *)(addr)))

#define CONFIG_SYS_CACHELINE_SIZE 32

#define Hit_Invalidate_I    0x10
#define Hit_Invalidate_D    0x11
#define Hit_Writeback_Inv_D 0x15

void flush_cache(UINT32 start, UINT32 size)
{
    UINT32 lsize = CONFIG_SYS_CACHELINE_SIZE;
    UINT32 addr = start & ~(lsize - 1);
    UINT32 aend = (start + size - 1) & ~(lsize - 1);

    while (1) {
        cache_op(Hit_Writeback_Inv_D, addr);
        cache_op(Hit_Invalidate_I, addr);
        if (addr == aend)
            break;
        addr += lsize;
    }
}

void flush_dcache_range(UINT32 start, UINT32 stop)
{
    UINT32 lsize = CONFIG_SYS_CACHELINE_SIZE;
    UINT32 addr = start & ~(lsize - 1);
    UINT32 aend = (stop - 1) & ~(lsize - 1);

    while (1) {
        cache_op(Hit_Writeback_Inv_D, addr);
        if (addr == aend)
            break;
        addr += lsize;
    }
}

void invalidate_dcache_range(UINT32 start, UINT32 stop)
{
    UINT32 lsize = CONFIG_SYS_CACHELINE_SIZE;
    UINT32 addr = start & ~(lsize - 1);
    UINT32 aend = (stop - 1) & ~(lsize - 1);

    while (1) {
        cache_op(Hit_Invalidate_D, addr);
        if (addr == aend)
            break;
        addr += lsize;
    }
}

    #endif
#endif
