#include "io.hpp"

void init_pic(void)
{
    // ICW1 init
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2 init
    outb(0x21, 0x20); // start vector = 32
    outb(0xA1, 0x70); // start vector = 96

    // ICW3 init
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4 init
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // interrupt masking
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

