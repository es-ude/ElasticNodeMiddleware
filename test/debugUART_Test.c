#include "unity.h"

#include "src/debug/debug.h"

#include "src/uart/Mockuart.h"
#include "src/uart/Mockuart_internal.h"

void debugReceiveCharHandler(uint8_t received);

extern volatile int uartFlag;
extern volatile int uartData;

void test_debugInit(void) {

    uart_Init_Expect(&debugReceiveCharHandler);
    uart_WriteStringBlock_Expect("\r\n\nStarting debug...");
    uart_WriteCharBlock_internal_Expect('\r');
    uart_WriteCharBlock_internal_Expect('\n');

    debugInit(NULL);

    TEST_ASSERT_EQUAL(0x0, uartFlag);
}

void test_debugReceiveCharHandler(void) {
    uint8_t received = 34;

    debugReceiveCharHandler(received);

    TEST_ASSERT_EQUAL(received, uartData);
    TEST_ASSERT_EQUAL(1, uartFlag);
}


void test_setDebugReceiveHandler(void) {
    void (*receiveHandler)(uint8_t) = 0x0;
    uart_setUartReceiveHandler_internal_Expect(receiveHandler);

    setDebugReceiveHandler(receiveHandler);
}

void test_debugNewLine(void) {
    uart_WriteCharBlock_internal_Expect('\r');
    uart_WriteCharBlock_internal_Expect('\n');

    debugNewLine();
}

void test_debugWriteBool_true(void) {
    uart_WriteStringBlock_Expect("true");
    debugWriteBool(1);
}

void test_debugWriteBool_false(void) {
    uart_WriteStringBlock_Expect("false");
    debugWriteBool(0);
}

void test_debugWriteLine(void) {
    char *s = "TestString";

    uart_WriteStringBlock_Expect(s);
    uart_WriteCharBlock_internal_Expect('\r');
    uart_WriteCharBlock_internal_Expect('\n');

    debugWriteLine(s);
}

void test_debugWriteString(void) {
    char *s = "TestString";

    uart_WriteStringBlock_Expect(s);

    uart_WriteStringBlock(s);
}

void test_debugWriteStringLength(void) {
    char *s = "TestString";
    uint16_t length = 3;
    uart_WriteStringLengthBlock_Expect(s, length);

    debugWriteStringLength(s, length);
}

void test_debugWriteChar(void) {
    char c = 'A';
    uart_WriteCharBlock_internal_Expect(c);

    debugWriteChar(c);
}

void test_debugWriteCharBlock(void) {
    char c = 'A';

    uart_WriteCharBlock_internal_Expect(c);

    debugWriteCharBlock(c);
}

void test_debugReadCharAvailable(void) {
    TEST_ASSERT_EQUAL(uartFlag, debugReadCharAvailable());
}

void test_debugReadCharProcessed(void) {
    debugReadCharProcessed();

    TEST_ASSERT_EQUAL(0, uartFlag);
}

void test_debugReadCharBlock(void) {
    uart_ReceiveCharBlocking_internal_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugReadCharBlock());
}

void test_debugGetChar(void) {
    TEST_ASSERT_EQUAL(uartData, debugGetChar());
}

void test_debugWriteHex8(void) {
    uint8_t num = 0x12;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%02X", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteHex8(num);
}

void test_debugWriteHex16(void) {
    uint16_t num = 0x1234;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%04X", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteHex16(num);
}

void test_debugWriteHex32(void) {
    uint16_t num = 0x1234;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%08lX", (unsigned long) num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteHex32(num);
}

void test_debugWriteDec8(void) {
    uint8_t num = 12;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%2d", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteDec8(num);
}

void test_debugWriteDec16(void) {
    uint16_t num = 60350;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%u", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteDec16(num);
}

//unsigned long
void test_debugWriteDec32(void) {
    uint32_t num = 603566660;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%lu", (unsigned long) num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteDec32(num);
}

//signed long
void test_debugWriteDec32S(void) {
    uint32_t num = 603566660;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%ld", (long) num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteDec32S(num);
}

void test_debugWriteBin4(void) {
    uint8_t num = 0x1;

    uart_WriteStringBlock_Expect("0b");
    uint32_t i = (uint32_t) 1 << (4 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i)
            uart_WriteCharBlock_internal_Expect('1');
        else
            uart_WriteCharBlock_internal_Expect('0');
    }

    debugWriteBin4(num);
}

void test_debugWriteBin8(void) {
    uint8_t num = 0x12;

    uart_WriteStringBlock_Expect("0b");
    uint32_t i = (uint32_t) 1 << (8 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i)
            uart_WriteCharBlock_internal_Expect('1');
        else
            uart_WriteCharBlock_internal_Expect('0');
    }

    debugWriteBin8(num);
}

void test_debugWriteBin32(void) {
    uint32_t num = 0x12345678;

    uart_WriteStringBlock_Expect("0b");
    uint32_t i = (uint32_t) 1 << (32 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i)
            uart_WriteCharBlock_internal_Expect('1');
        else
            uart_WriteCharBlock_internal_Expect('0');
    }

    debugWriteBin32(num);
}

void test_debugWriteBin(void) {
    uint32_t num = 0x1234;
    uint8_t length = 4;

    uart_WriteStringBlock_Expect("0b");
    uint32_t i = (uint32_t) 1 << (length - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i)
            uart_WriteCharBlock_internal_Expect('1');
        else
            uart_WriteCharBlock_internal_Expect('0');
    }

    debugWriteBin(num, length);
}

void test_debugWriteFloat(void) {
    float num = 1.23;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%.2f", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteFloat(num);
}

void test_debugWriteFloatFull(void) {
    float num = 1.2345;

    char *buf = (char *) malloc(100);
    sprintf(buf, "%f", num);
    uart_WriteStringBlock_Expect(buf);
    free(buf);

    debugWriteFloatFull(num);
}

void test_debugDone(void) {
    uart_WriteStringBlock_Expect("\n$$");
    uart_WriteCharBlock_internal_Expect('\r');
    uart_WriteCharBlock_internal_Expect('\n');

    debugDone();
}

void test_debugReady(void) {
    uart_WriteStringBlock_Expect("\n%%");
    uart_WriteCharBlock_internal_Expect('\r');
    uart_WriteCharBlock_internal_Expect('\n');

    debugReady();
}

void test_debugWaitUntilDone(void) {

    uart_WaitUntilDone_Expect();

    debugWaitUntilDone();
}

void test_debugSending(void) {
    uart_Sending_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugSending());
}

void test_debugAck() {
    uint8_t c = 'A';

    uart_WriteCharBlock_internal_Expect(c);

    debugAck(c);
}
