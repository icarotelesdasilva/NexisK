[BITS 32]

global ring3_test

ring3_test:
    cli

    mov eax, 1
    int 0x80

.loop:
    jmp .loop