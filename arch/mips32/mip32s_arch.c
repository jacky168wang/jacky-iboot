
#if 0
#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>

void __attribute__((weak)) _machine_restart(void)
{
}

int do_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    _machine_restart();

    fprintf(stderr, "*** reset failed ***\n");
    return 0;
}


void write_one_tlb(int index, u32 pagemask, u32 hi, u32 low0, u32 low1)
{
    write_c0_entrylo0(low0);
    write_c0_pagemask(pagemask);
    write_c0_entrylo1(low1);
    write_c0_entryhi(hi);
    write_c0_index(index);
    tlb_write_indexed();
}

/*
 * Breath some life into the CPU...
 *
 * Set up the memory map,
 * initialize a bunch of registers,
 * initialize the UPM's
 */
void cpu_init_f ()
{

}

/*
 * initialize higher level parts of CPU like time base and timers
 */
void cpu_init_r (void)
{

}
#endif
