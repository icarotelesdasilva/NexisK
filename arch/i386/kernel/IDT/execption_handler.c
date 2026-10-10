#include "../drivers/kpanic.h"
#include "../inlines/io.h"
#include <stdint.h>
#include "../drivers/vga.h"
#include "../interrupts/pic.h"
#include "../syscall/syscall.h"
#include "../drivers/keyboard.h"



struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};


extern void kpanic(const char* message);

const char* exception_messages[32] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "SIMD Floating-Point",
    "Virtualization", "Control Protection", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Security Exception", "Reserved"
};

void exception_handler(struct registers* regs) {
    if (regs->int_no < 32) {
        kpanic(exception_messages[regs->int_no]);
    }

    else if (regs->int_no == 33) {
        keyboard_handler(regs);

}

	 else if (regs->int_no == 34) {
       syscall(regs);
}
}
