#ifndef TLB_H
#define TLB_H

#include <stdint.h>

void tlb_flush_single(uint32_t virtual_address);

#endif
