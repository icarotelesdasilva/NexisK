 # Nexis Kernel
 # Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 # Author: @icarotelesdasilva
 # SPDX-License-Identifier: GPL-2.0-only
 

ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    .text :
    {
        *(.text.start)
        *(.text)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        *(COMMON)
        *(.bss*)
    }

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note*)
    }
}