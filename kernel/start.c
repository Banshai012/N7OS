#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>

void kernel_start(void)
{
    init_console();
    initialise_paging();
    printf("blablabla\nblebleble\tbliblibli\bblobloblo\rblu\fblyblybly");

    // lancement des interruptions
    sti();
    //alloc_page_entry(0xA0000000, 1, 1);
    uint32_t *ptr = (uint32_t*) 0xA0000000;
    (*ptr)++;

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
