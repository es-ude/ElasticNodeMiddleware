#ifndef ELASTICNODEMIDDLEWARE_UART_H
#define ELASTICNODEMIDDLEWARE_UART_H

#include <stdint.h>
#include <stdio.h>

#define UART_SENDING_BUFFER 128

// #define baud 9600 // 250000
#ifndef BAUD
// TODO: Baud rate?
#define BAUD 9600
#endif
#define UART_2X 1

#if UART_2X
#define my_bdr ((F_CPU / 8 / (BAUD)) - 1)
#else
#define my_bdr ((F_CPU / 16 / (BAUD)) - 1)
#endif


#if defined TEST

extern uint8_t* UART_UCSR1A;
extern uint8_t* UART_UCSR1B;
extern uint8_t* UART_UCSR1C;

#else

#define UART_UCSR1A &UCSR1A
#define UART_UCSR1B &UCSR1B
#define UART_UCSR1C &UCSR1C

#endif

void uart_WaitUntilDone(void);

void uart_Init(void (*receiveHandler)(uint8_t));

void *uart_getUartReceiveHandler(void);

uint8_t uart_Sending(void);

void uart_NewLine(void);

void uart_WriteLine(char *s);

void uart_WriteString(char *s);

void uart_WriteStringBlock(char *s);

void uart_WriteStringLength(char *s, uint16_t length);

void uart_WriteStringLengthBlock(char *s, uint16_t length);

void uart_ReceiveUint32Blocking(uint32_t *output);

void uart_WriteChar(uint8_t c);

void uart_ISR_Receive();

void uart_ISR_Transmit();

#endif //ELASTICNODEMIDDLEWARE_UART_H
