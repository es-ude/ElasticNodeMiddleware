#include "unity.h"

#include "src/uart/uart.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/uart/circularBuffer/MockcircularBuffer.h"
#include "src/uart/Mockuart_internal.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/MockinterruptManager.h"
#include "src/delay/Mockdelay.h"

circularBuffer sendingBuf;

void (*uartReceiveHandler)(uint8_t);

uint8_t sendingFlag;
volatile uint8_t receivedData;
uint8_t sendingData;

uint8_t ubrr1h;
uint8_t *UBRR1H = &ubrr1h;
uint8_t ubrr1l;
uint8_t *UBRR1L = &ubrr1l;
//for not going into endless while loop
//uint8_t UCSR1A = 0xEF;
uint8_t uart_ucsr1a;
uint8_t *UART_UCSR1A = &uart_ucsr1a;
uint8_t uart_ucsr1b;
uint8_t *UART_UCSR1B = &uart_ucsr1b;
uint8_t uart_ucsr1c;
uint8_t *UART_UCSR1C = &uart_ucsr1c;
uint8_t udr1;
uint8_t *UDR1 = &udr1;

void initalise_uart_mockRegister(void) {
    UBRR1H = &ubrr1h;
    UBRR1L = &ubrr1l;
    UART_UCSR1A = &uart_ucsr1a;
    UART_UCSR1B = &uart_ucsr1b;
    UART_UCSR1C = &uart_ucsr1c;
    UDR1 = &udr1;
}

void dummyFunction(uint8_t dummy) {}

void test_uart_WaitUntilDone(void) {
    initalise_uart_mockRegister();

    //while-call: expected two times
    uart_BufferCount_internal_ExpectAndReturn(1);
    _delay_ms_Expect(10);
    uart_BufferCount_internal_ExpectAndReturn(0);
    uart_WaitUntilDone();
}

void test_uart_init(void) {
    initalise_uart_mockRegister();

    void (*receiveHandler)(uint8_t) = &dummyFunction;

    //UART_2X is 1
    BitManipulation_setBit_Expect(UART_UCSR1A, U2X1);

    BitManipulation_setBit_Expect(UART_UCSR1B, RXEN1);
    BitManipulation_setBit_Expect(UART_UCSR1B, TXEN1);
    BitManipulation_setBit_Expect(UART_UCSR1B, RXCIE1);
    BitManipulation_setBit_Expect(UART_UCSR1B, TXCIE1);

    BitManipulation_setBit_Expect(UART_UCSR1C, USBS1);
    BitManipulation_setBit_Expect(UART_UCSR1C, UCSZ10);

    uart_setUartReceiveHandler_internal_Expect(receiveHandler);
    circularBuffer_Init_Expect(&sendingBuf, UART_SENDING_BUFFER);
    interruptManager_setInterrupt_Expect();

    uart_Init(receiveHandler);

    TEST_ASSERT_EQUAL_UINT8(sendingFlag, 0x0);
    TEST_ASSERT_EQUAL(UBRR1H, (uint8_t)(my_bdr >> 8));
    TEST_ASSERT_EQUAL(UBRR1L, (uint8_t)(my_bdr));
}

void test_uart_getUartReceiveHandler(void) {
    initalise_uart_mockRegister();

    void *ptr = uart_getUartReceiveHandler();
    TEST_ASSERT_EQUAL(ptr, uartReceiveHandler);
}

void test_uart_Sending(void) {
    initalise_uart_mockRegister();

    TEST_ASSERT_EQUAL_UINT8(sendingFlag, uart_Sending());
}

void test_uart_WriteString_EmptyString(void) {
    initalise_uart_mockRegister();
    char *s = "";
    uart_WriteNext_internal_Expect();
    uart_WriteString(s);
}

void test_uart_WriteString(void) {
    initalise_uart_mockRegister();
    char *s = "ai";
    char *dummy = s;
    uart_Queue_internal_ExpectAndReturn(*(dummy++), 1);
    uart_Queue_internal_ExpectAndReturn(*(dummy++), 1);
    uart_WriteNext_internal_Expect();
    uart_WriteString(s);
}

void test_uartWriteStringBlock(void) {
    initalise_uart_mockRegister();
    char *s = "ai";
    char *dummy = s;
    uart_WriteCharBlock_internal_Expect(*(dummy++));
    uart_WriteCharBlock_internal_Expect(*(dummy++));
    uart_WriteStringBlock(s);
}

void test_uart_WriteStringLength(void) {
    initalise_uart_mockRegister();
    char *s = "ai";
    char *dummy = s;
    uint16_t length = 2;
    uart_Queue_internal_ExpectAndReturn(*(dummy++), 1);
    uart_Queue_internal_ExpectAndReturn(*(dummy++), 1);
    uart_WriteNext_internal_Expect();
    uart_WriteStringLength(s, length);
}

void test_uart_WriteStringLengthBlock(void) {
    initalise_uart_mockRegister();
    char *s = "ai";
    char *dummy = s;
    uint16_t length = 2;
    uart_WriteCharBlock_internal_Expect(*(dummy++));
    uart_WriteCharBlock_internal_Expect(*(dummy++));
    uart_WriteStringLengthBlock(s, length);
}

void test_uart_ReceiveUint32Blocking(void) {
    initalise_uart_mockRegister();
    uint32_t output_var;
    uint32_t *output = &output_var;
    interruptManager_clearInterrupt_Expect();
    uart_ReceiveCharBlocking_internal_ExpectAndReturn(1);
    uart_ReceiveCharBlocking_internal_ExpectAndReturn(1);
    uart_ReceiveCharBlocking_internal_ExpectAndReturn(1);
    uart_ReceiveCharBlocking_internal_ExpectAndReturn(1);
    interruptManager_setInterrupt_Expect();
    uart_ReceiveUint32Blocking(output);
}

void test_uart_WriteChar(void) {
    initalise_uart_mockRegister();
    uint8_t c = 5;
    //Push successful
    circularBuffer_Push_ExpectAndReturn(&sendingBuf, c, 1);
    uart_WriteNext_internal_Expect();
    uart_WriteChar(c);

    TEST_ASSERT_EQUAL_UINT8(sendingFlag, 0x1);
}

void test_uart_ISR_Receive() {
    initalise_uart_mockRegister();
    interruptManager_clearInterrupt_Expect();
    interruptManager_setInterrupt_Expect();
    uart_ISR_Receive();

    //here uartReceiveHandler == NULL
    TEST_ASSERT_EQUAL(uart_getUartReceiveHandler(), 0);
}

void test_uart_ISR_Transmit_True() {
    initalise_uart_mockRegister();
    interruptManager_clearInterrupt_Expect();
    circularBuffer_Pop_ExpectAndReturn(&sendingBuf, &sendingData, 1);
    interruptManager_setInterrupt_Expect();
    uart_ISR_Transmit();

    TEST_ASSERT_EQUAL_UINT8(UDR1, sendingData);
}

void test_uart_ISR_Transmit_False() {
    initalise_uart_mockRegister();
    interruptManager_clearInterrupt_Expect();
    circularBuffer_Pop_ExpectAndReturn(&sendingBuf, &sendingData, 0);
    interruptManager_setInterrupt_Expect();
    uart_ISR_Transmit();

    TEST_ASSERT_EQUAL_UINT8(sendingFlag, 0x0);
}
