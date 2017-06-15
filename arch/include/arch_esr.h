/*******************************************************************************
 * Description : Exception service routine
 *
 *
 * Author :
 *     spasumar@ikanos.com, 2009/06/25, create
 *       jiwang@ikanos.com, 2011/03/16, modify
 ******************************************************************************/
#ifndef _ARCH_ESR_H_
#define _ARCH_ESR_H_

/************************************************************************
 *  Definitions
*************************************************************************/

/* Max size of CPU registers */
#if ( CPUCORE == 4 )
    #define REGSIZE      4
#elif ( CPUCORE == 5 )
    #define REGSIZE      8
#endif

/* CPU registers */
#define EXCEP_REG0      ( 0*REGSIZE)
#define EXCEP_REG1      ( 1*REGSIZE)
#define EXCEP_REG2      ( 2*REGSIZE)
#define EXCEP_REG3      ( 3*REGSIZE)
#define EXCEP_REG4      ( 4*REGSIZE)
#define EXCEP_REG5      ( 5*REGSIZE)
#define EXCEP_REG6      ( 6*REGSIZE)
#define EXCEP_REG7      ( 7*REGSIZE)
#define EXCEP_REG8      ( 8*REGSIZE)
#define EXCEP_REG9      ( 9*REGSIZE)
#define EXCEP_REG10     (10*REGSIZE)
#define EXCEP_REG11     (11*REGSIZE)
#define EXCEP_REG12     (12*REGSIZE)
#define EXCEP_REG13     (13*REGSIZE)
#define EXCEP_REG14     (14*REGSIZE)
#define EXCEP_REG15     (15*REGSIZE)
#define EXCEP_REG16     (16*REGSIZE)
#define EXCEP_REG17     (17*REGSIZE)
#define EXCEP_REG18     (18*REGSIZE)
#define EXCEP_REG19     (19*REGSIZE)
#define EXCEP_REG20     (20*REGSIZE)
#define EXCEP_REG21     (21*REGSIZE)
#define EXCEP_REG22     (22*REGSIZE)
#define EXCEP_REG23     (23*REGSIZE)
#define EXCEP_REG24     (24*REGSIZE)
#define EXCEP_REG25     (25*REGSIZE)
#define EXCEP_REG26     (26*REGSIZE)
#define EXCEP_REG27     (26*REGSIZE)
#define EXCEP_REG28     (28*REGSIZE)
#define EXCEP_REG29     (29*REGSIZE)
#define EXCEP_REG30     (30*REGSIZE)
#define EXCEP_REG31     (31*REGSIZE)

/* Saved special registers */
#define EXCEP_LO        (32*REGSIZE)
#define EXCEP_HI        (33*REGSIZE)

/* CP0 registers */
#define EXCEP_CP0_STATUS    (34*REGSIZE)
#define EXCEP_CP0_BADVADDR  (35*REGSIZE)
#define EXCEP_CP0_CAUSE     (36*REGSIZE)
#define EXCEP_CP0_EPC       (37*REGSIZE)
#define EXCEP_CP0_INDEX     (38*REGSIZE)
#define EXCEP_CP0_RANDOM    (39*REGSIZE)
#define EXCEP_CP0_ENTRYLO0  (40*REGSIZE)
#define EXCEP_CP0_ENTRYLO1  (41*REGSIZE)
#define EXCEP_CP0_CONTEXT   (42*REGSIZE)
#define EXCEP_CP0_PAGEMASK  (43*REGSIZE)
#define EXCEP_CP0_WIRED     (44*REGSIZE)
#define EXCEP_CP0_COUNT     (45*REGSIZE)
#define EXCEP_CP0_ENTRYHI   (46*REGSIZE)
#define EXCEP_CP0_COMPARE   (47*REGSIZE)
#define EXCEP_CP0_PRID      (48*REGSIZE)
#define EXCEP_CP0_ERROREPC  (49*REGSIZE)
#define EXCEP_CP0_CONFIG    (50*REGSIZE)
#define EXCEP_CP0_CONFIG1   (51*REGSIZE)
#define EXCEP_CP0_WATCHLO   (52*REGSIZE)
#define EXCEP_CP0_WATCHHI   (53*REGSIZE)
#define EXCEP_CP0_DEPC      (54*REGSIZE)

