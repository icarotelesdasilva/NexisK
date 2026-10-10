#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define PACKED __attribute__((packed))

struct GDT {
    uint32_t base;
    uint32_t limit;
    uint8_t  access_byte;
    uint8_t  flags;
};

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} PACKED;

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} PACKED;



void encodeGdtEntry(uint8_t *target, struct GDT source);
void gdt_init(void);
void gdt_user_space_segment(void);

#endif 

