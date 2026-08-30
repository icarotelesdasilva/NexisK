#ifndef PMM_H
#define PMM_H

#include <stdint.h>

struct PMM
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t ACPI_attributes;
};

void pmm_init(void);
uint32_t pmm_alloc_page(void);
void pmm_free_page(uint32_t address);

#endif