#include "pmm.h"

#define MIN_ALLOC_ADDR 0x1000

uint32_t* bitmap;
uint32_t total_pages = 0;
uint32_t bitmap_size = 0;

extern uint32_t _kernel_end;

static void bitmap_set(uint32_t page_index) {
    bitmap[page_index / 32] |= (1 << (page_index % 32));
}

static void bitmap_clear(uint32_t page_index) {
    bitmap[page_index / 32] &= ~(1 << (page_index % 32));
}

static int bitmap_test(uint32_t page_index) {
    return (bitmap[page_index / 32] & (1 << (page_index % 32))) != 0;
}

void* alloc_page() {
    uint32_t start_page_index = MIN_ALLOC_ADDR / PAGE_SIZE;

    for (uint32_t i = start_page_index; i < total_pages; i++) {
        if (bitmap_test(i) == 0) {
            bitmap_set(i);
            uint32_t physical_address = i * PAGE_SIZE;
            return (void*)physical_address;
        }
    }
    return 0; 
}

void free_page(void* ptr) {
    uint32_t physical_address = (uint32_t)ptr;
    uint32_t page_index = physical_address / PAGE_SIZE;
    bitmap_clear(page_index);
}

void init_physical_memory(struct multiboot_info* mbi) {
    bitmap = (uint32_t*)&_kernel_end;

    total_pages = 1048576; 
    bitmap_size = total_pages / 8;

    uint8_t* byte_bitmap = (uint8_t*)bitmap;
    for (uint32_t i = 0; i < bitmap_size; i++) {
        byte_bitmap[i] = 0xFF;
    }

    if (mbi->flags & MULTIBOOT_INFO_MEM_MAP) {
        struct multiboot_mmap_entry* mmap = (struct multiboot_mmap_entry*)mbi->mmap_addr;

        while ((uint32_t)mmap < (mbi->mmap_addr + mbi->mmap_length)) {
            if (mmap->type == 1) { 
                uint32_t page_start = (uint32_t)(mmap->addr / PAGE_SIZE);
                uint32_t page_end = (uint32_t)((mmap->addr + mmap->len) / PAGE_SIZE);

                for (uint32_t p = page_start; p < page_end; p++) {
                    bitmap_clear(p);
                }
            }
            mmap = (struct multiboot_mmap_entry*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
        }
    }

    uint32_t forbidden_pages = MIN_ALLOC_ADDR / PAGE_SIZE;
    for (uint32_t p = 0; p < forbidden_pages; p++) {
        bitmap_set(p);
    }

    uint32_t kernel_and_bitmap_end = (uint32_t)bitmap + bitmap_size;
    uint32_t protected_pages = (kernel_and_bitmap_end / PAGE_SIZE) + 1;

    for (uint32_t p = 0; p < protected_pages; p++) {
        bitmap_set(p);
    }
}

