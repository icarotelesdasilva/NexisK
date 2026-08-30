/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */


#include "stdint.h"
#include "memory/pmm.h"
#include "vmm/vmm.h"
#include "interrupts/pic.h"


extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t n);

extern void vga_clear(void);
extern void vga_print(char *str);


extern void pic_remap(uint8_t offset1, uint8_t offset2);
extern void idt_install(void);
extern void init_gdt(void);
extern void init_pit(uint32_t frequency);


extern void mouse_register_interrupt(void);
extern void ps2_mouse_init(int screen_width, int screen_height);
extern void unmask_mouse_irq(void);


void kmain(void)
{
    uint16_t count = *(volatile uint16_t *)0x4FF0;



    init_gdt();



    pic_remap(0x20, 0x28);


    pmm_init();
    vmm_init();



    idt_install();


    init_pit(1000);



    mouse_register_interrupt();
    ps2_mouse_init(800, 600);
    unmask_mouse_irq();



    vga_clear();

    serial_print("kernel alive.\n");

    serial_print("E820 entries: ");
    serial_print_hex(count);
    serial_print("\n");


    uint32_t page1 = pmm_alloc_page();
    uint32_t page2 = pmm_alloc_page();

    serial_print("Page 1: ");
    serial_print_hex(page1);
    serial_print("\n");

    serial_print("Page 2: ");
    serial_print_hex(page2);
    serial_print("\n");


    pmm_free_page(page1);


    uint32_t page3 = pmm_alloc_page();

    serial_print("Page 3: ");
    serial_print_hex(page3);
    serial_print("\n");



    vga_print("Kernel alive.");

    for (;;)
        asm volatile ("sti");
}