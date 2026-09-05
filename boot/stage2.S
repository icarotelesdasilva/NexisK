bits 16

global stage2
extern kmain

stage2:
    cli

    mov ax, 0x1000
    mov ds, ax


    mov ah, 0x00
    mov al, 0x03
    int 0x10



    mov si, nexisloader_msg - stage2

.print_loader:
    lodsb
    test al, al
    jz .select_position

    mov ah, 0x0E
    int 0x10

    jmp .print_loader



.select_position:

    mov ax, 0xB800
    mov es, ax

    mov di, 0x0140

    mov si, select_msg - stage2



.print_select:
    lodsb

    test al, al
    jz .wait_key

    mov es:[di], al

    inc di
    mov byte es:[di], 0x01

    inc di

    jmp .print_select



.wait_key:
    mov ah, 0x00
    int 0x16

    cmp al, 0x0D
    jne .wait_key



    mov ah, 0x00
    mov al, 0x03
    int 0x10



    mov si, loading_msg - stage2

.print_loading:
    lodsb

    test al, al
    jz .fim

    mov ah, 0x0E
    int 0x10

    jmp .print_loading



.fim:
    cli
    lgdt [gdt_descriptor - stage2]

    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    jmp dword 0x08:protected_mode


bits 32

protected_mode:
    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x00090000


    call kmain


halt:
    cli
    hlt
    jmp halt



align 4

gdt_start:

gdt_null:
    dq 0x0000000000000000

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start



nexisloader_msg:
    db "NexisLoader Menu:", 0

select_msg:
    db "> NexisK", 0

loading_msg:
    db "Loading NexisK...", 0