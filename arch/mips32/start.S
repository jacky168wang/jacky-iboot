/*******************************************************************************
 * Description :
 *     MIPS Exception Service Codes in ROM for exception entry points
 * including RESET(reset/SoftReset/NMI)
 *
 * Author :
 *     jiwang@ikanos.com, 2011/11/11, re-write
 *
 * Reference:
 *     MIPS32 34K Processor Core Family Software User's Manual
 */
/* Table 7.37 Cause Register Field Descriptions
 ------------------------------------------------
[31]BD: (ReadOnly) the last exception taken occurred in a branch delay slot
[30]TI: (ReadOnly) "Timer Interrupt" is pending
[29:28]CE: (ReadOnly) Coprocessor unit number referenced when a 
       "Coprocessor Unusable" exception is taken.
[27]DC: (R/W) Disable Count register if power-sensitive
[26]PCI: (ReadOnly) "Performance Counter Interrupt" is pending
[23]IV: (R/W) Use the general/special exception vector (16#180/200).
    If CauseIV is 1 and StatusBEV is 0, the special interrupt vector represents
    the base of the vectored interrupt table. (undefined when reset)
[22]WP: (R/W) indicates a "WATCH" exception was deferred because StatusEXL or 
    StatusERL were a 1 at the time the "WATCH" exception was detected. AND 
    causes this "WATCH" exception to be initiated once StatusEXL and StatusERL
    are both zero. As such, software must clear it as part of the "WATCH"
    exception handler to prevent a "WATCH" exception loop.
    it is UNPREDICTABLE if software write 1 when 0.
 */
/* Table 7.32 Status Register Field Descriptions
 ------------------------------------------------
[22]BEV: controls the Base of Exception Vectors (1 when reset).
   0 Normal    - KSEG0 (  cached, unmapped) C0_Ebase
   1 Bootstrap - KSEG1 (uncached, unmapped) 0xbfc00000
[20]SR/SoftReset: Indicates that the entry through the reset exception
   vector was due to a SoftReset. NOTICE:34K does not support SoftReset.
[19]NMI: Indicates that the entry through the reset exception vector was due
   to an NMI. write 0 will clear it, write 1 does not function.
[2]ERL/Error Level: Set by the processor when a RESET(reset/SoftReset/NMI) or
  CacheError exception are taken. When ERL is set:
   (1) The processor is running in kernel mode
   (2) Interrupts are disabled
   (3) The ERET instruction will use the return address held in 
       ErrorEPC instead of EPC
   (4) The lower (1<<29) bytes of KUSEG are treated as an unmapped
       and uncached region. This allows main memory to be accessed
       in the presence of cache errors.
[1]EXL/Exception Level: set by the processor when any exception other than
  RESET(reset/SoftReset/NMI) exceptions is taken. When EXL is set:
   (1) The processor is running in Kernel Mode
   (2) Interrupts are disabled
   (3) "TLB Refill" exceptions use the general exception vector instead of
       the "TLB Refill" vector
   (4) EPC, CauseBD/SRSCtl will not be updated if another exception is taken.
*/
/* Chapter8: Hardware and Software Initialization of the 34K Core
 ------------------------------------------------
 * 8.1 Hardware-Initialized Processor State
 *     Only a minimal subset of the processor state is cleared. This is enough
 * to bring the core up while running in unmapped and uncached code space.
 *   8.1.1 Coprocessor 0 State
 *   8.1.2 TLB Initialization
 *   8.1.3 Bus State Machines
 *   8.1.4 Static Configuration Inputs
 *   8.1.5 Fetch Address is directed to VA 0xBFC00000 (PA 0x1FC00000)
 *         KSEG1/uncached, TLB & caches do NOT require hardware initialization
 *
 * 8.2 Software Initialized Processor State
 *   8.2.1 Register File
 *     (1) register file powers up in an unknown state except r0.
 *     (2) the rest had better be initialized for added safety.
 *   8.2.2 TLB: an implementation specific feature
 *   8.2.3 Caches
 *     (1)the cache tag and data arrays power up to an unknown state.
 *     (2)and they are not affected by reset.
 *     (3)every tag in the cache arrays should be initialized to an invalid
 *        state using the CACHE instruction (Index Invalidate function).
 *     (4)the instruction cache initialization needs to be run in an uncached
 *        address region.
 *   8.2.4 Coprocessor 0 State
 *     (1)there are various exceptions blocked by ERL=1/EXL=1. they are not
 *        cleared by Reset. they should be cleared to avoid taking spurious 
 *        exceptions when leaving the boot code.
 *     (2)Cause: WP(WatchPending), SW0/1(SoftwareInterrupts) should be cleared.
 *     (3)Config: K0(kseg0 Coherency Algorithm) should be set to the desired 
 *        Cache Coherency Algorithm (CCA) prior to accessing KSEG0.
 *     (4)Config: KU/K23 should be set to the desired CCA for USEG/KUSEG and
 *        KSEG2/3 respectively prior to accessing those regions.
 *     (5)Count/Compare: should be set to a known value if Timer Interrupts
 *        are used. first Count then Compare, to avoid any unexpected interrupts
 *        as the write to compare will also clear any pending Timer Interrupts.
 *     (6)Status: desired state should be set.
 *     (7)Other: should be written before they are read.
 *   8.2.5 Multi-threading Initialization
 *     Chapter 4, Initializing the 34K - Multi-Threaded bootstrap issues
*/
#include "global_type.h"
#include "reg_landshark.h"
#include "debug.S"
#include "uart_boot.h"

