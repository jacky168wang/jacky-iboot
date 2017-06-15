/*******************************************************************************
 * Description   :
 *
 *
 * Author :
 *       jiwang@ikanos.com 2011/03/16
 ******************************************************************************/
#include "global_type.h"
#include "reg_landshark.h"
#include "uart_boot.h"
#include "timer_boot.h"
#include "soc_boot.h"

void c_uart_init(void)
{
    UINT16 divisor;
    
    /* DLAB Set, 8 bit word, no parity, 1 stop bit */
    UART1->lcr = 0x83;

    divisor = UART1->u0.dll;
    if (200*1000000 == gSysClock)
        divisor = divisor << 4;/* (200M/12.5M)=16 */
    else
        divisor = (divisor << 4) + (divisor << 2);/* (250M/12.5M)=20 */

    UART1->u1.dlh = (divisor >> 8) & 0xff;
    UART1->u0.dll = divisor & 0xff;

    /* Clear DLAB Bit */
    UART1->lcr = 0x03;
}

void c_put_char(char c)
{
    c_poll_until_set(UART1->lsr, UART_LSR_BITMASK_THRE);
    UART1->u0.thr = c ;
}

int c_get_char(void)
{
    c_poll_until_set(UART1_LSR, UART_LSR_BITMASK_DR);
    return (int)(UART1->u0.rbr);
}

int c_get_char_timout(UINT32 delayClk)
{       
    UINT32 lsr;
    
    SET_COUNTER(0);
    while (GET_COUNTER() < delayClk)
    {
        if (UART1->lsr & UART_LSR_BITMASK_DR)
            return (int)(UART1->u0.rbr);
    }
    return -1;
}

void c_put_str(char *s)
{
    while(*s != '\0')
        c_put_char(*s++);
}

/* JackyWang(jiwang@ikanos.com) add the following, Aug16,2011 */

//#define c_print_hex4(u) c_put_char((u)<10 ? 0x30+(u) : 0x57+(u)) //lowercase
#define c_print_hex4(u) c_put_char((u)<10 ? 0x30+(u) : 0x37+(u)) //uppercase

/*
void c_print_hex8(UINT8 u)
{
#if 0
    c_print_hex4((u>> 4)&0x0f);
    c_print_hex4( u     &0x0f);
#else
    int i=8;
    do {
        i -= 4;
        c_print_hex4((u>>i)&0x0f);
    } while (i!=0);

#endif
}*/

void c_put_hex16(UINT16 u)
{
#if 0
    c_print_hex4((u>>12)&0x0f);
    c_print_hex4((u>> 8)&0x0f);
    c_print_hex4((u>> 4)&0x0f);
    c_print_hex4( u     &0x0f);
#else
    int i=16;
    do {
        i -= 4;
        c_print_hex4((u>>i)&0x0f);
    } while (i!=0);

#endif
}

/*
void c_print_hex32(unsigned long u)
{
#if 0
    c_print_hex4((u>>28)&0x0f);
    c_print_hex4((u>>24)&0x0f);
    c_print_hex4((u>>20)&0x0f);
    c_print_hex4((u>>16)&0x0f);
    c_print_hex4((u>>12)&0x0f);
    c_print_hex4((u>> 8)&0x0f);
    c_print_hex4((u>> 4)&0x0f);
    c_print_hex4( u     &0x0f);
#else
    int i=32;
    do {
        i -= 4;
        c_print_hex4((u>>i)&0x0f);
    } while (i!=0);

#endif
}*/

void c_put_dec(UINT32 d)
{
    UINT8 d_a[10];
    UINT32 i=0;
    
    do {
        d_a[i] = d%10;
        d = d/10;
        if (d > 0)
            d_a[++i] = d%10;
        else
            break;
    } while(1);

    do {
        c_put_char('0'+d_a[i]);
    } while(i-- > 0);
}

#if (DEBUG_LEVEL > 0)
void debugDumpStaticReg(UINT32 addr, UINT32 flag)
{
    if (flag) c_put_str("\r\n  ");

    c_put_char('[');
    c_put_hex16((UINT16)(addr>>16));
    c_put_hex16((UINT16)(addr));
    c_put_char(']');
    c_put_hex16((UINT16)(H_RD32(addr) >> 16));
    c_put_hex16((UINT16)(H_RD32(addr)));
}

void debugDumpVolatileReg(UINT32 addr, UINT32 flag)
{
    H_D32 value_before_print, value_after_print;
    
    value_before_print = H_RD32(addr);

    if (flag) c_put_str("\r\n  ");

    c_put_char('[');
    c_put_hex16((UINT16)(addr>>16));
    c_put_hex16((UINT16)(addr));
    c_put_char(']');
    
    c_put_hex16((UINT16)(value_before_print >> 16));
    c_put_hex16((UINT16)(value_before_print));
    
    c_put_char('-');
    
    value_after_print = H_RD32(addr);
    c_put_hex16((UINT16)(value_after_print >> 16));
    c_put_hex16((UINT16)(value_after_print));
}

void debugDumpRegs(UINT32 base, UINT32 count)
{
    UINT32 i;
    
    //c_put_str("\r\n  ");
    for(i=0; i<count*4; i+=4)
    {
        if (0 == (i&0xf))
        {
            c_put_str("\r\n");
            c_put_hex16((base+i) >> 16);
            c_put_hex16((base+i));
            c_put_char(':');
        }
        c_put_char(' ');
        c_put_hex16(H_RD32(base+i) >> 16);
        c_put_hex16(H_RD32(base+i));
    }
}
#endif

#if 0

char *gets(char *s)
{
   int i, k = getchar();
   /* Return NULL if no input is available */
   if (k == 0) return NULL;

   /* Read and store characters until reaching a newline or end-of-file */
   for (i = 0; k != 0 && k != '\n'; i++) {
      if (k == '\r') {
         s[i] = 0;
      } else if (k == 0x08) {
         // bs
         i-=2;
         if (i<0) i=0;
      } else {
         s[i] = k;
      }
      k = getchar();
      uartPutChar((char *)&k,1);

      /* If a read error occurs, the resulting buffer is not usable. */
      if (k == 0) {
         return NULL;
      }
   }
   /* Null-terminate and return the buffer on success.
   The newline is not stored in the buffer. */
   s[i] = '\0';
   return s;
}
#endif
