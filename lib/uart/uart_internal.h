//
// Created by annika on 09.01.20.
//

#ifndef ELASTICNODEMIDDLEWARE_UART_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_UART_INTERNAL_H

#include <stdint.h>
#ifdef TEST

#else

#include <util/delay.h>

#endif

#define MAX_QUEUE_RETRIES 10

uint16_t uart_BufferCount_internal(void);
void uart_setUartReceiveHandler_internal(void (*receiveHandler)(uint8_t));
void uart_WriteNext_internal(void);
uint8_t uart_Queue_internal(uint8_t c);
void uart_WriteCharBlock_internal(uint8_t c);

#endif //ELASTICNODEMIDDLEWARE_UART_INTERNAL_H
