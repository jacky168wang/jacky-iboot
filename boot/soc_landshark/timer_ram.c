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

#if 0
void mdelay(unsigned int msec)
{
    /* Count increments every two clocks in Fusiv's realization */
    SET_COMPARE(gSysClock);
    /* DC bit in the Cause register must be 0 */
    SET_COUNTER(0);
    do {
    } while (GET_COUNTER() <= (gSysClock/1000) * msec);
}
#endif
void mdelay(unsigned int msec)
{
   SET_COMPARE(245000000);
   SET_COUNTER(0);
   do {
   } while (GET_COUNTER() <= (250000 * msec));
}

#if 0
#include <common.h>
#include <asm/mipsregs.h>

static unsigned long timestamp;
static unsigned long mips_timer_freq = CONFIG_SYS_MIPS_TIMER_FREQ;


/* how many counter cycles in a jiffy */
#define CYCLES_PER_JIFFY    (CONFIG_SYS_MIPS_TIMER_FREQ + CONFIG_SYS_HZ / 2) / CONFIG_SYS_HZ

#define GET_CYCLES_PER_JIFFY (mips_timer_freq + CONFIG_SYS_HZ / 2) / CONFIG_SYS_HZ
/*
 * timer without interrupts
 */

#if IS_VX185FAMILY
static void set_mips_timer_freqency(void)
{
    volatile unsigned int chip_type;

    chip_type = ((*(volatile unsigned int *) SYS_CTRL_REG) >> 4);
    chip_type &= 0x07;

   if((chip_type == CHIP_7183)  || (chip_type == CHIP_7173))/* Indicates Vx183 */
        mips_timer_freq = CONFIG_SYS_MIPS_VX183_TIMER_FREQ;

}
#endif
int timer_init(void)
{
    /* Set up the timer for the first expiration. */
#if IS_VX185FAMILY
    set_mips_timer_freqency();
#endif
    timestamp = 0;
    write_c0_compare(read_c0_count() + GET_CYCLES_PER_JIFFY);

    return 0;
}

void reset_timer(void)
{
#if IS_VX185FAMILY
    set_mips_timer_freqency();
#endif
    timestamp = 0;
    write_c0_compare(read_c0_count() + GET_CYCLES_PER_JIFFY);
}

ulong get_timer(ulong base)
{
    unsigned int count;
    unsigned int expirelo = read_c0_compare();

    /* Check to see if we have missed any timestamps. */
#if IS_VX185FAMILY
    set_mips_timer_freqency();
#endif
    count = read_c0_count();
    while ((count - expirelo) < 0x7fffffff) {
        expirelo += GET_CYCLES_PER_JIFFY;
        timestamp++;
    }
    write_c0_compare(expirelo);

    return (timestamp - base);
}

void set_timer(ulong t)
{
    unsigned int count;
    timestamp = t;
#if IS_VX185FAMILY
    set_mips_timer_freqency();
#endif
    count = read_c0_count();
    write_c0_compare(read_c0_count() + GET_CYCLES_PER_JIFFY);
}

void __udelay(unsigned long usec)
{
    unsigned int tmo;

    tmo = read_c0_count() + (usec * (mips_timer_freq / 1000000));
    while ((tmo - read_c0_count()) < 0x7fffffff)
        /*NOP*/;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On MIPS it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
    return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On MIPS it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
    return CONFIG_SYS_HZ;
}
#endif
