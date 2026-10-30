#include <stdint.h>
#include <stddef.h>

#define VGA ((volatile uint16_t*)0xB8000)
#define WIDTH 80
#define HEIGHT 25
#define COLOR 0x0F
#define KEYBOARD_STATUS 0x64
#define KEYBOARD_DATA 0x60

static size_t row = 0, col = 0;
static uint8_t color = COLOR;

static void put_at(char c, size_t x, size_t y) {
    VGA[y * WIDTH + x] = (uint16_t)c | ((uint16_t)color << 8);
}
static void clear_screen(void) {
    for (size_t y = 0; y < HEIGHT; y++)
        for (size_t x = 0; x < WIDTH; x++) put_at(' ', x, y);
    row = 0; col = 0;
}
static void scroll(void) {
    for (size_t y = 1; y < HEIGHT; y++)
        for (size_t x = 0; x < WIDTH; x++)
            VGA[(y - 1) * WIDTH + x] = VGA[y * WIDTH + x];
    for (size_t x = 0; x < WIDTH; x++) put_at(' ', x, HEIGHT - 1);
    row = HEIGHT - 1;
}
static void putchar_os(char c) {
    if (c == '\n') { col = 0; if (++row >= HEIGHT) scroll(); return; }
    if (c == '\r') { col = 0; return; }
    if (c == '\b') {
        if (col) { col--; put_at(' ', col, row); }
        return;
    }
    put_at(c, col, row);
    if (++col >= WIDTH) { col = 0; if (++row >= HEIGHT) scroll(); }
}
static void print(const char *s) { while (*s) putchar_os(*s++); }

static const char *keymap[128] = {
    [0x02]="1",[0x03]="2",[0x04]="3",[0x05]="4",[0x06]="5",
    [0x07]="6",[0x08]="7",[0x09]="8",[0x0A]="9",[0x0B]="0",
    [0x0C]="-",[0x0D]="=",[0x0E]="\b",[0x0F]="\t",
    [0x10]="q",[0x11]="w",[0x12]="e",[0x13]="r",[0x14]="t",
    [0x15]="z",[0x16]="u",[0x17]="i",[0x18]="o",[0x19]="p",
    [0x1A]="ü",[0x1B]="+",[0x1C]="\n",
    [0x1E]="a",[0x1F]="s",[0x20]="d",[0x21]="f",[0x22]="g",
    [0x23]="h",[0x24]="j",[0x25]="k",[0x26]="l",[0x27]="ö",
    [0x28]="ä",[0x2C]="y",[0x2D]="x",[0x2E]="c",[0x2F]="v",
    [0x30]="b",[0x31]="n",[0x32]="m",[0x33]=",",[0x34]=".",
    [0x35]="#",[0x39]=" "
};

static char input[80];
static size_t input_len = 0;

static void prompt(void) { print("\nNovaOS> "); }
static int same(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == *b;
}
static void run_command(void) {
    input[input_len] = 0;
    print("\n");
    if (same(input, "help")) {
        print("Befehle: help, about, clear, echo TEXT\n");
    } else if (same(input, "about")) {
        print("NovaOS 0.1 - dein eigenes Hobby-Betriebssystem!\n");
        print("Kernel: C / x86 | Boot: GRUB Multiboot2\n");
    } else if (same(input, "clear")) {
        clear_screen();
    } else if (input[0]=='e' && input[1]=='c' && input[2]=='h' && input[3]=='o' && input[4]==' ') {
        print(input + 5); print("\n");
    } else if (input_len) {
        print("Unbekannter Befehl. Tippe help.\n");
    }
    input_len = 0;
    prompt();
}
static void keyboard_poll(void) {
    if (!( *(volatile uint8_t*)KEYBOARD_STATUS & 1)) return;
    uint8_t sc = *(volatile uint8_t*)KEYBOARD_DATA;
    if (sc & 0x80) return; /* key release */
    if (sc >= 128 || !keymap[sc]) return;
    char c = keymap[sc][0];
    if (c == '\t') return;
    if (c == '\n') { run_command(); return; }
    if (c == '\b') {
        if (input_len) { input_len--; putchar_os('\b'); }
        return;
    }
    if (input_len < sizeof(input)-1) {
        input[input_len++] = c;
        putchar_os(c);
    }
}

void kernel_main(uint32_t magic, uint32_t mbi) {
    (void)mbi;
    clear_screen();
    print("========================================\n");
    print("              Willkommen bei\n");
    print("                  NovaOS\n");
    print("========================================\n");
    if (magic != 0x36D76289) print("Warnung: Unerwartete Boot-Magic.\n");
    print("NovaOS 0.1 | 32-bit Hobby-Kernel\n");
    print("Tippe 'help' fuer die Befehle.\n");
    prompt();
    for (;;) keyboard_poll();
}
