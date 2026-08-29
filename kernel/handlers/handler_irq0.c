#include "../timer/pit.h"

void irq0_handler(void) {
   
    pit_handler(); 
}