/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *     spasumar@ikanos.com 2009/06/25
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#ifndef _C_STDIO_H_
#define _C_STDIO_H_

#ifndef __va_list__
    #define __va_list__
    typedef char *va_list;
#endif
#define va_dcl int va_alist;
#define va_start(list) list = (char *) &va_alist
#define va_end(list)
#define va_arg(list,mode) ((mode *)(list += sizeof(mode)))[-1]

extern int printf(const char *, ...);
extern int sprintf(char *, const char *, ...);
extern int unformat(int (*)(), char *,int **);
extern int sscanf(char *s, char *fmt, ...);

extern int puts(char *s);

extern char *index(char *s, int c);

#endif /* _C_STDIO_H_ */
