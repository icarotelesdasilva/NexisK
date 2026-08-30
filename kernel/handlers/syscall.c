#include <stdint.h>

extern void serial_print(const char *str);

typedef struct
{
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_placeholder;

    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} registers_t;


void syscall_1(registers_t *regs)
{
    serial_print("syslog: 1\n");
    regs->eax = 0;
}

void handler_syscall(registers_t *regs)
{
    switch (regs->eax)
    {
        case 1:
            syscall_1(regs);
            break;

        default:
            serial_print("syslog: syscall error\n");
            regs->eax = (uint32_t)-1;
            break;
    }
}
