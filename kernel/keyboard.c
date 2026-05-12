#include <n7OS/keyboard.h>

static char kbd_buffer[256];
int tete = 0;
int queue = 0;
uint8_t shift = 0;

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
    if (tete == queue)
		return 0;
	char c = kbd_buffer[tete];
	tete=(tete+1)%256;
	return c;
}

void keyboard_IT_handler() {
    outb(0x20, 0x20);
    uint8_t key = inb(0x60);
    if (key == SHIFT_PRESSED || key == SHIFT_RELEASED)
	{
		shift = key == SHIFT_PRESSED ? 1 : 0;
		return;
	}
	char c = shift ? scancode_map_shift[key] : scancode_map[key];
    if (key & 0x80) {
    } else if (c == 0) {
    } else {
        buffer_put(c);
    }
}

void acquitter_IRQ_Keyboard() {
    outb(0x20, 0x21);
}