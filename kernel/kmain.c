#include "stdint.h"

/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

extern void serial_print(const char *str);
extern void vga_clear(void);
extern void vga_print(char *str);

#include "interrupts/io.h"
#include "interrupts/pic.h"

extern void pic_remap(uint8_t offset1, uint8_t offset2);
extern void kernel_panic(char *str);
extern void serial_print_hex(uint32_t n);
void init_pit(uint32_t frequency);
extern void idt_install(void);


void kmain(void)
{
    uint16_t count = *(volatile uint16_t *)0x4FF0;

    pic_remap(0x20, 0x28);
    idt_install();
    init_pit(1000);

    vga_clear();

    serial_print("kernel alive.\n");

    serial_print("E820 entries: ");
    serial_print_hex(count);
    serial_print("\n");

    vga_print("Kernel alive.");

    for (;;)
        asm volatile ("sti");
}