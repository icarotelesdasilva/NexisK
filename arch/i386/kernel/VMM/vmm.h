#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#define VMM_FLAG_PRESENT 0x01
#define VMM_FLAG_WRITABLE 0x02
#define VMM_FLAG_USER 0x04

typedef uint32_t pte_t;
typedef uint32_t pde_t;

typedef struct {
    pte_t entries[1024];
} __attribute__((aligned(4096))) page_table_struct;

typedef struct {
    pde_t entries[1024];
} __attribute__((aligned(4096))) page_directory_struct;

void vmm_map(page_directory_struct* page_directory, uint32_t virtual_address, uint32_t physical_address, uint32_t flags);
void vmm_load_cr3(page_directory_struct* page_directory);
void vmm_enable_paging(void);
void vmm_init(void);

#endif

