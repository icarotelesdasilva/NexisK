bits 16
org 0x7C00

KERNEL_SEG      equ 0x1000
KERNEL_OFFSET   equ 0x0000

CODE_SEG        equ 0x08
DATA_SEG        equ 0x10

KERNEL_SECTORS  equ 1

start:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl


    mov ax, KERNEL_SEG
    mov es, ax
    mov bx, KERNEL_OFFSET

    mov ah, 0x02            
    mov al, KERNEL_SECTORS  
    mov ch, 0               
    mov cl, 2               
    mov dh, 0               
    mov dl, [boot_drive]

    int 0x13

    jc disk_error

    cli


    lgdt [gdt_descriptor]


    mov eax, cr0
    or eax, 0x00000001
    mov cr0, eax

    
    jmp CODE_SEG:init_pm


bits 32

init_pm:

    mov ax, DATA_SEG

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    
    mov esp, 0x00090000


    jmp 0x00010000


bits 16

disk_error:
    xor ax, ax
    mov ds, ax

    mov si, disk_error_msg

.print:
    lodsb

    test al, al
    jz .halt

    mov ah, 0x0E
    mov bh, 0x00
    int 0x10

    jmp .print

.halt:
    cli
    hlt
    jmp .halt


boot_drive:
    db 0

disk_error_msg:
    db "BOOT READ DISK ERROR", 0



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



times 510 - ($ - $$) db 0

dw 0xAA55