/*******************************************************************************
 *      Exception entry points in ROM including RESET.
 *
 * 1, Exception Vector Base Addresses is 16#BFC0.0000
 * 2, Interrupt Compatibility Mode is entered when a "reset" exception occurs.
 *    interrupts are non-vectored and dispatched though exception vector offset
 *    16#180 (if CauseIV = 0) or vector offset 16#200 (if CauseIV = 1).
*/
    .text
    .set noreorder

    .org 0x000  /* RESET(reset/SoftReset/NMI) */
#if 0 /* not necessary for 34Kc */
    /* Force BootStrap BEV in case "undefined when reset" */
    mfc0  t0, C0_Status
    or    t0, M_StatusBEV
    mtc0  t0, C0_Status
    //ehb

    /* Force CauseIV=1 to vector interrupts to 0xBFC00400
     * as CauseIV is "undefined when reset"
     */
    mfc0  k0, C0_Cause
    or    k0, M_CauseIV
    mtc0  t0, C0_Cause
    //ehb
    
    /* Interrupts have been disabled already by hardware */
#endif
    b     boot_reset
    nop   /* delay slot */

    .org 0x200  /* TLB refill */
    b     boot_exception
    nop   /* delay slot */

    .org 0x280  /* XTLB refill */
    b     boot_exception
    nop   /* delay slot */

    .org 0x300  /* Cache error */
    b     boot_exception
    nop   /* delay slot */

    .org 0x380  /* General */
    b     boot_exception
    nop   /* delay slot */

    .org 0x400  /* Int, CauseIV=1 */
    b     boot_exception
    nop   /* delay slot */

    .org 0x480  /* EJTAG debug exception */
    b     boot_exception
    nop   /* delay slot */

boot_exception:
    /* Check for interrupt */
    mfc0  k0, C0_Cause
    and   k0, M_CauseExcCode
    srl   k0, S_CauseExcCode
    subu  k0, EX_INT
    beq   k0, zero, boot_interrupt
    nop   /* delay slot */
    
    a_put_exception
    b     inf_loop_exp
    nop   /* delay slot */

boot_interrupt:
    a_put_interrupt
    /* Interrupt : Disable interrupts and return */
    mfc0  k0, C0_Status
    srl   k0, 1
    sll   k0, 1
    mtc0  k0, C0_Status

inf_loop_exp:
    b  inf_loop_exp
    nop   /* time slot */

NON_LEAF(boot_reset)

/* ----Step1: 8.2.1 Register File, zero all GPRs except r0 */
.set noat
    move $1, zero
    move $2, zero
    move $3, zero
    move $4, zero
    move $5, zero
    move $6, zero
    move $7, zero
    move $8, zero
    move $9, zero
    move $10, zero
    move $11, zero
    move $12, zero
    move $13, zero
    move $14, zero
    move $15, zero
    move $16, zero
    move $17, zero
    move $18, zero
    move $19, zero
    move $20, zero
    move $21, zero
    move $22, zero
    move $23, zero
    move $24, zero
    move $25, zero
    move $26, zero
    move $27, zero
    move $30, zero
    move $31, zero
