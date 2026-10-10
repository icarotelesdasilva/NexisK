global jump_usermode
extern user_main

jump_usermode:
mov ax, (4 * 8) | 3 
mov ds, ax
mov es, ax 
mov fs, ax 
mov gs, ax


mov eax, esp
push (4 * 8) | 3 
push eax 
pushf 
push (3 * 8) | 3 
push user_main 
iret
