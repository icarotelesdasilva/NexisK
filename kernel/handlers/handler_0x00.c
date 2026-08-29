#include "interrupt.h"

extern void kpanic(char *str);

void isr_handler(registers_t regs) {
    switch (regs.int_no) {
        case 0:
            kpanic("Division by zero");
            break;
        default:
            kpanic("Unhandled interrupt/exception");
            break;
    }
}