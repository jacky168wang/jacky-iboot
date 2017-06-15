/***********************************************************************
 *
 * Linker script : Review *.map file to get a figure
 *
 * $gp寄存器出现的原因:
 *     RISC体系每条指令都是32Bit，而地址总线也是32Bit，这样，就不可能
 * 通过一条指令来实现Memory的访问。
 *     lw r1, offset(r2) 其中，offset为16Bit，
 * 也就是说，最多能访问以r2为基地址，前后32K的空间。
 *     我们知道，GCC在编译过程，会将变量分别放在data，bss，common等段。
 * 而编译mips程序时，会将小变量放入sdata，sbss，scommon等段。这个大小
 * 由编译选项-G来决定。-G0则表示不使用小数据段。默认是-G8，表示小于或
 * 等于8 byte的数据将放入小变量区。
 *     这样的话，在上电时给gp初始化一个值，那么，小变量区就可以通过
 * lw r1, offset(gp) 来访问了。
 *    那么，GCC怎么知道gp初始化的值呢？只要在link文件中添加_gp符号，
 * 连接器就会认为这是gp的值。我们在上电时，将_gp的值赋给gp寄存器就行了。
 *
 * $gp 的使用: points to the global area
 *     1)Always store it in the Saved Register Area of your stack. 
 *     2)Always load that value into the $gp when destroying the stack. 
 *     3)Dont use it. (i.e dont access global scope)
 *
 * 连接器有个默认的内置连接脚本, 可用ld --verbose查看. -T选项用以指定自己的链接脚本
 *
 * SECTIONS
 * {
 *   (1) ENTRY命令
 *   (2) 符号赋值语句
 *   (3) 一个输出section的描述(output section description)
 *   (4) 一个section叠加描述(overlay description)
 * }
 *
 * 'INCLUDE FILENAME'
 *     在当前点包含连接脚本文件. 在当前路径下或用'-L'选项指定的所有路径下搜索这个文件,
 * 你可以嵌套使用'INCLUDE'达10层
 * `INPUT(FILE FILE ...)' 指示连接器在连接时包含文件, 就像它们是在命令行上指定的一样
 * 如果你使用了'INPUT (-lFILE)', 'ld'会把文件名转换为'libFILE.a', 就象命令行参数'-l'一样
 * `GROUP(FILE FILE ...)' 库文件
 * `OUTPUT(FILENAME)' 命令行参数'-o'一样
 * `SEARCH_DIR(PATH)' 命令行参数'-L PATH'
 * `STARTUP(FILENAME)' 包含入口点的文件
 * `EXTERN(SYMBOL SYMBOL ...)' 命令行参数'-u'一样
 *
 * OUTPUT_FORMAT("elf32-tradlittlemips") 命令行参数'-oformat'一样
 * `OUTPUT_FORMAT(DEFAULT, BIG, LITTLE)' 例如
 * OUTPUT_FORMAT(elf32-bigmips, elf32-bigmips, elf32-littlemips)
 *    命令行参数      no             -EB             -EL

 *
 * `ASSERT(EXP, MESSAGE)' 如果EXP等于零, 连接器就会返回一个错误码退出,并打印出MESSAGE.

 ************************************************************************/

OUTPUT_ARCH(mips)

/**** Start point ****/
/*
ld 有多种方法设置进程入口地址，通常它按以下顺序：（编号越前, 优先级越高）
1, ld 命令行的-e选项
2, 连接脚本的 ENTRY(SYMBOL) 命令
3, 如果定义了 start 符号, 使用 start 符号值
4, 如果存在 .text section, 使用 .text section 的第一字节的位置值
5, 使用值 0
*/
/* ENTRY(0x991D0000) */

MEMORY
{
	/* 0x191D0000 - 0x191DFFFF */
    LMEM (xw) : ORIGIN = 0x991D0000, LENGTH = 32K
}

/*
如何将输入 section 合为输出 section
如何把输出 section 放入程序地址空间 (VMA) 和进程地址空间 (LMA)
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

    /* PROVIDE关键字用于定义这类符号：在目标文件内被引用，但没有在任何目标文件内被定义的符号 */
    PROVIDE (etext = _etext);
    PROVIDE (edata = _edata);
    PROVIDE (end = _end);
}
