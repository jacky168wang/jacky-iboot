#ifndef _DRAM_BOOT_H_
#define _DRAM_BOOT_H_

#ifdef __ASSEMBLY__

extern int s_init_ddr(void);

#else

extern int c_init_ddr(UINT32 cS, UINT32 cW, UINT32 bW);
extern int c_auto_ddr(void);

#endif /* __ASSEMBLY__ */

#endif /* _FILE_H_ */
