
#include "tlb.h"
#include <stdint.h>

void tlb_flush_single(uint32_t virtual_address) {

    __asm__ __volatile__("invlpg (%0)" :: "r" (virtual_address) : "memory");

}



