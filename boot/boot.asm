; Nexis Kernel
; Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
; Author: @icarotelesdasilva
; SPDX-License-Identifier: GPL-2.0-only

[BITS 16]
[ORG 0x7C00]


KERNEL_SEG          equ 0x1000
KERNEL_OFFSET       equ 0x0000

MEMORY_MAP          equ 0x5000
MEMORY_MAP_COUNT    equ 0x4FF0
MEMORY_MAP_MAX      equ 32

CODE_SEG            equ 0x08
DATA_SEG            equ 0x10

%include "kernel_sectors.inc"



start:
    cli

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl


    call get_memory_map
    jc memory_map_error


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


get_memory_map:
    xor ebx, ebx

    xor ax, ax
    mov [MEMORY_MAP_COUNT], ax

    xor ax, ax
    mov es, ax
    mov di, MEMORY_MAP

.next_entry:

    mov ax, [MEMORY_MAP_COUNT]
    cmp ax, MEMORY_MAP_MAX
    jae .done

    mov eax, 0x0000E820
    mov edx, 0x534D4150
    mov ecx, 24

    push di

    int 0x15

    pop di

    jc .error

    cmp eax, 0x534D4150
    jne .error

    cmp ecx, 20
    jb .error

    inc word [MEMORY_MAP_COUNT]

    add di, 24

    test ebx, ebx
    jz .done

    jmp .next_entry


.done:
    cmp word [MEMORY_MAP_COUNT], 0
    je .error

    clc
    ret


.error:
    stc
    ret


[BITS 32]

init_pm:
    mov ax, DATA_SEG

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x00090000

    jmp 0x00010000



[BITS 16]

memory_map_error:
    xor ax, ax
    mov ds, ax

    mov si, memory_map_error_msg

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

memory_map_error_msg:
    db "E820 ERROR", 0



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

