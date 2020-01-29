//
// Created by annika on 08.01.20.
//

#ifndef ELASTICNODEMIDDLEWARE_UART_H
#define ELASTICNODEMIDDLEWARE_UART_H

#include <stdint.h>
#include <stdio.h>

#ifdef TEST
#else

#include <util/delay.h>

#endif

#define UART_SENDING_BUFFER 128

// #define baud 9600 // 250000
#define UART_2X 1

#if UART_2X
#define my_bdr ((F_CPU / 8 / (BAUD)) - 1)
#else
#define my_bdr ((F_CPU / 16 / (BAUD)) - 1)
#endif

void uart_WaitUntilDone(void);

void uart_Init(void (*receiveHandler)(uint8_t));
void* uart_getUartReceiveHandler(void);
uint8_t uart_Sending(void);
void uart_NewLine(void);
void uart_WriteLine(char *s);
void uart_WriteString(char *s);
void uart_WriteStringBlock(char *s);
void uart_WriteStringLength(char *s, uint16_t length);
void uart_WriteStringLengthBlock(char *s, uint16_t length);
void uart_ReceiveUint32Blocking(uint32_t *output);
void uart_WriteChar(uint8_t c);

void uart_WriteBin(uint32_t num, uint8_t length);
void uart_WriteBin4(uint8_t num);
void uart_WriteBin8(uint8_t num);
void uart_WriteBin32(uint32_t num);

void uart_WriteDec8(uint8_t num);
void uart_WriteDec16(uint16_t num);
void uart_WriteDec32(uint32_t num);
void uart_WriteDec32S(int32_t num);

void uart_WriteHex8(uint8_t num);
void uart_WriteHex16(uint16_t num);
void uart_WriteHex32(uint32_t num);

void uart_WriteFloat(float num);
void uart_Ack(uint8_t c);

void uart_ISR_Receive();
void uart_ISR_Transmit();


void uart_WriteNext(void);

#endif //ELASTICNODEMIDDLEWARE_UART_H
