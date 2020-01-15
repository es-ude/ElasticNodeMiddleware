//
// Created by annika on 08.01.20.
//

#include "lib/uart/uart.h"
#include "lib/uart/circularBuffer/circularBuffer.h"
#include "lib/uart/uart_internal.h"
#include "lib/interruptManager/interruptManager.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "EmbeddedUtilities/BitManipulation.h"

circularBuffer sendingBuf;
void (*uartReceiveHandler)(uint8_t);
uint8_t sendingFlag;

void uart_WaitUntilDone(void){
    while (uart_BufferCount_internal() > 0)
    {
        _delay_ms(10);
    }
}

void uart_Init(void (*receiveHandler)(uint8_t)){
    UBRR1H = (uint8_t) (my_bdr >> 8);
    UBRR1L = (uint8_t) (my_bdr);

#if UART_2X
    BitManipulation_setBit(UCSR1A, U2X1);
#endif

    BitManipulation_setBit(UCSR1B, (RXEN1 | TXEN1 | RXCIE1 | TXCIE1));
    BitManipulation_setBit(UCSR1C, (USBS1 | ( 3 << UCSZ10)));
    uart_setUartReceiveHandler_internal(receiveHandler);
    sendingFlag = 0x0;
    circularBuffer_Init(&sendingBuf, UART_SENDING_BUFFER);
    interruptManager_setInterrupt();
}

void* uart_getUartReceiveHandler(void){
    return uartReceiveHandler;
}

uint8_t uart_Sending(void){
    return sendingFlag;
}

void uart_NewLine(void){
    uart_WriteChar('\r');
	uart_WriteChar('\n');
}

void uart_WriteLine(char *s){
    uart_WriteString(s);
    uart_NewLine();
}

void uart_WriteString(char *s){
    while (*s != 0)
    {
        uart_Queue_internal(*(s++));
    }
    uart_WriteNext_internal();
}

void uart_WriteStringBlock(char *s){

}

void uart_WriteStringLength(char *s, uint16_t length){

}

void uart_WriteStringLengthBlock(char *s, uint16_t length){

}

uint8_t uart_ReceiveCharBlocking(void){

}

void uart_ReceiveUint32Blocking(uint32_t *output){

}

void uart_WriteChar(uint8_t c){
    if (circularBuffer_Push(&sendingBuf, c)) {
        // check if sending already
        if (!sendingFlag)
            // start process
            uart_WriteNext_internal();

        sendingFlag = 0x1;
    }
}

void uart_WriteHex8(uint8_t num){

}

void uart_WriteDec8(uint8_t num){

}

void uart_WriteBin8(uint8_t num){

}

void uart_WriteDec16(uint16_t num){

}

void uart_WriteHex16(uint16_t num){

}

void uart_WriteHex32(uint32_t num){

}

void uart_WriteBin32(uint32_t num){

}

void uart_WriteDec32(uint32_t num){

}

void uart_WriteDec32S(int32_t num){

}

void uart_WriteBin4(uint8_t num){

}

void uart_WriteFloat(float num){

}

void uart_Ack(uint8_t c){

}