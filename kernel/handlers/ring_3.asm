[BITS 32]

section .text         
global ring3_test

ring3_test:
    mov eax, 1          
    mov ebx, msg_teste  
    mov ecx, 29         
    
    int 0x80           

    cmp eax, 0
    jne .erro_syscall

.loop:
    jmp .loop

.erro_syscall:
    jmp .loop

section .data
msg_teste: db "Hello world from user space!", 0x0A, 0
