#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/time.h>
#include <n7OS/keyboard.h>
#include <n7OS/sys.h>
#include <n7OS/processus.h>
#include <unistd.h>

extern void idle();
extern void fork2();

void kernel_start(void)
{
    init_console();
    initialise_paging();

    // lancement des interruptions
    init_irq();
    sti();
    init_timer();
    init_keyboard();
    init_syscall();

    init_process();

    // on ne doit jamais sortir de kernel_start
    //while (1) {
        //char c = kgetch();
        //console_putchar(c);
        // cette fonction arrete le processeur
        //hlt();
    //}
}
