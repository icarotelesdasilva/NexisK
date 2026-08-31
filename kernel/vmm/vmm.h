#ifndef VMM_H
#define VMM_H

#include <stdint.h>

void vmm_init(void);

void map_page(uintptr_t virtual_addr, uintptr_t physical_addr);

#endif