#ifndef _STACK_DEPTH_H
#include <stdint.h>
// ##### CHANGED #####
//#include "Uart.h"
#include "lib/uart/uart.h"

#ifdef UART_DEBUG
#ifdef __AVR__
#include <avr/io.h>
#endif
#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif
void printStackPointer(const char*, int16_t);
#endif
#endif