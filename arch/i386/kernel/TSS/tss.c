#include "tss.h"
#include "../GDT/gdt.h"
#include "../inlines/memset.h"

#include <stdint.h>

tss_entry_t tss_entry;

void write_tss(void *gdt_ptr)
{
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = sizeof(tss_entry) - 1;

    uint8_t *g = (uint8_t *) gdt_ptr;

    g[0] = limit & 0xFF;
    g[1] = (limit >> 8) & 0xFF;
    g[2] = base & 0xFF;
    g[3] = (base >> 8) & 0xFF;
    g[4] = (base >> 16) & 0xFF;
    g[5] = 0x89; 
    g[6] = ((limit >> 16) & 0x0F) | 0x40; 
    g[7] = (base >> 24) & 0xFF;

    local_memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0  = 0x10;
    tss_entry.esp0 = 0x90000;
    tss_entry.iomap_base = sizeof(tss_entry);
}

void set_kernel_stack(uint32_t stack)
{
    tss_entry.esp0 = stack;
}

