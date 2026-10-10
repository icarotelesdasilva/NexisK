 bits 32

section .multiboot

align 4

dd 0x1BADB002
dd 0x00000003

dd -(0x1BADB002 + 0x00000003)

section .bss 

align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _start
extern kmain

_start:
cld
mov esp, stack_top
xor ebp, ebp
push ebx
push eax
cmp eax, 0x2BADB002
jne .hang
cli
call kmain
add esp, 8

.hang:


mov word [0xb8000], 0x4F46

.loop:

cli
hlt
jmp .loop
