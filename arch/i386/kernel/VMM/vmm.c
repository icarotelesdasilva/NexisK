#include "vmm.h"
#include "../memory/pmm.h"
#include "../TLB/tlb.h"

extern uint32_t _kernel_end;

void vmm_map(page_directory_struct* page_directory, uint32_t virtual_address, uint32_t physical_address, uint32_t flags){
    uint32_t page_directory_index = virtual_address >> 22;
    uint32_t page_table_index = (virtual_address >> 12) & 0x3FF;

    physical_address &= 0xFFFFF000;

    pde_t* page_directory_entry = &page_directory->entries[page_directory_index];
    page_table_struct* page_table_entry;

    if(!(*page_directory_entry & VMM_FLAG_PRESENT)){
        uint32_t new_table = (uint32_t)alloc_page();
        *page_directory_entry = new_table | VMM_FLAG_PRESENT | VMM_FLAG_WRITABLE | VMM_FLAG_USER;
        page_table_entry = (page_table_struct*)new_table;
        for(uint32_t i = 0; i < 1024; i++){
            page_table_entry->entries[i] = 0;
        }
    }else{
        page_table_entry = (page_table_struct*)(*page_directory_entry & 0xFFFFF000);
    }

    page_table_entry->entries[page_table_index] = physical_address | VMM_FLAG_PRESENT | flags;
    tlb_flush_single(virtual_address);
}

void vmm_load_cr3(page_directory_struct* page_directory){
    __asm__ __volatile__("mov %0, %%cr3" : : "r"(page_directory) : "memory");
}

void vmm_enable_paging(void){
    __asm__ __volatile__(
        "mov %%cr0, %%eax\n\t"
        "or $0x80000000, %%eax\n\t"
        "mov %%eax, %%cr0"
        :
        :
        : "eax"
    );
}

void vmm_init(void){
    page_directory_struct* boot_dir = (page_directory_struct*)alloc_page();

    for(int i = 0; i < 1024; i++) {
        boot_dir->entries[i] = 0;
    }

    uint32_t bitmap_size = 1048576 / 8;
    uint32_t kernel_and_bitmap_end = (uint32_t)&_kernel_end + bitmap_size;
    uint32_t max_initial_map = 0x400000;

    if (kernel_and_bitmap_end > max_initial_map) {
        max_initial_map = (kernel_and_bitmap_end + 0x3FFFFF) & 0xFFC00000;
    }

    for(uint32_t addr = 0; addr < max_initial_map; addr += 4096){
        vmm_map(boot_dir, addr, addr, VMM_FLAG_WRITABLE | VMM_FLAG_USER);
    }

    vmm_map(boot_dir, 0xB8000, 0xB8000, VMM_FLAG_WRITABLE | VMM_FLAG_USER);

    vmm_load_cr3(boot_dir);
    vmm_enable_paging();
}

