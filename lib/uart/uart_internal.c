//
// Created by annika on 09.01.20.
//

#include "lib/uart/uart_internal.h"
#include "lib/uart/circularBuffer/circularBuffer.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "EmbeddedUtilities/BitManipulation.h"

circularBuffer sendingBuf;
void (*uartReceiveHandler)(uint8_t);
uint8_t sendingData;
uint8_t sendingFlag;


uint16_t uart_BufferCount_internal(void) {
    return circularBuffer_Count(&sendingBuf);
}

void uart_setUartReceiveHandler_internal(void (*receiveHandler)(uint8_t)){
    uartReceiveHandler = receiveHandler;
}


void uart_WriteNext_internal(void){
    if(!sendingFlag) {
        /* Wait for empty transmit buffer */
        while(!( BitManipulation_bitIsSetOnArray(UCSR1A, UDRE1))) {}
        circularBuffer_Pop(&sendingBuf, &sendingData);
        UDR1 = sendingData;
        sendingFlag = 0x1;
    }
}