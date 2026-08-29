extern void vga_clear(void);
extern void vga_print(char *str);

void kmain(void)
{
 
 vga_clear();
 vga_print("Kernel alive.");

    for (;;)
        asm volatile ("hlt");
}