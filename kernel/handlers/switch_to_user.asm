[BITS 32]

global switch_to_user_mode
extern ring3_test

USER_DATA_SEL equ 0x23 
USER_CODE_SEL equ 0x1B 

section .text

switch_to_user_mode:
    cli                    

    mov ax, USER_DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push USER_DATA_SEL    
    
    mov eax, esp
    push eax              
    pushf                  
    pop eax
    and eax, ~0x200          
    push eax              

    push USER_CODE_SEL     
    push ring3_test       
    iret