.set at
    /* Mult result regs */
    mtlo  zero
    mthi  zero
    
/* ----Step2: 8.2.4 Coprocessor 0 State, (7), zero some CP0 registers */ 

    mtc0  zero, C0_BadVAddr
    mtc0  zero, C0_EPC
    mtc0  zero, C0_Index
    mtc0  zero, C0_Random
    mtc0  zero, C0_EntryLo0
    mtc0  zero, C0_EntryLo1
    mtc0  zero, C0_Context
    mtc0  zero, C0_PageMask
    mtc0  zero, C0_Wired
    mtc0  zero, C0_EntryHi
    mtc0  zero, C0_ErrorEPC

/* ----Step3: 8.2.4 Coprocessor 0 State */ 

    /* Table 7.37 and 8.2.4 Coprocessor 0 State (2)
     * clear CauseWP/SW1/SW0, set CauseIV/DC
     */
    //li    k0, (M_CauseIV | M_CauseDC)
    li    k0, (M_CauseIV) /* clear C0_Cause[DC] to enable counting */
    mtc0  k0, C0_Cause
    /* 8.2.4 Coprocessor 0 State (1)
     */
    mtc0  zero, C0_WatchLo
    mtc0  zero, C0_WatchHi

    /* 8.2.4 Coprocessor 0 State (5)
     * Set compare with -1 to clear previous timer interrupt and
     * delay latter timer interrupt in case
     */
    mtc0  zero, C0_Count /* clear C0_Cause[DC] to enable counting */
    li    t0, -1
    mtc0  t0, C0_Compare

    /* Table 7.32 and 8.2.4 Coprocessor 0 State (6) */
    /* Disable Coprocessor Usable bits 
     * Turn off reverse endian if writeable
     * Clear TS, SR, NMI bits if writeable
     * Clear UserMode, ERL, EXL
     * Clear Interrupt Masks & Interrupt Enable 
     */
    li    t0, (M_StatusBEV | M_StatusRP) /* interrupts are still disabled */
    mtc0  t0, C0_Status

/* ----Step4: initialise UART */

#if !defined(UARTBOOT_CODE)
    a_uart_init
#endif
    a_put_hello

/* ----Step5: read processor ID, configs */

#if (DEBUG_LEVEL > 0)
    a_put_PRId
    mfc0  a0, C0_PRId
    a_put_hex()
#endif

#if (DEBUG_LEVEL > 1)
    a_put_Config
    mfc0  a0, C0_Config
    a_put_hex()
    a_put_char(',')
    mfc0  a0, C0_Config1
    a_put_hex()
    a_put_char(',')
    mfc0  a0, C0_Config2
    a_put_hex()
    a_put_char(',')
    mfc0  a0, C0_Config3
    a_put_hex()
#endif

/* ----Step6: 8.2.4 Coprocessor 0 State, (3), KSEG0 cache coherency */

#if (DEBUG_LEVEL > 1)
    a_put_k0seg
#endif
    /* Note the selected cache coherency attribute setting has influence
     * on some of the tests
     */
    mfc0  s0, C0_Config /* default uncached after reset */
#if !defined(K0SEG_CACHED)
    li    s1, (K_CacheAttrU      << S_ConfigK0)
#else
    #if defined(KSEG0_CACHED_NONCOHERENT)
    li    s1, (K_CacheAttrCNWBWA << S_ConfigK0)
    #else
    li    s1, (K_CacheAttrCWTnWA << S_ConfigK0)
    #endif
#endif
    /* 'ins Rt,Rs,pos,size': Insert Bit Field: Rt=InsertField(Rs,Rt,pos,size) */
    ins   s0, s1, 0, 3 /* S_ConfigK0, W_ConfigK0 */ 
    mtc0  s0, C0_Config
#if (DEBUG_LEVEL > 1)
#if !defined(K0SEG_CACHED)
    a_put_uncached
