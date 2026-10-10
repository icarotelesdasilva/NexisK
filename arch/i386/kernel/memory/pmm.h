#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define MULTIBOOT_INFO_MEM_MAP         0x00000040
#define PAGE_SIZE                      4096

struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed));

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed));

void init_physical_memory(struct multiboot_info* mbi);
void* alloc_page();
void free_page(void* ptr);

#endif

