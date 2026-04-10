#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT_time();

void init_irq() {
    init_irq_entry(32, (uint32_t) handler_IT_time);
    init_irq_entry(50, (uint32_t) handler_IT);
}

void handler_en_C() {
    printf("Interruption en C\n");
}

void handler_time() {
    timer_IT_handler();
}