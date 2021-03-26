#include "unity.h"

#include "test/header_replacements/PeripheralInterface/MockLufaUsartImpl.h"

#include "src/debug/debug.h"

#include "src/delay/Mockdelay.h"

void test_debugTask(void) {
    lufaTask_Expect();

    debugTask();
}

void test_debugNumInputAvailable(void) {
    lufaNumInputAvailable_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugNumInputAvailable());
}

void test_debugInit(void) {
    initLufa_Expect();
    lufaOutputAvailable_ExpectAndReturn(0);
    _delay_ms_Expect(100);
    lufaOutputAvailable_ExpectAndReturn(1);
    debugInit(NULL);
}

void test_debugNewLine(void) {
    lufaWriteByte_Expect('\r');
    lufaWaitUntilDone_Expect();
    lufaWriteByte_Expect('\n');
    lufaWaitUntilDone_Expect();

    debugNewLine();
}

void test_debugWriteBool_true(void) {
    lufaWriteString_Expect("true");
    debugWriteBool(1);
}

void test_debugWriteBool_false(void) {
    lufaWriteString_Expect("false");
    debugWriteBool(0);
}

void test_debugWriteLine(void) {
    char *s = "TestString";

    lufaWriteString_Expect(s);

    lufaWriteByte_Expect('\r');
    lufaWaitUntilDone_Expect();
    lufaWriteByte_Expect('\n');
    lufaWaitUntilDone_Expect();

    debugWriteLine(s);
}

void test_debugWriteString(void) {
    char *s = "TestString";

    lufaWriteString_Expect(s);

    debugWriteString(s);
}

void test_debugWriteStringLength(void) {
    char *s = "TestString";
    uint16_t length = 10;

    lufaWriteStringLength_Expect(s, length);

    debugWriteStringLength(s, length);
}

void test_debugWriteChar(void) {
    char s = 'T';

    lufaWriteByte_Expect(s);

    debugWriteChar(s);
}

void test_debugWriteCharBlock(void) {
    char s = 'T';

    lufaWriteByte_Expect(s);
    lufaWaitUntilDone_Expect();

    debugWriteCharBlock(s);
}

void test_debugReadCharAvailable(void) {
    lufaReadAvailable_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugReadCharAvailable());
}

void test_debugReadCharBlock(void) {
    lufaReadByteBlocking_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugReadCharBlock());
}

void test_debugGetChar(void) {
    lufaGetChar_ExpectAndReturn(0);

    TEST_ASSERT_EQUAL(0, debugGetChar());
}

void test_debugWriteHex8(void) {
    uint8_t num = 0x12;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%02X", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteHex8(num);
}

void test_debugWriteHex16(void) {
    uint16_t num = 0x1234;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%04X", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteHex16(num);
}

void test_debugWriteHex32(void) {
    uint16_t num = 0x1234;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%08lX", (unsigned long) num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteHex32(num);
}

void test_debugWriteDec8(void) {
    uint8_t num = 12;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%2d", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteDec8(num);
}

void test_debugWriteDec16(void) {
    uint16_t num = 60350;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%u", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteDec16(num);
}

void test_debugWriteDec32(void) {
    uint32_t num = 603566660;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%lu", (unsigned long) num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteDec32(num);
}

void test_debugWriteDec32S(void) {
    uint32_t num = 603566660;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%ld", (long) num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteDec32S(num);
}

void test_debugWriteBin4(void) {
    uint8_t num = 0x1;

    lufaWriteString_Expect("0b");
    uint32_t i = (uint32_t) 1 << (4 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i) {
            lufaWriteByte_Expect('1');
            lufaWaitUntilDone_Expect();
        } else {
            lufaWriteByte_Expect('0');
            lufaWaitUntilDone_Expect();
        }
    }

    debugWriteBin4(num);
}

void test_debugWriteBin8(void) {
    uint8_t num = 0x12;

    lufaWriteString_Expect("0b");
    uint32_t i = (uint32_t) 1 << (8 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i) {
            lufaWriteByte_Expect('1');
            lufaWaitUntilDone_Expect();
        } else {
            lufaWriteByte_Expect('0');
            lufaWaitUntilDone_Expect();
        }
    }

    debugWriteBin8(num);
}

void test_debugWriteBin32(void) {
    uint32_t num = 0x12345678;

    lufaWriteString_Expect("0b");
    uint32_t i = (uint32_t) 1 << (32 - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i) {
            lufaWriteByte_Expect('1');
            lufaWaitUntilDone_Expect();
        } else {
            lufaWriteByte_Expect('0');
            lufaWaitUntilDone_Expect();
        }
    }

    debugWriteBin32(num);
}

void test_debugWriteBin(void) {
    uint32_t num = 0x1234;
    uint8_t length = 4;

    lufaWriteString_Expect("0b");
    uint32_t i = (uint32_t) 1 << (length - 1);
    uint32_t number = num;
    for (; i; i >>= 1) {
        if (number & i) {
            lufaWriteByte_Expect('1');
            lufaWaitUntilDone_Expect();
        } else {
            lufaWriteByte_Expect('0');
            lufaWaitUntilDone_Expect();
        }
    }

    debugWriteBin(num, length);
}

void test_debugWriteFloat(void) {
    float num = 1.23;

    char *buf = (char *) malloc(10);
    sprintf(buf, "%.2f", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteFloat(num);
}

void test_debugWriteFloatFull(void) {
    float num = 1.2345;

    char *buf = (char *) malloc(100);
    sprintf(buf, "%f", num);
    lufaWriteString_Expect(buf);
    free(buf);

    debugWriteFloatFull(num);
}

void test_debugDone(void) {
    lufaWriteString_Expect("\n$$");

    lufaWriteByte_Expect('\r');
    lufaWaitUntilDone_Expect();
    lufaWriteByte_Expect('\n');
    lufaWaitUntilDone_Expect();

    debugDone();
}

void test_debugReady(void) {
    lufaWriteString_Expect("\n%%");

    lufaWriteByte_Expect('\r');
    lufaWaitUntilDone_Expect();
    lufaWriteByte_Expect('\n');
    lufaWaitUntilDone_Expect();

    debugReady();
}

void test_debugWaitUntilDone(void) {
    lufaWaitUntilDone_Expect();
    debugWaitUntilDone();
}

void test_debugAck(void) {
    char c = 'f';

    lufaWriteByte_Expect(c);
    lufaWaitUntilDone_Expect();

    debugAck(c);
}
