/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _C_CHAR_H_
#define _C_CHAR_H_

/* Control characters */
#define CTRL_A          ('A'-0x40)
#define CTRL_B          ('B'-0x40)
#define CTRL_C          ('C'-0x40)
#define CTRL_D          ('D'-0x40)
#define CTRL_E          ('E'-0x40)
#define CTRL_F          ('F'-0x40)
#define CTRL_H          ('H'-0x40)
#define CTRL_K          ('K'-0x40)
#define CTRL_N          ('N'-0x40)
#define CTRL_P          ('P'-0x40)
#define CTRL_U          ('U'-0x40)
#define DEL             0x7F
#define TAB             0x09
#define CR              0x0D
#define LF              0x0A
#define ESC             0x1B
#define SP              0x20
#define CSI             0x9B
#define EOF_SREC        -2
#define UART_ERROR      -1

#define _UPC    0x1
#define _LWR    0x2
#define _DIG    0x4
#define _SPC    0x8
#define _PUN    0x10
#define _CTR    0x20
#define _HEX    0x40
#define _BLK    0x80

extern char _ctype[];

#define isalpha(c)  ((_ctype)[(unsigned char)(c)]&(_UPC|_LWR))
#define isupper(c)  ((_ctype)[(unsigned char)(c)]&_UPC)
#define islower(c)  ((_ctype)[(unsigned char)(c)]&_LWR)
#define isdigit(c)  ((_ctype)[(unsigned char)(c)]&_DIG)
#define isxdigit(c) ((_ctype)[(unsigned char)(c)]&(_DIG|_HEX))
#define isspace(c)  ((_ctype)[(unsigned char)(c)]&_SPC)
#define ispunct(c)  ((_ctype)[(unsigned char)(c)]&_PUN)
#define isalnum(c)  ((_ctype)[(unsigned char)(c)]&(_UPC|_LWR|_DIG))
#define isprint(c)  ((_ctype)[(unsigned char)(c)]&(_PUN|_UPC|_LWR|_DIG|_BLK))
#define isgraph(c)  ((_ctype)[(unsigned char)(c)]&(_PUN|_UPC|_LWR|_DIG))
#define iscntrl(c)  ((_ctype)[(unsigned char)(c)]&_CTL)
#define isascii(c)  ((unsigned char)(c)<=0x7f)
#define toascii(c)  ((unsigned char)(c)&0x7f)

extern int toupper(int);
extern int tolower(int);

#endif /* _C_CHAR_H_ */
