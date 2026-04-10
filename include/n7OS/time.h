#ifndef __TIME_H__
#define __TIME_H__

#include <inttypes.h>
#include <n7OS/cpu.h>

#define FREQUENCE 1190

#define HORLOGE 1000

typedef struct
{
    int hours;
    int minutes;
    int seconds;
} time_t;

void init_timer();

void timer_IT_handler();

time_t time_converter(int time);

void masquer_IRQ();

void demasquer_IRQ();

void acquitter_IRQ();

#endif