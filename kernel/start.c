#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>

void kernel_start(void)
{
    init_console();
    PDE* dir = initialise_paging();
    setup_base(dir /* la memoire virtuelle n'est pas encore definie */);
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
