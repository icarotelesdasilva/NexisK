section .text

extern shell_C

global shell

shell:

mov eax, 2
int 0x10

call shell_C
