/*******************************************************************************
 * Description   : CSRs for Monitor
 *
 * Author :
 *         jiwang@ikanos.com, 2011/03/16, create
 ******************************************************************************/
#ifndef _REG_MONITOR_H_
#define _REG_MONITOR_H_

/************************************************************************
 * "Definitions": All Registers Location/Map
 ************************************************************************/

#define MON_BASE 0xBF020000

#if 1

#define MON_HAPPY      (MON_BASE + 0x0000)
#define MON_SAD        (MON_BASE + 0x0008)
#define MON_PUTX_MON   (MON_BASE + 0x0018)
#define MON_PUTCH      (MON_BASE + 0x0020)
#define MON_VCDTRACE   (MON_BASE + 0x0028)
#define MON_TIME       (MON_BASE + 0x0030)

#define MON_SET_RETRY  (MON_BASE + 0x0038)
#define MON_LOCK       (MON_BASE + 0x0040)
#define MON_ACK        (MON_BASE + 0x0048)
#define MON_SET_SPLIT  (MON_BASE + 0x0050)
#define MON_SET_WS     (MON_BASE + 0x0058)
#define MON_DELAYED_INT (MON_BASE + 0x0060)
#define MON_CLEAR_INT  (MON_BASE + 0x0068)
#define MON_CLKCFG     (MON_BASE + 0x0080)

#define MON_ENDIAN     (MON_BASE + 0x0090)
#define MON_VERBOSE    (MON_BASE + 0x00b8)
#define MON_SRAM_ADDR  (MON_BASE + 0x00c0)
#define MON_WDMAX      (MON_BASE + 0x00c8)

#else

#endif

/************************************************************************
 * "Definitions": BitMask of Each Register 
*************************************************************************/


/************************************************************************
 *  Public "Variables"
 ************************************************************************/

/************************************************************************
 *  Public "Functions" for 'assemble' routines
 ************************************************************************/

#if defined(__ASSEMBLY__)

#define MON_PUTS(string)   \
    la      a0, 80f;        \
    li      a1, MON_PUTCH;  \
82: lb      a2, 0(a0);      \
    beq     zero, a2, 81f;  \
    addiu   a0, 1;          \
    b       82b;            \
    sw      a2, 0(a1);      \
    ssnop;  /* optimize ruby prefetch */ \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    .align  4; /* 4K alignment optimiza2ion */ \
80: .asciiz         string; \
    .align  2;              \
81:

#define MON_PUTS_UC(string) \
    la      a0, 80f;        \
    KSEG1A(a0);             \
    li      a1, MON_PUTCH;  \
82: lb      a2, 0(a0);      \
    beq     zero, a2, 81f;  \
    addiu   a0, 1;          \
    b       82b;            \
    sw      a2, 0(a1);      \
    ssnop;          /* optimize ruby prefetch */ \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    ssnop;                  \
    .align  4;      /* 4K alignment optimization */ \
80: .asciiz         string; \
    .align  2;              \
81:

/* argument in a0     */
#define MON_PUTX            \
    li      s1, MON_PUTCH;  \
    li      s4, '0';        \
    sw      s4, 0(s1);      \
    li      s4, 'x';        \
    sw      s4, 0(s1);      \
    li      s5, 8;          \
    li      s2, 28;         \
1:  srl     s4, a0, s2;     \
    sll     a0, a0, 4;      \
    addiu   s3, s4, -10;    \
    bltz    s3, 2f;         \
    addiu   s4, '0';        \
    addiu   s4, 'a'-'0'-10; \
2:  addiu   s5, -1;         \
    bne     zero,s5,1b;     \
    sw      s4, 0(s1);

#else
/************************************************************************
 *  Public "Functions" for 'c' routines
 ************************************************************************/

#endif /* __ASSEMBLY__ */

#endif /* _REG_MONITOR_H_ */
