/***********************************************************************
 *
 * Linker script : Review *.map file to get a figure
 *
 * $gp�Ĵ������ֵ�ԭ��:
 *     RISC��ϵÿ��ָ���32Bit������ַ����Ҳ��32Bit���������Ͳ�����
 * ͨ��һ��ָ����ʵ��Memory�ķ��ʡ�
 *     lw r1, offset(r2) ���У�offsetΪ16Bit��
 * Ҳ����˵������ܷ�����r2Ϊ����ַ��ǰ��32K�Ŀռ䡣
 *     ����֪����GCC�ڱ�����̣��Ὣ�����ֱ����data��bss��common�ȶΡ�
 * ������mips����ʱ���ὫС��������sdata��sbss��scommon�ȶΡ������С
 * �ɱ���ѡ��-G��������-G0���ʾ��ʹ��С���ݶΡ�Ĭ����-G8����ʾС�ڻ�
 * ����8 byte�����ݽ�����С��������
 *     �����Ļ������ϵ�ʱ��gp��ʼ��һ��ֵ����ô��С�������Ϳ���ͨ��
 * lw r1, offset(gp) �������ˡ�
 *    ��ô��GCC��ô֪��gp��ʼ����ֵ�أ�ֻҪ��link�ļ�������_gp���ţ�
 * �������ͻ���Ϊ����gp��ֵ���������ϵ�ʱ����_gp��ֵ����gp�Ĵ��������ˡ�
 *
 * $gp ��ʹ��: points to the global area
 *     1)Always store it in the Saved Register Area of your stack. 
 *     2)Always load that value into the $gp when destroying the stack. 
 *     3)Dont use it. (i.e dont access global scope)
 *
 * �������и�Ĭ�ϵ��������ӽű�, ����ld --verbose�鿴. -Tѡ������ָ���Լ������ӽű�
 *
 * SECTIONS
 * {
 *   (1) ENTRY����
 *   (2) ���Ÿ�ֵ���
 *   (3) һ�����section������(output section description)
 *   (4) һ��section��������(overlay description)
 * }
 *
 * 'INCLUDE FILENAME'
 *     �ڵ�ǰ��������ӽű��ļ�. �ڵ�ǰ·���»���'-L'ѡ��ָ��������·������������ļ�,
 * �����Ƕ��ʹ��'INCLUDE'��10��
 * `INPUT(FILE FILE ...)' ָʾ������������ʱ�����ļ�, ��������������������ָ����һ��
 * �����ʹ����'INPUT (-lFILE)', 'ld'����ļ���ת��Ϊ'libFILE.a', ���������в���'-l'һ��
 * `GROUP(FILE FILE ...)' ���ļ�
 * `OUTPUT(FILENAME)' �����в���'-o'һ��
 * `SEARCH_DIR(PATH)' �����в���'-L PATH'
 * `STARTUP(FILENAME)' ������ڵ���ļ�
 * `EXTERN(SYMBOL SYMBOL ...)' �����в���'-u'һ��
 *
 * OUTPUT_FORMAT("elf32-tradlittlemips") �����в���'-oformat'һ��
 * `OUTPUT_FORMAT(DEFAULT, BIG, LITTLE)' ����
 * OUTPUT_FORMAT(elf32-bigmips, elf32-bigmips, elf32-littlemips)
 *    �����в���      no             -EB             -EL

 *
 * `ASSERT(EXP, MESSAGE)' ���EXP������, �������ͻ᷵��һ���������˳�,����ӡ��MESSAGE.

 ************************************************************************/

OUTPUT_ARCH(mips)

/**** Start point ****/
/*
ld �ж��ַ������ý�����ڵ�ַ��ͨ����������˳�򣺣����Խǰ, ���ȼ�Խ�ߣ�
1, ld �����е�-eѡ��
2, ���ӽű��� ENTRY(SYMBOL) ����
3, ��������� start ����, ʹ�� start ����ֵ
4, ������� .text section, ʹ�� .text section �ĵ�һ�ֽڵ�λ��ֵ
5, ʹ��ֵ 0
*/
/* ENTRY(0x991D0000) */

MEMORY
{
	/* 0x191D0000 - 0x191DFFFF */
    LMEM (xw) : ORIGIN = 0x991D0000, LENGTH = 32K
}

/*
��ν����� section ��Ϊ��� section
��ΰ���� section ��������ַ�ռ� (VMA) �ͽ��̵�ַ�ռ� (LMA)
*/
SECTIONS
{
    /* 0x991D0000 : recoveryboot Entry point */
    /* 0xBFC00000 : ROM Entry point */
    /* 0x991D3018 : primaryboot Entry point */
    . = 0x991D0000;
    .text :
    {
        _ftext = ABSOLUTE(.);/* Start of code and read-only data */
        start.o (.text)
        fboot.o
        *(.text)
        _ecode = ABSOLUTE(.);/* End of code and Start of read-only data */
        *(.rodata.str1.4)
        *(.rodata)
        _etext = ABSOLUTE(.);/* End of code and read-only data */
    }
    
    . = ALIGN(8);
    .data :
    {
        _fdata = ABSOLUTE(.);/* Start of initialised data */
        *(.data)
        /* We want the small data sections together, so single-instruction offsets
           can access them all, and initialized data all before uninitialized, so
           we can shorten the on-disk segment size. */
        . = ALIGN(8);
        /*_gp = ABSOLUTE(. + 0x7ff0); baseAddress of small data : 32K - 16 */
        _gp = ABSOLUTE(.);
        *(.sdata)
        *(.lit8) /* 64-bit floating-point constants */
        *(.lit4) /* 32-bit floating-point constants */
        _edata = ABSOLUTE(.);/* End of initialised data */
    }

    . = ALIGN(16);
    .bss :
    {
        _fbss = ABSOLUTE(.);/* Start of uninitialised data */
        *(.sbss)    /* small    bss variable in C */
        *(.scommon) /* small common variable in Fortran */
        *(.bss)     /*          bss variable in C */
        *(COMMON)   /*       common variable in Fortran */
        _ebss = ABSOLUTE(.);/* End of unitialised data */
    }

    /* Allocate room for stack */
    . = ALIGN(8);
    .stack :
    {
        _tstack = ABSOLUTE(.);/* top of stack */
        . += 0x400 ;/* 1K */
        _sp = ABSOLUTE(. - 16);
        _bstack = ABSOLUTE(.);/* bottom of stack */
    }
    
    _end = .;
    _free = .;

	/* These must appear regardless of '.' */
    .gptab.sdata : { *(.gptab.data) *(.gptab.sdata) }
    .gptab.sbss  : { *(.gptab.bss)  *(.gptab.sbss)  }

    /DISCARD/ :
    {
        *(.reginfo)
    }

    /* PROVIDE�ؼ������ڶ���������ţ���Ŀ���ļ��ڱ����ã���û�����κ�Ŀ���ļ��ڱ�����ķ��� */
    PROVIDE (etext = _etext);
    PROVIDE (edata = _edata);
    PROVIDE (end = _end);
}