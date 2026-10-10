#include "gdt.h"

uint8_t my_gdt[48] __attribute__((aligned(16)));

extern void setGdt(uint16_t limit, uint32_t base);
extern void write_tss(void *gdt_ptr);
extern void flush_tss(void);

void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    if (source.limit > 0xFFFFF) {
        return;
    }

    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    target[5] = source.access_byte;

    target[6] |= ((source.flags & 0x0F) << 4);
}

void gdt_init(void)
{
    struct GDT null_entry = {0, 0, 0, 0};
    encodeGdtEntry(&my_gdt[0], null_entry);

    struct GDT code_entry = {0x00000000, 0xFFFFF, 0x9A, 0x0C};
    encodeGdtEntry(&my_gdt[8], code_entry);

    struct GDT data_entry = {0x00000000, 0xFFFFF, 0x92, 0x0C};
    encodeGdtEntry(&my_gdt[16], data_entry);

    setGdt((8 * 3) - 1, (uint32_t)&my_gdt);
}

void gdt_user_space_segment(void)
{
    struct GDT user_code = {
        .base = 0x00000000,
        .limit = 0xFFFFF,
        .access_byte = 0xFA,
        .flags = 0x0C
    };
    encodeGdtEntry(&my_gdt[24], user_code);

    struct GDT user_data = {
        .base = 0x00000000,
        .limit = 0xFFFFF,
        .access_byte = 0xF2,
        .flags = 0x0C
    };
    encodeGdtEntry(&my_gdt[32], user_data);

    write_tss(&my_gdt[40]);

    setGdt(sizeof(my_gdt) - 1, (uint32_t)&my_gdt);
    flush_tss();
}

