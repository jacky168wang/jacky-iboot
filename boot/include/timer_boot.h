#ifndef _TIMER_BOOT_H_
#define _TIMER_BOOT_H_

#ifdef __ASSEMBLY__

    .macro nop_loop n_clock
    li    t3, \n_clock
1:  addi  t3, t3, -1
    bnez  t3, 1b
    nop   /* delay slot */
    .endm

    .macro set_compare ticks
    li    t3, \ticks
    mtc0  t3, $11
    .endm

    .macro get_counter
    mfc0  t3, $9
    .endm

    .macro set_counter ticks
    li    t3, \n_clock
    mtc0  t3, $9
    .endm
#else

#define SET_COMPARE(ticks) __asm__ volatile ("mtc0 %0,$11"::"d"(ticks))

#define GET_COUNTER() ({\
    unsigned long ticks;\
    __asm__ volatile ("mfc0 %0,$9":"=d"(ticks));\
    (ticks); })

#define SET_COUNTER(ticks) __asm__ volatile ("mtc0 %0,$9"::"d"(ticks))

extern void mdelay(unsigned int msec);

#endif /* __ASSEMBLY__ */

#endif /* _FILE_H_ */
