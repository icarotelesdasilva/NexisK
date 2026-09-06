#include "memory_learn.h"

extern void serial_print(const char *str);

extern void serial_print_hex(uint32_t *n);

extern void serial_print_hex64(uint64_t n);

extern void _fim_do_kernel(void);


void pmm_page_block(void *address)
{
    int entradas_reais = *(volatile uint16_t *)0x57000;
    memory_map *mm = (memory_map *)0x50000;

    for (int i = 0; i < entradas_reais; i++)
    {
        if (mm[i].type == 1)
        {
            for (uint64_t addr = mm[i].base_address;
                 addr < mm[i].base_address + mm[i].size;
                 addr += 0x1000)
            {
                if (addr == (uint64_t)address)
                {
                    serial_print("Memory Block Found:\n");

                    serial_print("Base Address: ");
                    serial_print_hex64(mm[i].base_address);

                    serial_print("\nSize: ");
                    serial_print_hex64(mm[i].size);

                    serial_print("\n\n");

                    return;
                }
            }
        }
    }
}


void pmm_init(void)
{
    int entradas_reais = *(volatile uint16_t *)0x57000;
    memory_map *mm = (memory_map *)0x50000;

    for (int i = 0; i < entradas_reais; i++)
    {
        if (mm[i].type == 1)
        {
            serial_print("Free Memory Region Found:\n");

            serial_print("Base Address: ");
            serial_print_hex64(mm[i].base_address);

            serial_print("\nSize: ");
            serial_print_hex64(mm[i].size);

            serial_print("\n\n");
        }
    }
}