#else
    #if defined(KSEG0_CACHED_NONCOHERENT)
    a_put_cached_mapped
    #else
    a_put_cached_unmapped
    #endif
#endif
#endif
#if (DEBUG_LEVEL > 0)
    a_put_Config
    mfc0  a0, C0_Config
    a_put_hex()
#endif

#if defined(K0SEG_CACHED)
/* ----Step7: 8.2.3 Caches, invalidate I/instruction caches for KSEG0 */

#if (DEBUG_LEVEL > 0)
    a_put_icache
#endif
    /* Determine cache size using CP0_CONFIG1 */
    mfc0  s2, C0_Config1
    
    /* Isolate I$ Line Size: S_Config1IL[21:19] */
    sll   s3, s2, 10 /* bit29 - bit19 */
    srl   s3, 29
    /* Skip ahead if No I$ */
    beq   s3, zero, L_icache_none
    nop   /* delay slot */
    /* get the true I$ Line size in bytes */
    li    s6, 2
    sllv  s3, s6, s3 /* Rd = Rt << Rs[4:0], s3 = 1 << (1 + s3) */
    
    /* Isolate I$ Sets per way: S_Config1IS[24:22] */
    sll   s4, s2, 7 /* bit29 - bit22 */
    srl   s4, 29
    /* get the true I$ Sets per way in bytes */
    li    s6, 64
    sllv  s4, s6, s4 /* Rd = Rt << Rs[4:0], s4 = 1 << (6 + s4) */

    /* Isolate I$ Associativity: S_Config1IA[18:16] */
    sll   s5, s2, 13 /* bit29 - bit16 */
    srl   s5, 29     /* I$ Assoc (-1) */
    /* get the true I$ Associativity */
    add   s5, 1

#if (DEBUG_LEVEL > 0)
    a_put_cacheline
    move a0, s3
    a_put_hex()

    a_put_waysets
    a_put_char('(')
    move a0, s4
    a_put_hex()
    a_put_char(' ')
    a_put_char('*')
    a_put_char(' ')
    move a0, s5
    a_put_hex()
    a_put_char(')')
#endif
    mul   s4, s4, s5  /* Total number of sets */

#if (DEBUG_LEVEL > 1)
    a_put_cacheinit
#endif
    /* Clear TagLo(TagHi) registers first before initialise cache */
    mtc0  zero, C0_ITagLo
    mtc0  zero, C0_ITagHi

    li    s6, A_K0BASE  /* Get a KSEG0 address for cacheops */
    move  s7, s4  

    /*  Index Store Tag Cache Op:
     *  Will invalidate the tag entry, clear the lock bit, 
     *  and clear the LRF bit
     */
L_icache_nextline:
    cache ICIndexStTag, 0(s6)
    add   s7, -1      /* Decrement set counter */
    bne   s7, zero, L_icache_nextline
    add   s6, s3      /* delay slot, Get next line address */
    
    b     L_icache_end_end
    nop   /* delay slot */
L_icache_none:
    a_put_0size
    b     L_icache_end_end
    nop   /* delay slot */
L_icache_end:

#if 0 /* we are in KSEG1/ROM space */
#if (DEBUG_LEVEL > 2)
    li    t0, L_icache_end_end
    move a0, t0
    a_put_hex()
    li    t0, L_icache_end_end
    KSEG0_A(t0)
    move a0, t0
    a_put_hex()
#endif
    /* shift to KSEG0 at once as I cache is initialised */
    li    t0, L_icache_end_end
    KSEG0_A(t0)
    j     t0
    nop   /* delay slot */
#endif
L_icache_end_end:

/* ----Step8: 8.2.3 Caches, invalidate D/data caches for KSEG0 */

#if (DEBUG_LEVEL > 0)
    a_put_dcache
