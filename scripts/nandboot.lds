/*********************************************************************************
 * Overview: 									 *
 * This is the linker script used by the Primary Bootloader. 			 *	
 *										 *
 * The LMEM Layout table used for NAND Boot is mentioned below 			 *				
 * 0xb91d0000 - 0xb91d2fff	: BCH decode table & code			 *
 * 0xb91d3000 - 0xb91d777F  	: NAND Program Download Area (Primary Bootloader)*
 * 0xb91d7780 - 0xb91d7bc0  	: NAND boot stack				 *
 * 0xb91d7bc4 - 0xb91d7fff  	: Bootrom variable 				 *
 *										 *
 *********************************************************************************/

OUTPUT_ARCH(mips)

/**** Start point ****/
ENTRY(__reset_handler)			/* Entry point of application		*/

MEMORY
{
    LMEM (wx) : ORIGIN = 0x991d3018, LENGTH = 17K
}

SECTIONS
{
   /* ROM Entry point */
  .boot 0x991D3018 :
  {
    reset.o (.text)
    
    int_s.o (.text)

    startup.o (.text)

    _ftext = ABSOLUTE(.) ;	/* Start of code and read-only data	*/
    *(.text)
    _ecode = ABSOLUTE(.) ;	/* End of code				*/

    *(.rodata)

    . = ALIGN(8);
    _etext = ABSOLUTE(.);	/* End of code and read-only data	*/

    _fdata = ABSOLUTE(.);	/* Start of initialised data		*/
    *(.data)

    . = ALIGN(8);

    _gp = ABSOLUTE(. + 0x7ff0); /* Base of small data			*/

    *(.lit8)
    *(.lit4)
    *(.sdata)

    . = ALIGN(8);

    _edata  = ABSOLUTE(.);	/* End of initialised data		*/
  } > LMEM

  .sbss :
  {
    . = ALIGN(16);
    *(.sbss)
    *(.scommon)
  } > LMEM

  .bss :
  {
    . = ALIGN(16);
    *(.bss)
    *(COMMON)

    /* Allocate room for stack */
    .   =  ALIGN(8) ;
    .   += 0x400 ;
    _sp =  . - 16;
    
    _stack = .;
  } > LMEM

  _end = . ;			/* End of unitialised data		*/


  /DISCARD/ :
  {
    *(.reginfo)
  }

}