/* Size of context structure */
#define EXCEP_SIZE_CONTEXT  (55*REGSIZE)

#ifndef __ASSEMBLY__

/* This is the C-structure corresponding to the above definitions */
#if ( CPUCORE == 4 )
typedef struct {
    /* CPU registers */
    UINT32  reg0,  reg1,  reg2,  reg3,  reg4,  reg5,  reg6,  reg7;
    UINT32  reg8,  reg9,  reg10, reg11, reg12, reg13, reg14, reg15;
    UINT32  reg16, reg17, reg18, reg19, reg20, reg21, reg22, reg23;
    UINT32  reg24, reg25, reg26, reg27, reg28, reg29, reg30, reg31;

    /* Special registers */
    UINT32  lo;
    UINT32  hi;

    /* CP0 registers */
    UINT32  cp0_status;
    UINT32  cp0_badvaddr;
    UINT32  cp0_cause;
    UINT32  cp0_epc;
    UINT32  cp0_index;
    UINT32  cp0_random;
    UINT32  cp0_entrylo0;
    UINT32  cp0_entrylo1;
    UINT32  cp0_context;
    UINT32  cp0_pagemask;
    UINT32  cp0_wired;
    UINT32  cp0_entryhi;
    UINT32  cp0_prid;
    UINT32  cp0_errorepc;
    UINT32  cp0_config;
    UINT32  cp0_config1;
    UINT32  cp0_watchlo;
    UINT32  cp0_watchhi;
    UINT32  cp0_debug;
    UINT32  cp0_depc;
}t_excep_context;

#elif ( CPUCORE == 5 )

typedef struct {
    /* CPU registers */
    UINT64  reg0,  reg1,  reg2,  reg3,  reg4,  reg5,  reg6,  reg7;
    UINT64  reg8,  reg9,  reg10, reg11, reg12, reg13, reg14, reg15;
    UINT64  reg16, reg17, reg18, reg19, reg20, reg21, reg22, reg23;
    UINT64  reg24, reg25, reg26, reg27, reg28, reg29, reg30, reg31;

    /* Special registers */
    UINT64  lo;
    UINT64  hi;

    /* CP0 registers */
    UINT64  cp0_status;
    UINT64  cp0_badvaddr;
    UINT64  cp0_cause;
    UINT64  cp0_epc;
    UINT64  cp0_index;
    UINT64  cp0_random;
    UINT64  cp0_entrylo0;
    UINT64  cp0_entrylo1;
    UINT64  cp0_context;
    UINT64  cp0_pagemask;
    UINT64  cp0_wired;
    UINT64  cp0_entryhi;
    UINT64  cp0_prid;
    UINT64  cp0_errorepc;
    UINT64  cp0_config;
    UINT64  cp0_config1;
    UINT64  cp0_watchlo;
    UINT64  cp0_watchhi;
    UINT64  cp0_debug;
    UINT64  cp0_depc;
} t_excep_context;
#endif

#if ( CPUCORE == 4 ) || ( CPUCORE == 5 )

/* Exception service routine (ESR) */
typedef t_excep_context *(*t_excep_esr)(t_excep_context *context);

/************************************************************************
 *  Description : Install jump instructions to exc handler 
 *      at exception vector locations.
 *
 *  Parameters : ESR to be registered
 *  Return values : None
 ************************************************************************/
void excep_install_exc_in_ram( t_excep_esr esr );

#endif

#endif /* __ASSEMBLY__ */

#endif /* _ARCH_ESR_H_ */
