#include <n7OS/console.h>
#include <n7OS/cpu.h>

uint16_t *scr_tab;

void console_putcursor(uint16_t pos) {
    outb(0x0F, 0x3D4);
    outb(pos & 0xFF, 0x3D5);
    outb(0x0E, 0x3D4);
    outb((pos >> 8) & 0xFF, 0x3D5);
}

uint16_t console_getcursor() {
    uint16_t pos = 0;
    outb(0x0F, 0x3D4);
    pos = (uint16_t)inb(0x3D5);
    outb(0x0E, 0x3D4);
    pos |= ((uint16_t)inb(0x3D5) << 8);
    return pos;
}

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
}

void console_putchar(const char c) {
    uint16_t pos = console_getcursor();
    if (c > 31 && c < 127) {
            scr_tab[pos]= CHAR_COLOR<<8|c;
            pos++;
            console_putcursor(pos);
    } else if (c == 8) {
        console_putcursor(pos-1);
    } else if (c == 9) {
        console_putcursor(pos+8);
    } else if (c == 10) {
        console_putcursor(80*(1+(pos/80)));
    } else if (c == 12) {
        for (int i = 0; i < VGA_WIDTH*VGA_HEIGHT;i++) {
            scr_tab[i] = CHAR_COLOR<<8|' ';
        }
        console_putcursor(0);
    } else if (c == 13) {
        console_putcursor(80*(pos/80));
    }
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}