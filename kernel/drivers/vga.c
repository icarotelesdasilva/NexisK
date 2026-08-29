/*
 * Kernel-Ícaro (vmicaro)
 * Copyright (c) 2026 Ícaro Teles da Silva (@icarotelesdasilva)
 */

char *vga  = (char*)0xB8000;
int coluna = 0;
int linha  = 0;

void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i * 2]     = ' ';    
        vga[i * 2 + 1] = 0x07;   
    }
    coluna = 0;
    linha  = 0;
}

void vga_print(char *str) {
    int i = 0;

    while(str[i] != '\0') {
        if(str[i] == '\n') {
            coluna = 0;
            linha++;
        }
        else {
            int pos = (linha * 80 + coluna) * 2;
            vga[pos]   = str[i];
            vga[pos+1] = 0x03; 
            coluna++;
            
            if(coluna >= 80) {
                coluna = 0;
                linha++;
            }
        }

        if(linha >= 25) {
            vga_clear(); 
        }

        i++;
    }
}
