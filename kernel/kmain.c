extern void vga_clear(void);
extern void vga_print(char *str);
#include "interrupts/io.h"
#include "interrupts/pic.h"
extern void pic_remap(uint8_t offset1, uint8_t offset2);
extern void kernel_panic(char *str);
void init_pit(uint32_t frequency);
extern void idt_install(void);



void kmain(void)
{

    pic_remap(0x20, 0x28); 
    idt_install();
    init_pit(1000);
   vga_clear();
   vga_print("Kernel alive.");

    for (;;)
        asm volatile ("hlt");
}