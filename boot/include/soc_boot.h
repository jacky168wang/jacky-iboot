#ifndef _SOC_BOOT_H_
#define _SOC_BOOT_H_

#ifdef __ASSEMBLY__

#define host_mips_is_500M() \
    li    t0, SCU_BASE;\
    lw    s4, 0x000C(t0);\
    srl   s4, s4, 4;\
    andi  s4, s4, 0x7;\
    andi  s5, s4, 0x2; /* bit05 in cpu_ctl */ \
    \
    lw    t3, 0x00EC(t0); /* read chip revison */ \
    sra   s6, t3, 24  /* bit24: 0/1 - b0, b1 */

#else
extern UINT32 gChipType;
extern UINT32 gSysClock;

extern UINT32 detect_SoC(void);
extern void detect_resetcause(void);
extern void detect_bootmode(void);
extern void softreset_SoC(void);
extern void RemapCS0_NOR(void);
extern void RemapCS0_ROM(void);
extern void reset_peripheral(void);

#endif /* __ASSEMBLY__ */

#endif /* _FILE_H_ */
