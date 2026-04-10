#include "n7OS/console.h"
#include "n7OS/time.h"

int tps = 0;

void init_timer() {
    outb(0x34, 0x43);
    outb(FREQUENCE & 0xFF, 0x40);
    outb(FREQUENCE >> 8, 0x40);
    outb(inb(0x20) & 0xFE, 0x21);

    tps = 0;
    console_put_time(time_converter(tps));
}

void timer_IT_handler() {
    acquitter_IRQ();
    tps++;
    if (tps % HORLOGE == 0) {
        console_put_time(time_converter(tps));
    }
}

time_t time_converter(int time) {
    time_t time_conv;
    time_conv.hours = (time / 3600000);
    time_conv.minutes = (time % 3600000) / 60000;
    time_conv.seconds = (time % 60000) / 1000;
    return time_conv;
}

void masquer_IRQ() {
    outb(inb(0x21) | 1, 0x21);
}

void demasquer_IRQ() {
    outb(inb(0x21) &0xfe, 0x21);
}

void acquitter_IRQ() {
    outb(0x20, 0x20);
}