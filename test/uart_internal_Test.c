//
// Created by annika on 09.01.20.
//

#include "unity.h"
#include "lib/uart/uart_internal.h"
#include "lib/uart/circularBuffer/MockcircularBuffer.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

circularBuffer sendingBuf;
void (*uartReceiveHandler)(uint8_t);
uint8_t sendingData;
uint8_t sendingFlag;

uint8_t udr1;
uint8_t* UDR1 = &udr1;
uint8_t ucsr1a;
uint8_t* UCSR1A = &ucsr1a;

void initalise_uart_internal_MockRegister(void) {
    UDR1 = &udr1;
    UCSR1A = &ucsr1a;
}
void test_uart_BufferCount(void) {
    initalise_uart_internal_MockRegister();

    circularBuffer_Count_ExpectAndReturn(&sendingBuf, 23);
    uart_BufferCount_internal();
}

void dummyFunction(uint8_t dummy){}

void test_uart_setUartReceiveHandler_internal(void) {
    initalise_uart_internal_MockRegister();

    void (*receiveHandler)(uint8_t) = &dummyFunction;
    uart_setUartReceiveHandler_internal(receiveHandler);
    TEST_ASSERT_EQUAL(uartReceiveHandler, receiveHandler);
}

void test_uart_WriteNext_internal(void) {
    initalise_uart_internal_MockRegister();

    if(!sendingFlag) {
        BitManipulation_bitIsSetOnArray_ExpectAndReturn(UCSR1A, UDRE1, 0);
        BitManipulation_bitIsSetOnArray_ExpectAndReturn(UCSR1A, UDRE1, 1);
        circularBuffer_Pop_ExpectAndReturn(&sendingBuf, &sendingData, 1);
    }
    uart_WriteNext_internal();

    TEST_ASSERT_EQUAL(UDR1, sendingData);
    TEST_ASSERT_EQUAL_UINT8(sendingFlag, 0x1);

}