#include <n7OS/keyboard.h>

static char kbd_buffer[256];
int tete = 0;
int queue = 0;

void init_keyboard() {
    outb(inb(0x21) & ~(1 << 1), 0x21);
}

void buffer_put(char c) {
    if (((queue+1)%256)>((tete+1)%256)){
    } else {
        kbd_buffer[queue]=c;
        queue+=1;
    }
}

char kgetch() {
    char c= '\0';

    return c;
}

void keyboard_IT_handler() {
    uint8_t key = inb(0x60);
    char c = scancode_map[key];
    if (key & 0x80) {
    } else { 
        buffer_put(c);
    }
}

void acquitter_IRQ() {
    outb(0x20, 0x21);
}