#endif
    /* Determine cache size using CP0_CONFIG1 */
    mfc0  s2, C0_Config1

    /* Isolate D$ Line Size: S_Config1DL[12:10] */
    sll   s3, s2, 19 /* bit29 - bit10 */
    srl   s3, 29
    /* Skip ahead if No D$ */
    beq   s3, zero, L_dcache_none
    nop   /* delay slot */
    /* get the true D$ Line size in bytes */
    li    s6, 2
    sllv  s3, s6, s3 /* Rd = Rt << Rs[4:0], s3 = 1 << (1 + s3) */

    /* Isolate D$ Sets per way: S_Config1DS[15:13] */
    sll   s4, s2, 16 /* bit29 - bit13 */
    srl   s4, 29
    /* get the true D$ Sets per way in bytes */
    li    s6, 64
    sllv  s4, s6, s4 /* Rd = Rt << Rs[4:0], s4 = 1 << (6 + s4) */

    /* Isolate D$ Associativity: S_Config1DA[09:07] */
    sll   s5, s2, 22 /* bit29 - bit07 */
    srl   s5, 29
    /* get the true I$ Associativity */
    add   s5, 1

#if (DEBUG_LEVEL > 0)
    a_put_cacheline
    move a0, s3
    a_put_hex()

    a_put_waysets
    a_put_char('(')
    move a0, s4
    a_put_hex()
    a_put_char(' ')
    a_put_char('*')
    a_put_char(' ')
    move a0, s5
    a_put_hex()
    a_put_char(')')
#endif
    mul   s4, s4, s5  /* Get total number of sets */

#if (DEBUG_LEVEL > 1)
    a_put_cacheinit
#endif
    /* Clear DataLo/DataHi registers first before initialise cache */
    mtc0  zero, C0_DTagLo
    mtc0  zero, C0_DTagHi

    li    s6, A_K0BASE  /* Get a KSeg0 address for cacheops */
    move  s7, s4  

    /*  Index Store Tag Cache Op:
     *  Will invalidate the tag entry, clear the lock bit, 
     *  and clear the LRF bit
     */
L_dcache_nextline:
    cache DCIndexStTag, 0(s6)
    add   s7, -1      /* Decrement set counter */
    bne   s7, zero, L_dcache_nextline
    add   s6, s3      /* delay slot, Get next line address */
    
    b     L_dcache_end
    nop   /* delay slot */
L_dcache_none:
    a_put_0size
L_dcache_end:

#endif

#if 0
/* ----Step9: Set up Interrupt environment and then enable Interrupts */

#if (DEBUG_LEVEL > 1)
    a_put_intenv
#endif
    /* Clear CauseWP to avoid watch exception upon user code entry 
     * Clear software interrupts
     *   Set CauseIV to map Interrupts to 0xBFC00400
     */
    mfc0  t0, C0_Cause
    and   t0, ~(M_CauseWP | M_CauseIP1 | M_CauseIP0)
    or    t0, M_CauseIV
    mtc0  t0, C0_Cause

    /* Clear any pending "WATCH" exception */

    li    t0, 0x7 /* (M_WatchHiI | M_WatchHiR | M_WatchHiW) */
    mtc0  t0, C0_WatchHi 
    ehb /* Stop instruction execution until execution hazards are cleared */
    
    /* Check the presence of additional Watch registers */
    mfc0  t0, C0_WatchHi
    srl   t0, t0, 31
    beq   t0, zero, L_watch_one
    li    t0, 0x7 /* delay slot */

    /* Disable watch exceptions */
    mtc0  zero, C0_WatchLo, 1
    mtc0  zero, C0_WatchLo, 2
    mtc0  zero, C0_WatchLo, 3
    /* Clear Watch Status bits */
    mtc0  t0, C0_WatchHi, 1
    mtc0  t0, C0_WatchHi, 2
    mtc0  t0, C0_WatchHi, 3
    /* no jump */

L_watch_one:
    /* Disable watch exceptions */
    mtc0  zero, C0_WatchLo
    /* Clear Watch Status bits */
    mtc0  t0, C0_WatchHi

    /* enable all hardware/software Interrupts */
    mfc0  t0, C0_Status
    or    t0, 0xff01
    mtc0  t0, C0_Status

#endif

/* ----Step10: Set up 'C' environment */

    la    sp, _sp
    la    gp, _gp

    /*  C-functions (and Assembler) may now be called since RAM is ready and
     *  sp, gp have been setup.
     */
#if (DEBUG_LEVEL > 0)
    a_put_cjump
#endif
    /*  Call the first C-function. It should never return.
     */
    la    t0, main
    jr    t0
    nop   /* delay slot */

ENDF(boot_reset)
