#include "../drivers/vga.h"
#include "syscall.h"
#include "../IDT/execption_handler.h"

#include <stdint.h>

static int validate_user_string(const char *str, uint32_t max_len) {
    if (!str) {
        return 0;
    }

    for (uint32_t i = 0; i < max_len; i++) {
        if (str[i] == '\0') {
            return 1;
        }
    }

    return 0;
}

void syscall(struct registers *regs)
{
    uint32_t syscall_num = regs->eax;

    switch (syscall_num) {
        case 0:
            vga_print("\n\nsyscall: Hello from userspace!\n");
            regs->eax = 0;
            break;

        case 1: {
            const char *user_string = (const char *)regs->ebx;

            if (validate_user_string(user_string, 256)) {
                vga_print((char *)user_string);
                regs->eax = 0;
            } else {
                vga_print("[Kernel Error]: Invalid string pointer from user space.\n");
                regs->eax = (uint32_t)-1;
            }
            break;
        }

        case 2:
            vga_print("command list: help");
            regs->eax = 2;
            break;

        default:
            regs->eax = (uint32_t)-1;
            break;
    }
}

