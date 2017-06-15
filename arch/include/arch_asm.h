/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _ARCH_ASM_H_
#define _ARCH_ASM_H_

#if defined(__ASSEMBLY__)

#define BSS .bss
//#define BSS .section bss

#define LEAF(name)\
        .text;\
        .globl  name;\
        .ent    name;\
name:

#define NON_LEAF(name)\
        .text;\
        .globl  name;\
        .ent    name;\
name:

#define ENDF(name)\
        .size name, .-name;\
        .end  name

#if 0
#define SWAPEND32( src, tmp0, tmp1 ) \
        and tmp0, src, 0xff;\
        srl src,  8;\
        sll tmp0, 8;\
        and tmp1, src, 0xff;\
        or  tmp0, tmp1;\
        srl src,  8;\
        sll tmp0, 8;\
        and tmp1, src, 0xff;\
        or  tmp0, tmp1;\
        srl src,  8;\
        sll tmp0, 8;\
        or  src,  tmp0
#endif

#define a_poll_until_set(addrh, addrl, bitmsk) \
    lui   t0, addrh;\
1:  lw    t2, addrl(t0);\
    and   t2, t2, bitmsk;\
    xor   t2, t2, bitmsk;\
    bnez  t2, 1b;\
    nop   /* time slot */

#define a_poll_until_clr(addrh, addrl, bitmsk) \
    lui   t0, addrh;\
1:  lw    t2, addrl(t0);\
    andi  t2, t2, bitmsk;\
    bnez  t2, 1b;\
    nop   /* time slot */

#endif /* __ASSEMBLY__ */

/* ºê»ã±à */
#if !defined(__ASSEMBLY__)
/*
1, R-Type (operate)
"addu d,s"   addu d,d,s
2,
"move d,s"   or d,zero,s
"negu d,s"   subu d,zero,s
"not d"      nor d,zero,d
3, I-Type (immediate)
"addu $2,$3,64"      addiu $2,$3,64
"addu $2,0x12345"    li at,0x12345; addu $2,$2,at
"li $2,-5"           addiu $2,zero,-5
"li $3,0x8777"       ori   $3,zero,0x8777
"li $4,0x870000"     lui   $4,0x87
"li $5,0x12345"      lui   $5,0x1; ori $5,$5,0x2345
*/
#endif /* __ASSEMBLY__ */

/*
 * the initial Stack size
 */
#define SYS_STACK_SIZE  0x1000

#endif /* _ARCH_ASM_H_ */
