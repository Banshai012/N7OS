#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/time.h>
#include <n7OS/sys.h>
#include <unistd.h>

void kernel_start(void)
{
    init_console();
    initialise_paging();

    // lancement des interruptions
    init_irq();
    sti();
    init_timer();
    init_syscall();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
