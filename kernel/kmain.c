/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include <stdint.h>

#include "interrupts/pic.h"

extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t n);

extern void vga_clear(void);
extern void vga_print(const char *str);

extern void pic_remap(uint8_t offset1, uint8_t offset2);
extern void idt_install(void);
extern void init_pit(uint32_t frequency);

extern void mouse_register_interrupt(void);
extern void ps2_mouse_init(int screen_width, int screen_height);
extern void unmask_mouse_irq(void);

extern void ring3_test(void);

void kmain(void)
{





    pic_remap(0x20, 0x28);


    idt_install();


    init_pit(1000);

    mouse_register_interrupt();
    ps2_mouse_init(800, 600);
    unmask_mouse_irq();


    vga_clear();

    serial_print("kernel alive.\n");

    vga_print("Kernel alive.");

    ring3_test();

    for (;;)
        asm volatile ("hlt");
}
