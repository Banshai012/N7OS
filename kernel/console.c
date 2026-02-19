#include <n7OS/console.h>
#include <n7OS/cpu.h>

uint16_t *scr_tab;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
    console_putcursor(0);
}

void console_putchar(const char c, int pos) {
    if (c > 31 && c < 127) {
            scr_tab[pos]= CHAR_COLOR<<8|c;
            pos++;
            console_putcursor(pos);
        }

}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i], i);
    }
}

void console_putcursor(uint16_t pos) {
    outb(0x0F, 0x3D4);
    outb(pos & 0xFF, 0x3D5);
    outb(0x0E, 0x3D4);
    outb((pos >> 8) & 0xFF, 0x3D5);
}

uint16_t console_getcursor() {
    
}