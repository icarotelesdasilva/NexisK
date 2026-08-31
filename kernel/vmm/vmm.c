#include "vmm.h"
#include <stdint.h>

extern uint32_t *pmm_alloc_page(void);
extern void pmm_free_page(uint32_t *physical_addr);

static uintptr_t *page_directory = 0;


void vmm_init()
{
    page_directory = (uintptr_t *)pmm_alloc_page();

    for (int i = 0; i < 1024; i++)
    {
        page_directory[i] = 0x00000002;
    }

    uintptr_t *first_page_table =
        (uintptr_t *)pmm_alloc_page();

    for (int i = 0; i < 1024; i++)
    {
        first_page_table[i] = (i * 4096) | 3;
    }

    page_directory[0] =
        ((uintptr_t)first_page_table) | 3;

    __asm__ volatile (
        "mov %0, %%cr3\n\t"
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0\n\t"
        :
        : "r"(page_directory)
        : "%eax", "memory"
    );
}

void map_page(uintptr_t virtual_addr, uintptr_t physical_addr)
{
    uint32_t page_directory_index;
    uint32_t page_table_index;

    page_directory_index =
        (virtual_addr >> 22) & 0x3FF;

    page_table_index =
        (virtual_addr >> 12) & 0x3FF;

    uintptr_t *page_table;

    if (page_directory[page_directory_index] & 1)
    {
        page_table =
            (uintptr_t *)(page_directory[page_directory_index] & ~0xFFF);
    }
    else
    {
        page_table =
            (uintptr_t *)pmm_alloc_page();

        for (int i = 0; i < 1024; i++)
        {
            page_table[i] = 0x00000002;
        }

        page_directory[page_directory_index] =
            ((uintptr_t)page_table) | 3;
    }

    page_table[page_table_index] =
        physical_addr | 3;

    __asm__ volatile (
        "invlpg (%0)"
        :
        : "r"(virtual_addr)
        : "memory"
    );
}