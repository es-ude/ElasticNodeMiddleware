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

#include "PeripheralInterface/LufaUsartImpl.h"

#include "src/debug/debug.h"

#ifdef TEST

void _delay_ms(uint8_t delay);

#else

#include <util/delay.h>

#endif

void debugWriteBin(uint32_t num, uint8_t length);

void debugTask(void) {
    lufaTask();
}

uint16_t debugNumInputAvailable(void) {
    return lufaNumInputAvailable();
}

void debugInit(void (*receiveHandler)(uint8_t)) {
    initLufa();
    while (!lufaOutputAvailable()) {
        _delay_ms(100);
    }
}

void debugNewLine(void) {
    debugWriteCharBlock('\r');
    debugWriteCharBlock('\n');
    debugWaitUntilDone();
}

void debugWriteBool(uint8_t input) {
    if (input)
        debugWriteString("true");
    else
        debugWriteString("false");
}

void debugWriteLine(char *s) {
    debugWriteString(s);
    debugNewLine();
}

void debugWriteString(char *s) {
    lufaWriteString(s);
}

void debugWriteStringLength(char *s, uint16_t length) {
    lufaWriteStringLength(s, length);
}

void debugWriteChar(uint8_t c) {
    lufaWriteByte(c);
}


void debugWriteCharBlock(uint8_t c) {
    lufaWriteByte(c);
    debugWaitUntilDone();
}

uint8_t debugReadCharAvailable(void) {
    return lufaReadAvailable();
}

uint8_t debugReadCharBlock(void) {
    return lufaReadByteBlocking();
}

uint8_t debugGetChar(void) {
    return lufaGetChar();
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
    lufaWaitUntilDone();
}

uint8_t debugSending(void) {
//#warning "lufa not waiting for finished"
    return 0;
}

void debugAck(uint8_t c) {
    debugWriteCharBlock(c);
}

// not needed
void debugReadCharProcessed(void) {}

void setDebugReceiveHandler(void (*receiveHandler)(uint8_t)) {}
