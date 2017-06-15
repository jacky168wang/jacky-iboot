
#if !defined(ASM_PRINT_STRING)
#define a_put_hello \
    a_put_newline;\
    a_put_char('R');\
    a_put_char('E');\
    a_put_char('S');\
    a_put_char('E');\
    a_put_char('T')
#else
#define a_put_hello \
    a_put_str("\r\nRESET")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_PRId \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('C');\
    a_put_char('P');\
    a_put_char('0');\
    a_put_char('_');\
    a_put_char('P');\
    a_put_char('R');\
    a_put_char('I');\
    a_put_char('d');\
    a_put_char('=')
#else
#define a_put_PRId \
    a_put_str("\r\n CP0_PRId=")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_Config \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('C');\
    a_put_char('P');\
    a_put_char('0');\
    a_put_char('_');\
    a_put_char('C');\
    a_put_char('o');\
    a_put_char('n');\
    a_put_char('f');\
    a_put_char('i');\
    a_put_char('g');\
    a_put_char('=')
#else
#define a_put_Config \
    a_put_str("\r\n CP0_Config=")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_k0seg \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('K');\
    a_put_char('0');\
    a_put_char('S');\
    a_put_char('E');\
    a_put_char('G')
#else
#define a_put_k0seg \
    a_put_str("\r\n K0SEG")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_uncached \
    a_put_char(' ');\
    a_put_char('u');\
    a_put_char('n');\
    a_put_char('c');\
    a_put_char('a');\
    a_put_char('c');\
    a_put_char('h');\
    a_put_char('e');\
    a_put_char('d')
#else
#define a_put_uncached \
    a_put_str(" uncached")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_cached_unmapped \
    a_put_char(' ');\
    a_put_char('c');\
    a_put_char('a');\
    a_put_char('c');\
    a_put_char('h');\
    a_put_char('e');\
    a_put_char('d');\
    a_put_char('_');\
    a_put_char('u');\
    a_put_char('n');\
    a_put_char('m');\
    a_put_char('a');\
    a_put_char('p');\
    a_put_char('p');\
    a_put_char('e');\
    a_put_char('d')
#else
#define a_put_cached_unmapped \
    a_put_str(" cached_unmapped")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_cached_mapped \
    a_put_char(' ');\
    a_put_char('c');\
    a_put_char('a');\
    a_put_char('c');\
    a_put_char('h');\
    a_put_char('e');\
    a_put_char('d');\
    a_put_char('_');\
    a_put_char('m');\
    a_put_char('a');\
    a_put_char('p');\
    a_put_char('p');\
    a_put_char('e');\
    a_put_char('d')
#else
#define a_put_cached_mapped \
    a_put_str(" cached_mapped")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_icache \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('I');\
    a_put_char('$')
#else
#define a_put_icache \
    a_put_str("\r\n I$")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_dcache \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('D');\
    a_put_char('$')
#else
#define a_put_dcache \
    a_put_str("\r\n D$")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_0size \
    a_put_char(' ');\
    a_put_char('N');\
    a_put_char('O');\
    a_put_char('N');\
    a_put_char('E')
#else
#define a_put_0size \
    a_put_str(" NONE")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_cacheline \
    a_put_char(' ');\
    a_put_char('l');\
    a_put_char('i');\
    a_put_char('n');\
    a_put_char('e');\
    a_put_char('=')
#else
#define a_put_cacheline \
    a_put_str(" line=")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_waysets \
    a_put_char(' ');\
    a_put_char('w');\
    a_put_char('a');\
    a_put_char('y');\
    a_put_char('s');\
    a_put_char('e');\
    a_put_char('t');\
    a_put_char('s');\
    a_put_char('=')
#else
#define a_put_waysets \
    a_put_str(" waysets=")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_cacheinit \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char(' ');\
    a_put_char('c');\
    a_put_char('l');\
    a_put_char('e');\
    a_put_char('a');\
    a_put_char('r');\
    a_put_char(' ');\
    a_put_char('t');\
    a_put_char('a');\
    a_put_char('g')
#else
#define a_put_cacheinit \
    a_put_str("\r\n  clear tag")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_watch \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('w');\
    a_put_char('a');\
    a_put_char('t');\
    a_put_char('c');\
    a_put_char('h')
#else
#define a_put_watch \
    a_put_str("\r\n watch")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_status \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('s');\
    a_put_char('t');\
    a_put_char('a');\
    a_put_char('t');\
    a_put_char('u');\
    a_put_char('s')
#else
#define a_put_status \
    a_put_str("\r\n status")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_intenv \
    a_put_newline;\
    a_put_char(' ');\
    a_put_char('I');\
    a_put_char('N');\
    a_put_char('T');\
    a_put_char(' ');\
    a_put_char('e');\
    a_put_char('n');\
    a_put_char('a');\
    a_put_char('b');\
    a_put_char('l');\
    a_put_char('e')
#else
#define a_put_intenv \
    a_put_str("\r\n enable INT")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_cjump \
    a_put_newline;\
    a_put_char('e');\
    a_put_char('n');\
    a_put_char('t');\
    a_put_char('e');\
    a_put_char('r');\
    a_put_char(' ');\
    a_put_char('\'');\
    a_put_char('C');\
    a_put_char('\'');\
    a_put_newline;
#else
#define a_put_intenv \
    a_put_str("\r\nenter 'C'\r\n")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_exception \
    a_put_newline;\
    a_put_char('E');\
    a_put_char('X');\
    a_put_char('C');\
    a_put_char('E');\
    a_put_char('P');\
    a_put_char('T');\
    a_put_char('I');\
    a_put_char('O');\
    a_put_char('N')
#else
#define a_put_exception \
    a_put_str("\r\nEXCEPTION")
#endif

#if !defined(ASM_PRINT_STRING)
#define a_put_interrupt \
    a_put_newline;\
    a_put_char('I');\
    a_put_char('N');\
    a_put_char('T');\
    a_put_char('E');\
    a_put_char('R');\
    a_put_char('R');\
    a_put_char('U');\
    a_put_char('P');\
    a_put_char('T')
#else
#define a_put_interrupt \
    a_put_str("\r\nINTERRUPT")
#endif
