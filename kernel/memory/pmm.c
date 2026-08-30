/*
 * Nexis Kernel
 * Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 * Author: @icarotelesdasilva
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "pmm.h"
#include <stdint.h>

extern void serial_print(const char *str);
extern void serial_print_hex(uint32_t n);
extern void kpanic(const char *str);
extern uint32_t _fim_do_kernel;

#define PAGE_SIZE    4096U
#define MEMORY_START 0x00100000U
#define STACK_END    0x00090000U
#define MAX_REGIONS  64

static uint8_t *pmm_bitmap;
static uint32_t pmm_max_pages;

static uint32_t region_base[MAX_REGIONS];
static uint32_t region_end[MAX_REGIONS];
static uint32_t region_count;

static uint32_t kernel_first_page;
static uint32_t kernel_last_page;

void pmm_init(void)
{
    uint16_t count = *(volatile uint16_t *)0x4FF0;
    struct PMM *entries = (struct PMM *)0x5000;

    uint32_t max_ram_addr = MEMORY_START;

    region_count = 0;

    for (uint16_t i = 0; i < count; i++)
    {
        if (entries[i].type != 1)
            continue;

        uint64_t base = entries[i].base;
        uint64_t end = entries[i].base + entries[i].length;

        if (end <= MEMORY_START)
            continue;

        if (base < MEMORY_START)
            base = MEMORY_START;

        if (region_count >= MAX_REGIONS)
            break;

        region_base[region_count] = (uint32_t)base;
        region_end[region_count] = (uint32_t)end;

        if (end > max_ram_addr)
            max_ram_addr = (uint32_t)end;

        region_count++;
    }

    
    pmm_max_pages = max_ram_addr / PAGE_SIZE;

    
    pmm_bitmap =
        (uint8_t *)(((uint32_t)&_fim_do_kernel + PAGE_SIZE - 1)
                    & ~(PAGE_SIZE - 1));

    for (uint32_t i = 0; i < pmm_max_pages; i++)
        pmm_bitmap[i] = 1;

    
    for (uint32_t r = 0; r < region_count; r++)
    {
        uint32_t start_page =
            (region_base[r] + PAGE_SIZE - 1) / PAGE_SIZE;

        uint32_t end_page =
            region_end[r] / PAGE_SIZE;

        for (uint32_t page = start_page;
             page < end_page && page < pmm_max_pages;
             page++)
        {
            pmm_bitmap[page] = 0;
        }
    }

    /*
     * Reservar kernel.
     */
    kernel_first_page =
        0x00010000U / PAGE_SIZE;

    kernel_last_page =
        ((uint32_t)&_fim_do_kernel + PAGE_SIZE - 1) / PAGE_SIZE;

    for (uint32_t page = kernel_first_page;
         page < kernel_last_page && page < pmm_max_pages;
         page++)
    {
        pmm_bitmap[page] = 1;
    }

  
    uint32_t bitmap_size = pmm_max_pages;

    uint32_t bitmap_start =
        (uint32_t)pmm_bitmap;

    uint32_t bitmap_end =
        bitmap_start + bitmap_size;

    if (bitmap_end >= STACK_END)
        kpanic("PMM: Bitmap overlaps stack");

    

    serial_print("PMM initialized.\n");

    serial_print("Max pages: ");
    serial_print_hex(pmm_max_pages);

    serial_print("\nBitmap: ");
    serial_print_hex((uint32_t)pmm_bitmap);

    serial_print("\n");
}

uint32_t pmm_alloc_page(void)
{
    for (uint32_t page = MEMORY_START / PAGE_SIZE;
         page < pmm_max_pages;
         page++)
    {
        if (pmm_bitmap[page] == 0)
        {
            pmm_bitmap[page] = 1;

            return page * PAGE_SIZE;
        }
    }

    kpanic("PMM: Out of memory");

    return 0;
}

void pmm_free_page(uint32_t address)
{
    if (address % PAGE_SIZE != 0)
        kpanic("PMM: Unaligned address");

    if (address < MEMORY_START)
        kpanic("PMM: Invalid address");

    uint32_t page = address / PAGE_SIZE;

    if (page >= pmm_max_pages)
        kpanic("PMM: Invalid address");

   
    if (page >= kernel_first_page &&
        page < kernel_last_page)
    {
        kpanic("PMM: Free kernel page");
    }

    uint32_t usable = 0;

    for (uint32_t r = 0; r < region_count; r++)
    {
        uint32_t start_page =
            (region_base[r] + PAGE_SIZE - 1) / PAGE_SIZE;

        uint32_t end_page =
            region_end[r] / PAGE_SIZE;

        if (page >= start_page && page < end_page)
        {
            usable = 1;
            break;
        }
    }

    if (!usable)
        kpanic("PMM: Page is not usable");

    if (pmm_bitmap[page] == 0)
        kpanic("PMM: Double free");

    pmm_bitmap[page] = 0;
}