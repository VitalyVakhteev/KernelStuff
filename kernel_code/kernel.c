

#define VGA_ADDRESS 0xB8000   /* video memory begins here. */

/* VGA provides support for 16 colors */
#define BLACK 0
#define GREEN 2
#define RED 4
#define YELLOW 14
#define WHITE_COLOR 15
#include <stdio.h>

unsigned short *terminal_buffer;
unsigned int vga_index;

void clear_screen(void)
{
    int index = 0;
    /* there are 25 lines each of 80 columns;
       each element takes 2 bytes */
    while (index < 80 * 25 * 2) {
        terminal_buffer[index] = ' ';
        index += 2;
    }
}

void print_string(char *str, unsigned char color)
{
    int index = 0;
    while (str[index]) {
        terminal_buffer[vga_index] = (unsigned short)str[index]|(unsigned short)color << 8;
        index++;
        vga_index++;
    }
}

#include <stdint.h>

#define PIT_CHANNEL_2 0x42
#define PIT_CONTROL 0x43
#define SPEAKER_CTRL 0x61

void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void itoa(int value, char *str, int base) {
    char *ptr = str, *ptr1 = str, tmp_char;
    int tmp_value;

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
    } while (value);

    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}

void set_pit_frequency(uint32_t frequency) {
    uint16_t divisor = 1193180 / frequency;
    print_string("Setting PIT frequency...\n", WHITE_COLOR);

    char msg[50];
    print_string(msg, WHITE_COLOR);

    outb(PIT_CONTROL, 0xB6); // Configure PIT: Channel 2, Mode 3 (square wave)
    outb(PIT_CHANNEL_2, divisor & 0xFF); // Low byte
    outb(PIT_CHANNEL_2, (divisor >> 8) & 0xFF); // High byte
    print_string("PIT frequency set.\n", GREEN);
}

void enable_speaker() {
    print_string("Enabling speaker...\n", WHITE_COLOR);
    uint8_t port_val = inb(SPEAKER_CTRL);

    char msg[50];
    itoa(port_val, msg, 16);  // Convert port_val to a string in base 16 (hexadecimal)
    print_string("Speaker port value: 0x", WHITE_COLOR);
    print_string(msg, WHITE_COLOR);
    print_string("\n", WHITE_COLOR);

    outb(SPEAKER_CTRL, port_val | 0x03); // Enable speaker and link to PIT channel 2
    print_string("Speaker enabled.\n", GREEN);
}

void disable_speaker() {
    print_string("Disabling speaker...\n", WHITE_COLOR);
    uint8_t port_val = inb(SPEAKER_CTRL);

    char msg[50];
    itoa(port_val, msg, 16);  // Convert port_val to a string in base 16 (hexadecimal)
    print_string("Speaker port value before disabling: 0x", WHITE_COLOR);
    print_string(msg, WHITE_COLOR);
    print_string("\n", WHITE_COLOR);

    outb(SPEAKER_CTRL, port_val & 0xFC); // Disable speaker
    print_string("Speaker disabled.\n", GREEN);
}

void play_note_debug(uint32_t frequency, uint32_t duration_ms) {
    char msg[50];
    itoa(frequency, msg, 10);  // Convert frequency to a string
    print_string("Playing note: ", WHITE_COLOR);
    print_string(msg, WHITE_COLOR);
    print_string(" Hz for ", WHITE_COLOR);
    itoa(duration_ms, msg, 10);  // Convert duration to a string
    print_string(msg, WHITE_COLOR);
    print_string(" ms\n", WHITE_COLOR);

    set_pit_frequency(frequency);
    enable_speaker();

    for (volatile uint32_t i = 0; i < duration_ms * 1000; i++);

    disable_speaker();
    print_string("Note finished.\n", GREEN);
}

void play_sound() {
    while (1) {
        play_note_debug(261, 500); // C note for 500ms
        play_note_debug(293, 500); // D note for 500ms
    }
}

void main(void) {
    terminal_buffer = (unsigned short *)VGA_ADDRESS;
    vga_index = 0;

    clear_screen();
    print_string("Playing sound...", YELLOW);

    play_sound(); // Play sound in an infinite loop

    return;
}

