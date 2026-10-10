#include "pic.h"
#include <stdint.h>
#include "../inlines/io.h"

void pic_remap(uint8_t offset1, uint8_t offset2) {
    outb(0x20, 0x11); io_wait();
    outb(0xA0, 0x11); io_wait();

    outb(0x21, offset1); io_wait();
    outb(0xA1, offset2); io_wait();

    outb(0x21, 0x04); io_wait();
    outb(0xA1, 0x02); io_wait();

    outb(0x21, 0x01); io_wait();
    outb(0xA1, 0x01); io_wait();

    outb(0x21, 0x00); io_wait();
    outb(0xA1, 0x00); io_wait();
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
}

