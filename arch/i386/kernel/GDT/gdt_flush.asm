global setGdt

section .data
align 4
gdtr:
   dw 0 
   dd 0 

section .text
setGdt:
   mov   ax, [esp + 4]
   mov   [gdtr], ax
   
   mov   eax, [esp + 8]
   mov   [gdtr + 2], eax
   
   lgdt  [gdtr]
   
   mov   ax, 0x10
   mov   ds, ax
   mov   es, ax
   mov   fs, ax
   mov   gs, ax
   mov   ss, ax
   
   jmp   0x08:.flush

.flush:
   ret

