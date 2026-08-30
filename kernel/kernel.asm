
 
 ; Nexis Kernel
 ; Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 ; Author: @icarotelesdasilva
 ; SPDX-License-Identifier: GPL-2.0-only
 

bits 32

global _start
extern kmain

section .text.start

_start:
    call kmain

.hang:
    cli
    hlt
    jmp .hang