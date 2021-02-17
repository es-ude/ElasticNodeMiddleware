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

#include "src/debug/debug.h"
#include "src/uart/uart.h"
#include "src/uart/uart_internal.h"

void debugWriteBin(uint32_t num, uint8_t length);

void debugReceiveCharHandler(uint8_t received);

volatile uint8_t uartData;
volatile int uartFlag = 0x0;


void debugReceiveCharHandler(uint8_t received) {
    uartData = received;
    uartFlag = 1;
}

void debugInit(void (*receiveHandler)(uint8_t)) {
    uartFlag = 0x0;

    uart_Init(&debugReceiveCharHandler);
    debugWriteLine("\r\n\nStarting debug...");
}

void setDebugReceiveHandler(void (*receiveHandler)(uint8_t)) {
    uart_setUartReceiveHandler_internal(receiveHandler);
}

void debugNewLine(void) {
    debugWriteCharBlock('\r');
    debugWriteCharBlock('\n');
}

void debugWriteBool(uint8_t input) {
    if (input)
        debugWriteString("true");
    else
        debugWriteString("false");
}

void debugWriteLine(char *s) {
    debugWriteStringBlock(s);
    debugNewLine();
}

void debugWriteString(char *s) {
    uart_WriteStringBlock(s);
}

void debugWriteStringBlock(char *s) {
    uart_WriteStringBlock(s);
}

void debugWriteStringLength(char *s, uint16_t length) {
    uart_WriteStringLengthBlock(s, length);
}

void debugWriteChar(uint8_t c) {
    uart_WriteCharBlock_internal(c);
}


void debugWriteCharBlock(uint8_t c) {
    uart_WriteCharBlock_internal(c);
}

uint8_t debugReadCharAvailable(void) {
    return uartFlag;
}


void debugReadCharProcessed(void) {
    uartFlag = 0;
}

uint8_t debugReadCharBlock(void) {
    return uart_ReceiveCharBlocking_internal();
}

uint8_t debugGetChar(void) {
    return uartData;
}

void debugWriteHex8(uint8_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%02X", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteHex16(uint16_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%04X", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteHex32(uint32_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%08lX", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteDec8(uint8_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%2d", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteDec16(uint16_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%u", num);
    debugWriteString(buf);
    free(buf);
}

//unsigned long
void debugWriteDec32(uint32_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%lu", num);
    debugWriteString(buf);
    free(buf);
}

//signed long
void debugWriteDec32S(int32_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%ld", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteBin4(uint8_t num) {
    debugWriteBin((uint32_t) num, 4);
}

void debugWriteBin8(uint8_t num) {
    debugWriteBin((uint32_t) num, 8);
}

void debugWriteBin32(uint32_t num) {
    debugWriteBin(num, 32);
}

void debugWriteBin(uint32_t num, uint8_t length) {
    debugWriteString("0b");
    uint32_t i = (uint32_t) 1 << (length - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i) {
            debugWriteChar('1');
        } else {
            debugWriteChar('0');
        }
    }
}

void debugWriteFloat(float num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%.2f", num);
    debugWriteString(buf);
    free(buf);
}

void debugWriteFloatFull(float num) {
    char *buf = (char *) malloc(100);
    sprintf(buf, "%f", num);
    debugWriteString(buf);
    free(buf);
}

void debugDone(void) {
    debugWriteLine("\n$$");
}

void debugReady(void) {
    debugWriteLine("\n%%");
}

void debugWaitUntilDone(void) {
    uart_WaitUntilDone();
}

uint8_t debugSending(void) {
    return uart_Sending();
}

void debugAck(uint8_t c) {
    debugWriteChar(c);
}
