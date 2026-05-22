#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/cpu.h>

void idle() {
	while (1) {
        char c = kgetch();
        console_putchar(c);
		hlt();
	}
}