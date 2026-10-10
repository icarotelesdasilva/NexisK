#include "IDT/idt.h"
#include "GDT/gdt.h"
#include "drivers/vga.h"
#include "interrupts/pic.h"
#include "memory/pmm.h"
#include "VMM/vmm.h"
#include "drivers/kpanic.h"
#include "inlines/io.h"
#include "TSS/tss.h"

extern void jump_usermode(void);

void services(uint32_t multiboot_ptr) {
    gdt_init();
    gdt_user_space_segment();
    idt_init();
    pic_remap(0x20, 0x28);

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
/*
  //  init_physical_memory((struct multiboot_info*)multiboot_ptr);
//    vmm_init();
*/ // temporary

}

void kmain(uint32_t magic, uint32_t multiboot_ptr) {
    if (magic != 0x2BADB002) {
        kpanic("MAGIC BOOT INVALID!");
    }

    services(multiboot_ptr);
    asm volatile("sti");

    vga_clear();
    vga_print("Hello, World!");
    vga_print("\n\nkeyboard actived.");

    jump_usermode();

    for (;;) {
        asm volatile("hlt");
    }
}

