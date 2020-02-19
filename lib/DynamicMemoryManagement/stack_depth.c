#include "lib/DynamicMemoryManagement/stack_depth.h"
#ifdef UART_DEBUG
#include <stdio.h>
void printStackPointer(const char* func, int16_t s){
    char buf[50];
    snprintf ( buf, 50, "%s %04x\r\n", func, s );
    // ##### CHANGED #####
    //uart_printstring(buf);
    uart_WriteString(buf);
}
#endif