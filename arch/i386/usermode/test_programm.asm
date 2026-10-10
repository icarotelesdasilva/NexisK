global user_main

user_main:
  mov eax, 0 
  int 34 
  mov eax, 1    
  int 34
.loop:

jmp .loop
