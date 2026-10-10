#ifndef SYSCALL_H
#define SYSCALL_H

struct registers;

void syscall(struct registers *regs);

#endif

