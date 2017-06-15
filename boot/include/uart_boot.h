#ifndef _UART_BOOT_H_
#define _UART_BOOT_H_

#ifdef __ASSEMBLY__

#if !defined(UARTBOOT_CODE)
/* (non-NOR)Bootrom boot: 
 *     default  9600 when AMC_DATA[2:0]=011
 *     default 38400 when AMC_DATA[2:0]=111
 * there is one command to change baudrate, so the second stage program
 * could read R
 * > u <divisor>               : Set UART divisor
*/
//#define UART_BAUD 38400
//#define UART_BAUD 115200

/* 12.5 MHz system clock */
#if UART_BAUD == 115200
#define UART_DIV_LOW_12M5 0x07
#elif UART_BAUD == 38400
#define UART_DIV_LOW_12M5 0x14
#else
#define UART_DIV_LOW_12M5 0x51
#endif

#define a_uart_init() \
    li    t0, UART1_BASE;\
    li    t1, (UART_LCR_DLAB_1 | UART_LCR_WLS_8);/* set DLAB to access Divisor */ \
    \
    sw    t1, 0xc(t0);\
    and   t1, zero, t1;\
    sw    t1, 4(t0);/* set Divisor High */ \
    li    t1, UART_DIV_LOW_12M5;\
    sw    t1, 0(t0);/* set Divisor Low */ \
    \
    li    t1, UART_LCR_WLS_8; /* clear DLAB to return normal mode */ \
    sw    t1, 0xc(t0)
#endif

/* a0 - ch */
#define a_put_char(ch) \
    li    t0, UART1_BASE; \
1: /* L_putchar_tx */ \
    lw    t1, UART_LSR_OFFSET(t0); \
    and   t1, t1, 0x20; \
    beq   t1, zero, 1b; /*L_putchar_tx*/ \
    nop;   /* delay slot */ \
    li    a0, ch; \
    sw    a0, UART_THR_OFFSET(t0)

#if 0
#define a_put_str string
    la    a0, L_str_address
#if 0 //def KSEG0_NONCOHERENT
    KSEG1_A(a2)
#endif
    li    a0, UART1_BASE
82: lb    a3, 0(a2)
    beq   a3, zero, 81f
    addiu a2, 1 /* delay slot */
1:  lw    a1, UART_LSR_OFFSET(a0)
    and   a1, a1, 0x20
    beq   a1, zero, 1b
    nop   /* delay slot */
    b     82b
    sw    a3, 0(a0) /* delay slot */
    ssnop     /* optimize ruby prefetch */
    .align  4 /* 4K alignment optimization */
L_str_address: .asciiz string
    .align  2
81:
#endif
#if defined(ASM_PRINT_STRING)
#define a_put_str(string) \
    la    a0, 3f; /*L_str_address*/ \
    li    t0, UART1_BASE;\
2: /*L_putstr_next*/ \
    lb    t1, 0(a0);\
    beq   t1, zero, 4f;/*L_putstr_end*/ \
    addiu a0, 1;/* delay slot */ \
1: /*L_putstr_tx*/ \
    lw    t2, UART_LSR_OFFSET(t0);\
    and   t2, t2, 0x20;\
    beq   t2, zero, 1b; /*L_putstr_tx*/ \
    nop;  /* delay slot */\
    sw    t1, UART_THR_OFFSET(t0);\
    b     2b;/*L_putstr_next*/ \
    nop;  /* delay slot */\
    .align  4;\
3: /*L_str_address*/ \
    .asciiz string;\
    .align  2;\
4: /*L_putstr_end*/
#endif

/* argument in a0 */
/* FROM_FIRST_NONZERO - "do not show all zero MSB" */
#define a_put_hex() \
    la    t1, UART1_BASE;\
1: /* L_puthex_tx1 */ \
    lw    t0, UART_LSR_OFFSET(t1);\
    and   t0, t0, 0x20;\
    beq   t0, zero, 1b; /* L_puthex_tx1 */ \
    nop;   /* delay slot */ \
    li    t2, '0';\
    sw    t2, UART_THR_OFFSET(t1);\
2: /* L_puthex_tx2 */ \
    lw    t0, UART_LSR_OFFSET(t1);\
    and   t0, t0, 0x20;\
    beq   t0, zero, 2b; /* L_puthex_tx2 */ \
    nop;   /* delay slot */ \
    li    t2, 'x';\
    sw    t2, UART_THR_OFFSET(t1);\
/*#ifdef FROM_FIRST_NONZERO */ \
    and   t4, zero, t4;  /* t4=FALSE */ \
/*#endif*/ \
    li    t3, 8;\
    li    t5, 28;\
3: /* L_puthex_next_nibbe */ \
    srl   t2, a0, t5;\
    sll   a0, a0, 4;\
    addiu t3, -1;\
/*#ifdef FROM_FIRST_NONZERO*/ \
    bne   t4, zero, 4f; /*L_puthex_char*/ /* print if t4 TRUE */ \
    nop;   /* delay slot */ \
    slt   t4, t4, t2;    /* set t4 if t2 > 0 */ \
    beq   t4, zero, 3b; /*L_puthex_next_nibbe*/  /* do NOT print */ \
    nop;   /* delay slot */ \
4: /* L_puthex_char */ \
/*#endif*/ \
    addiu t6, t2, -10;\
    bltz  t6, 5f; /*L_puthex_char09*/ \
    addiu t2, '0'; /* delay slot */ \
    addiu t2, 'a'-'0'-10;\
5: /* L_puthex_char09 */ \
    lw    t0, UART_LSR_OFFSET(t1);\
    and   t0, t0, 0x20;\
    beq   t0, zero, 5b; /*L_puthex_char09*/ \
    nop;   /* delay slot */ \
    sw    t2, UART_THR_OFFSET(t1);\
    \
    bne   t3, zero, 3b; /*L_puthex_next_nibbe*/ \
    nop   /* delay slot */

#define a_put_newline \
    a_put_char('\r');\
    a_put_char('\n')

#else

extern void c_uart_init(void);
extern void c_put_char(char c);
extern void c_put_str(char *s);
extern void c_put_hex16(UINT16 u);
extern void c_put_dec(UINT32 d);
#if (DEBUG_LEVEL > 0)
extern void debugDumpStaticReg(UINT32 addr, UINT32 flag);
extern void debugDumpVolatileReg(UINT32 addr, UINT32 flag);
extern void debugDumpRegs(UINT32 base, UINT32 count);
#endif
extern int c_get_char_timout(UINT32 delayClk);

#endif /* __ASSEMBLY__ */

#endif /* _FILE_H_ */
