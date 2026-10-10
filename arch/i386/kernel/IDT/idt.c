#include "idt.h"
#include <stdint.h>
#include <stdbool.h>

extern void isr_stub_34(void);
extern void* isr_stub_table[];

__attribute__((aligned(0x10))) idt_entry_t idt[IDT_MAX_DESCRIPTORS];
idtr_t idtr;
bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; 
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init(void);
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1;

    for (uint8_t vector = 0; vector < 34; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    idt_set_descriptor(34, isr_stub_34, 0xEE); 
    vectors[34] = true; 

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

