/*******************************************************************************
 * Description :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _GENERAL_BITSOP_H_
#define _GENERAL_BITSOP_H_

/* Register field mapping */
#define REGFIELD(reg, rfld) (((reg) & rfld##_MSK) >> rfld##_SHF)

/* register read field */
#define REGARD(addr,fld)    ((H_RD32(addr) & addr##_##fld##_##MSK)  \
             >> addr##_##fld##_##SHF)

/* register write numeric field value                   */
#define REGAWRI(addr,fld,intval) ((H_RD32(addr) & ~(addr##_##fld##_##MSK))\
                 | ((intval) << addr##_##fld##_##SHF))

/* register write enumerated field value                */
#define REGAWRE(addr,fld,enumval) ((H_RD32(addr) & ~(addr##_##fld##_##MSK))\
            | ((addr##_##fld##_##enumval) << addr##_##fld##_##SHF))

/* Examples:
 * 
 *  exccode = REGARD(CPU_CAUSE,EXC);
 *  H_RD32(SDR_CONTROL) = REGAWRI(OSG_CONTROL,TMO,17)
 *           | REGAWRE(OSG_CONTROL,DTYPE,PC1);
 */


/* relative register address, access: base address + offsett        */
#define REG(base,offs)  H_RD32((base) + offs##_##OFS)

/* register read field                          */
#define REGRD(base,offs,fld) ((REG(base,offs) & offs##_##fld##_##MSK)   \
             >> offs##_##fld##_##SHF)

/* register write numeric field value                   */
#define REGWRI(base,offs,fld,intval)((REG(base,offs)& ~(offs##_##fld##_##MSK))\
                                 | (((intval) << offs##_##fld##_##SHF) & offs##_##fld##_##MSK))

/* register write enumerated field value                */
#define REGWRE(base,offs,fld,enumval)((REG(base,offs) & ~(offs##_##fld##_##MSK))\
                | ((offs##_##fld##_##enumval) << offs##_##fld##_##SHF))

/* physical register address, access: base address + offsett        */
#define REGP(base,phys) H_RD32((base) + (phys))

/* physical register read field                         */
#define REGPRD(base,phys,fld) ((REGP(base,phys) & phys##_##fld##_##MSK)     \
             >> phys##_##fld##_##SHF)

/* physical register write numeric field value                  */
#define REGPWRI(base,phys,fld,intval)((REGP(base,phys)& ~(phys##_##fld##_##MSK))\
                 | ((intval) << phys##_##fld##_##SHF))

/* physical register write enumerated field value               */
#define REGPWRE(base,phys,fld,enumval)((REGP(base,phys) & ~(phys##_##fld##_##MSK))\
                | ((phys##_##fld##_##enumval) << phys##_##fld##_##SHF))

// Bit operations
// These operations are used to operaton on the specified bit (or bitfield)
// within a register.
//
// They rely on the following naming conventions:
//  XX_BIT            The bit position of the single bit XX
//  YY_FST            The bit position of the LSB of bitfield YY
//  YY_LEN            The number of bit in the bitfield YY
//
// Don't panic too much at the number of operations - the optimizer in the
// compiler should be able to resolve almost all of them to constants.
#define SETBIT(var, bit, val) \
    do {\
        if (val) \
            var |= (1 << bit ## _BIT ); \
        else \
            var &= ~(1 << bit ## _BIT); \
    } while (0)
#define GETBIT(var, bit)        (((var) >> bit ## _BIT) & 0x1)

#define ZEROMASK(fld)           ((1 << (fld ## _LEN)) - 1)
#define FLDMASK(fld)            (ZEROMASK(fld) << (fld ## _FST))
#define SETFIELD(var, fld, val) (var = (var & ~FLDMASK(fld)) | \
                                 (((val) & ZEROMASK(fld)) << fld ## _FST))
#define GETFIELD(val, fld)      (((val) >> (fld ## _FST)) & ZEROMASK(fld))

#endif /* _GENERAL_BITSOP_H_ */
