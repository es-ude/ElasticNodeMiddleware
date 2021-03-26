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

#ifndef LUFA
#ifndef UART
#error "LUFA or UART must be defined for debugging"
#endif
#endif

#ifdef LUFA
#ifdef UART
#error "Only one of LUFA or UART can be defined for debugging"
#endif
#endif


#ifdef LUFA
#include "PeripheralInterface/LufaUsartImpl.h"
#endif

#ifdef UART
#include "src/uart/uart.h"
#include "src/uart/uart_internal.h"
#endif

#include "src/debug/debug.h"

#ifdef TEST

void _delay_ms(uint8_t delay);

#else

#include <util/delay.h>

#endif

#ifdef UART
volatile uint8_t uartData;
volatile int uartFlag = 0x0;
#endif

#ifdef LUFA
void debugTask(void) {
    lufaTask();
}
#endif

#ifdef LUFA
uint16_t debugNumInputAvailable(void) {
    return lufaNumInputAvailable();
}
#endif

#ifdef UART
void debugReceiveCharHandler(uint8_t received) {
    uartData = received;
    uartFlag = 1;
}
#endif

void debugInit(void (*receiveHandler)(uint8_t)) {
#ifdef LUFA
    initLufa();
    while (!lufaOutputAvailable()) {
        _delay_ms(100);
    }
#endif
#ifdef UART
    uartFlag = 0x0;

    uart_Init(&debugReceiveCharHandler);
    debugWriteLine("\r\n\nStarting debug...");
#endif
}

#ifdef UART
void setDebugReceiveHandler(void (*receiveHandler)(uint8_t)) {
    uart_setUartReceiveHandler_internal(receiveHandler);
}
#endif

void debugNewLine(void) {
    debugWriteCharBlock('\r');
    debugWriteCharBlock('\n');
}

void debugWriteBool(uint8_t input) {
    if (input) {
        debugWriteString("true");
    } else {
        debugWriteString("false");
    }
}

void debugWriteLine(char *s) {
    debugWriteString(s);
    debugNewLine();
}

void debugWriteString(char *s) {
#ifdef LUFA
    lufaWriteString(s);
#endif
#ifdef UART
    uart_WriteStringBlock(s);
#endif
}

void debugWriteStringLength(char *s, uint16_t length) {
#ifdef LUFA
    lufaWriteStringLength(s, length);
#endif
#ifdef UART
    uart_WriteStringLengthBlock(s, length);
#endif
}

void debugWriteChar(uint8_t c) {
#ifdef LUFA
    lufaWriteByte(c);
#endif
#ifdef UART
    uart_WriteCharBlock_internal(c);
#endif
}

void debugWriteCharBlock(uint8_t c) {
#ifdef LUFA
    lufaWriteByte(c);
    lufaWaitUntilDone();
#endif
#ifdef UART
    uart_WriteCharBlock_internal(c);
#endif
}

uint8_t debugReadCharAvailable(void) {
#ifdef LUFA
    return lufaReadAvailable();
#endif
#ifdef UART
    return uartFlag;
#endif
}

#ifdef UART
void debugReadCharProcessed(void) {
    uartFlag = 0;
}
#endif

uint8_t debugReadCharBlock(void) {
#ifdef LUFA
    return lufaReadByteBlocking();
#endif
#ifdef UART
    return uart_ReceiveCharBlocking_internal();
#endif
}

uint8_t debugGetChar(void) {
#ifdef LUFA
    return lufaGetChar();
#endif
#ifdef UART
    return uartData;
#endif
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
    sprintf(buf, "%08lX", (unsigned long) num);
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
    sprintf(buf, "%lu", (unsigned long) num);
    debugWriteString(buf);
    free(buf);
}

//signed long
void debugWriteDec32S(int32_t num) {
    char *buf = (char *) malloc(10);
    sprintf(buf, "%ld", (long) num);
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
        if (number & i)
            debugWriteCharBlock('1');
        else
            debugWriteCharBlock('0');
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
#ifdef LUFA
    lufaWaitUntilDone();
#endif
#ifdef UART
    uart_WaitUntilDone();
#endif
}

#ifdef UART
uint8_t debugSending(void) {
    return uart_Sending();
}
#endif

void debugAck(uint8_t c) {
    debugWriteCharBlock(c);
}
