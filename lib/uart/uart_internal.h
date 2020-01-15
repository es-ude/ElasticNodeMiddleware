//
// Created by annika on 09.01.20.
//

#ifndef ELASTICNODEMIDDLEWARE_UART_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_UART_INTERNAL_H

#include <stdint.h>
#ifdef TEST

void _delay_ms(uint8_t delay);

#else

#endif

uint16_t uart_BufferCount_internal(void);
void uart_setUartReceiveHandler_internal(void (*receiveHandler)(uint8_t));
void uart_WriteNext_internal(void);

#endif //ELASTICNODEMIDDLEWARE_UART_INTERNAL_H
