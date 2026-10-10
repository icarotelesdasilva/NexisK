#include "keyboard.h"
#include "../inlines/io.h"
#include <stdint.h>
#include "../drivers/vga.h"
#include "../interrupts/pic.h"



struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};


void keyboard_handler(struct registers* regs) {
 uint8_t scancode = inb(0x60);

    if (!(scancode & 0x80)) {
        char caract = kbd_us[scancode];

        if (caract != 0) {
            char str_temporaria[2] = { caract, '\0' };
            vga_print(str_temporaria);
        }
    }
    pic_send_eoi(regs->int_no);
}

