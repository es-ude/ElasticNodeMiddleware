/*****
 * Software Half-duplex UART Driver
 * Henry Chan (hc352 @cornell.edu)
 * Based on AVR304
 *
 * Sample Usage:
 * RECEIVING
 * sw_uart_receive_byte();
 * // Other instructions while receiving
 * if (sw_uart_state == IDLE) {
 *    x = sw_uart_rxdata;
 * }
 *
 * TRANSMITTING
 * sw_uart_send_byte(c);
 * // Other instructions while transmitting
 * if (sw_uart_state == IDLE) {
 *    //Stuff when transmission done
 * }
 ****/

#include "EmbeddedUtilities/BitManipulation.h"

#include "src/uart/uart.h"

#include "src/uart/uart_internal.h"
#include "src/uart/circularBuffer/circularBuffer.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/interruptManager.h"
#include "src/delay/delay.h"

circularBuffer sendingBuf;

void (*uartReceiveHandler)(uint8_t);

uint8_t sendingFlag;
uint8_t sendingData;
volatile uint8_t receivedData;

void uart_WaitUntilDone(void) {
    while (uart_BufferCount_internal() > 0) {
        _delay_ms(10);
    }
}

void uart_Init(void (*receiveHandler)(uint8_t)) {
    UBRR1H = (uint8_t)(my_bdr >> 8);
    UBRR1L = (uint8_t)(my_bdr);

#if UART_2X
    BitManipulation_setBit(UART_UCSR1A, U2X1);
    //UCSR1A |= (1 << U2X1);
#endif
    BitManipulation_setBit(UART_UCSR1B, RXEN1);
    BitManipulation_setBit(UART_UCSR1B, TXEN1);
    BitManipulation_setBit(UART_UCSR1B, RXCIE1);
    BitManipulation_setBit(UART_UCSR1B, TXCIE1);

    BitManipulation_setBit(UART_UCSR1C, USBS1);
    BitManipulation_setBit(UART_UCSR1C, UCSZ10);
    //UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1) | (1 << TXCIE1);
    //UCSR1C = (1 << USBS1) | (3 << UCSZ10);

    uart_setUartReceiveHandler_internal(receiveHandler);
    sendingFlag = 0x0;
    circularBuffer_Init(&sendingBuf, UART_SENDING_BUFFER);
    interruptManager_setInterrupt();
}

void *uart_getUartReceiveHandler(void) {
    return uartReceiveHandler;
}

uint8_t uart_Sending(void) {
    return sendingFlag;
}

void uart_WriteString(char *s) {
    while (*s != 0) {
        uart_Queue_internal(*(s++));
    }
    uart_WriteNext_internal();
}

void uart_WriteStringBlock(char *s) {
    while (*s != 0) {
        uart_WriteCharBlock_internal(*(s++));
    }
}

void uart_WriteStringLength(char *s, uint16_t length) {
    char *ptr = s;
    for (uint16_t i = 0; i < length; i++)
        uart_Queue_internal(*(ptr++));
    uart_WriteNext_internal();
}

void uart_WriteStringLengthBlock(char *s, uint16_t length) {
    char *ptr = s;
    for (uint16_t i = 0; i < length; i++) {
        uart_WriteCharBlock_internal(*(ptr++));
    }
}

void uart_ReceiveUint32Blocking(uint32_t *output) {
    interruptManager_clearInterrupt();
    uint8_t *outputPtr = (uint8_t *) output;

    *outputPtr++ = uart_ReceiveCharBlocking_internal();
    *outputPtr++ = uart_ReceiveCharBlocking_internal();
    *outputPtr++ = uart_ReceiveCharBlocking_internal();
    *outputPtr++ = uart_ReceiveCharBlocking_internal();

    // return output;
    interruptManager_setInterrupt();
}

void uart_WriteChar(uint8_t c) {
    if (circularBuffer_Push(&sendingBuf, c)) {
        // check if sending already
        if (!sendingFlag) {
            // start process
            uart_WriteNext_internal();

        }
        sendingFlag = 0x1;
    }
}

void uart_ISR_Receive() {
    interruptManager_clearInterrupt();
    receivedData = UDR1;
    if (uartReceiveHandler != NULL) {
        uartReceiveHandler(receivedData);
    }
    interruptManager_setInterrupt();
}

void uart_ISR_Transmit() {
    interruptManager_clearInterrupt();
    // check if more data to send
    if (circularBuffer_Pop(&sendingBuf, &sendingData)) {
        // if avail, send it
        UDR1 = sendingData;
    } else {
        sendingFlag = 0x0;
    }
    interruptManager_setInterrupt();
}

/*
void uart_WriteBin(uint32_t num, uint8_t length) {
    uart_WriteString("0b");
    uint32_t number = num;
    for (uint8_t i = length - 1; i >= 0; i--) {
        if (number & (1 << i)) {
            uart_WriteChar('1');
        } else {
            uart_WriteChar('0');
            // number >>= 1;
        }
    }
}

void uart_NewLine(void) {
    uart_WriteChar('\r');
    uart_WriteChar('\n');
}

void uart_WriteBin4(uint8_t num) {
    uart_WriteBin(num, 4);
}

void uart_WriteBin8(uint8_t num) {
    uart_WriteBin(num, 8);
}

void uart_WriteBin32(uint32_t num) {
    uart_WriteBin(num, 32);
}

void uart_WriteDec8(uint8_t num) {
    char buf[4];
    sprintf(buf, "%d", num);
    uart_WriteString(buf);
}

void uart_WriteDec16(uint16_t num) {
    char buf[20];
    sprintf(buf, "%u", num);
    uart_WriteString(buf);
}

void uart_WriteDec32(uint32_t num) {
    char buf[20];
    sprintf(buf, "%lu", num);
    uart_WriteString(buf);
}

void uart_WriteDec32S(int32_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%ld", num);
    uart_WriteString(buf);
    free(buf);
}

void uart_WriteHex8(uint8_t num) {
    char buf[3];
    sprintf(buf, "%02X", num);
    uart_WriteString(buf);
}

void uart_WriteHex16(uint16_t num) {
    char buf[10];
    sprintf(buf, "%04X", num);
    uart_WriteString(buf);
}

void uart_WriteHex32(uint32_t num) {
    char buf[10];
    sprintf(buf, "%08lX", num);
    uart_WriteString(buf);
}

void uart_WriteFloat(float num) {
    char buf[10];
    sprintf(buf, "%.2f", num);
    uart_WriteString(buf);
}

void uart_Ack(uint8_t c) {
    uart_WriteChar(c);
}
 */