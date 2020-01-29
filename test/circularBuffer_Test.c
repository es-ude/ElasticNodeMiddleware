//
// Created by annika on 08.01.20.
//

#include "unity.h"
#include "lib/uart/circularBuffer/circularBuffer.h"
#include "lib/interruptManager/MockinterruptManager.h"


uint8_t SREG = 0xBF;

void test_circularBuffer_Init(void) {

    circularBuffer sendingBuf;
    circularBuffer *c = &sendingBuf;
    uint16_t size = 64;

    //malloc return NULL, if it fails --> the if statement in the function already checks if malloc is successfully

    circularBuffer_Init(c, size);
    TEST_ASSERT_EQUAL((c->head), (c->buffer));
    TEST_ASSERT_EQUAL((c->tail), (c->buffer));
    TEST_ASSERT_EQUAL((c->last), (c->buffer + size - 1 ));
    TEST_ASSERT_EQUAL_UINT16(c->currentLen, 0);
    TEST_ASSERT_EQUAL_UINT16(c->maxLen, size);
}

void test_circularBuffer_Space(void) {

    circularBuffer sendingBuf;
    circularBuffer *c = &sendingBuf;

    TEST_ASSERT_EQUAL_UINT16((c->maxLen - c->currentLen),(circularBuffer_Space(c)));
}

void test_circularBuffer_Count(void) {

    circularBuffer sendingBuf;
    circularBuffer *c = &sendingBuf;

    TEST_ASSERT_EQUAL_UINT16((c->currentLen), (circularBuffer_Count(c)));
}

void test_circularBuffer_CountObjects(void) {

    circularBuffer sendingBuf;
    circularBuffer *c = &sendingBuf;
    uint16_t size = 64;

    TEST_ASSERT_EQUAL_UINT16((c->currentLen / size), circularBuffer_CountObjects(c, size));
}

void test_circularBuffer_push(void) {

    circularBuffer sendingBuf;
    circularBuffer* c = &sendingBuf;
    uint8_t data = 33;

    uint8_t* checkVarHead = c->head;
    uint16_t checkVarLen = c->currentLen;

    //assumption: SREG & (1<<7) == 1
    interruptManager_clearInterrupt_Expect();
    if (c->currentLen == c->maxLen)
    {
        interruptManager_setInterrupt_Expect();
    }
    interruptManager_setInterrupt_Expect();

    circularBuffer_Push(c, data);

    if(checkVarHead > c->last) {
        TEST_ASSERT_EQUAL(c->head ,c->buffer);
    } else {
        TEST_ASSERT_EQUAL(c->head, checkVarHead++);
    }
    TEST_ASSERT_EQUAL(c->currentLen, checkVarLen+1);

}

void test_circularBuffer_pop(void) {

    circularBuffer sendingBuf;
    circularBuffer *c = &sendingBuf;
    uint8_t dataVar;
    uint8_t* data = &dataVar;

    uint16_t checkVarLen = c->currentLen;
    uint8_t* checkVarTail = c->tail;

    //assumption: SREG & (1<<7) == 1
    interruptManager_clearInterrupt_Expect();
    interruptManager_setInterrupt_Expect();
    circularBuffer_Pop(c, data);

    TEST_ASSERT_EQUAL(*data, *checkVarTail);
    TEST_ASSERT_EQUAL_UINT16(checkVarLen-1, c->currentLen);
    if(c->tail > c->last) {
        TEST_ASSERT_EQUAL(c->tail, c->buffer);
    } else {
        TEST_ASSERT_EQUAL(checkVarTail, c->tail);
    }
}