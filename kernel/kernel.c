// Define necessary integer types for a freestanding environment
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define VGA_TEXT_BUFFER ((uint16_t*)0xB8000)
#define VGA_COLS 80
#define VGA_ROWS 25

// Write a string to VGA text buffer
static void putstr(const char *s) {
    uint16_t* vga = VGA_TEXT_BUFFER;
    static uint16_t pos = 0;
    for (; *s; s++) {
        if (*s == '\n') {
            pos = (pos / VGA_COLS + 1) * VGA_COLS;
        } else {
            vga[pos++] = (0x0F << 8) | *s; // White on black
        }
    }
}

// Wait for a short time
static void io_wait(void) {
    for (volatile int i = 0; i < 1000000; i++);
}

// Set the frequency for the Programmable Interval Timer (PIT)
static void pit_set_frequency(uint32_t freq) {
    uint32_t divisor = 1193180 / freq;
    __asm__ __volatile__("outb %0, $0x43" : : "a"((uint8_t)0xB6));
    __asm__ __volatile__("outb %0, $0x42" : : "a"((uint8_t)(divisor & 0xFF)));
    __asm__ __volatile__("outb %0, $0x42" : : "a"((uint8_t)((divisor >> 8) & 0xFF)));
}

// Turn on the PC speaker
static void speaker_on(void) {
    uint8_t tmp;
    __asm__ __volatile__("inb $0x61, %0" : "=a"(tmp));
    if ((tmp & 0x03) != 0x03) {
        tmp |= 3;
        __asm__ __volatile__("outb %0, $0x61" : : "a"(tmp));
    }
}

// Turn off the PC speaker
static void speaker_off(void) {
    uint8_t tmp;
    __asm__ __volatile__("inb $0x61, %0" : "=a"(tmp));
    tmp &= 0xFC;
    __asm__ __volatile__("outb %0, $0x61" : : "a"(tmp));
}

// Main kernel entry point
void kmain() {
    putstr("Hello World!\n");

    // Play a tone at 440 Hz
    pit_set_frequency(440);
    speaker_on();
    io_wait();
    speaker_off();

    // Halt the CPU
    for (;;) __asm__ __volatile__("hlt");
}
