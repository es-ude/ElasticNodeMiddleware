//
// Created by annika on 02.10.19.
//

#include "unity.h"
#include "elasticnodemiddleware/registerAbstraction.h"

void test_abstraction_setRegisterBitsHigh(void)
{
    //Hexzahl 0x00 bekommt an fünfter Stelle 1 --> 16
    uint8_t myRegister = 0x00;
    uint8_t bitPosition = (1 << 4);
    abstraction_setRegisterBitsHigh(&myRegister, bitPosition);
    TEST_ASSERT_EQUAL_UINT8(16, myRegister);
}

void test_abstraction_setRegisterBitsHighDoesNotAffectOtherBits(void)
{
    uint8_t myRegister = 0x68;
    uint8_t bitPosition = 1;
    //check function
    uint8_t expectedRegisterValue =  myRegister | (1 << bitPosition);
    abstraction_setRegisterBitsHigh(&myRegister, (1 << bitPosition));
    TEST_ASSERT_EQUAL_UINT8(expectedRegisterValue, myRegister);
}


void test_abstraction_setRegisterBitsHighDoesNotAffectMemory(void) {
    //beliebiger 4 Byte großer Speicherraum
    uint32_t myRegsiterAndSurroudingArea = 0x3212fe44;
    //Adresse vom dem Speicherraum +1 = eine Speicheradresse weiter
    //(little endian von hinten, also das zweite Byte von hinten)
    uint8_t* theActualRegister = ((uint8_t*) (&myRegsiterAndSurroudingArea) + 1);
    uint8_t bitPosition = 0;
    // 2. Byte von hinten +1 rechnen
    uint32_t expectedMemory = 0x3212ff44;
    // s.o. 2 Byte von hinten +1 rechnen
    abstraction_setRegisterBitsHigh(theActualRegister, (1 << bitPosition));
    TEST_ASSERT_EQUAL_UINT32(expectedMemory, myRegsiterAndSurroudingArea);
}


void test_abstraction_setRegisterBitsLow(void) {
    uint8_t myRegister = 0xff; //255
    uint8_t bitPosition = (1 << 0);
    abstraction_setRegisterBitsLow(&myRegister, bitPosition);
    TEST_ASSERT_EQUAL_UINT8(myRegister, 0xFE);
}

void test_abstraction_setRegisterBitsLowDoesNotAffectOtherBits(void) {
    uint8_t myRegister = 0x1F;
    uint8_t bitPosition = 2;
    uint8_t expectedRegisterValue = myRegister & ~(1 << bitPosition);
    abstraction_setRegisterBitsLow(&myRegister, (1 << bitPosition));
    TEST_ASSERT_EQUAL_UINT8(expectedRegisterValue, myRegister);
}

void test_abstraction_setRegisterBitsLowDoesNotAffectMemory(void) {
    uint32_t myRegsiterAndSurroudingArea = 0x3212fe33;
    uint8_t* theActualRegister = ((uint8_t*) (&myRegsiterAndSurroudingArea) +1);
    uint8_t bitPosition = 1;
    uint32_t expectedMemory = 0x3212fc33;
    abstraction_setRegisterBitsLow(theActualRegister, (1 << bitPosition));
    TEST_ASSERT_EQUAL_UINT32(expectedMemory, myRegsiterAndSurroudingArea);
}

void test_abstraction_setRegisterToValue(void) {
    uint8_t myRegister = 0xff; //255
    uint8_t valueToSet = 4;
    abstraction_setRegisterToValue(&myRegister, valueToSet);
    TEST_ASSERT_EQUAL_UINT8(myRegister, valueToSet);
}

void test_abstraction_setRegisterToValueDoesNotAffectOtherBits(void) {
    uint8_t myRegister = 0x1F;
    uint8_t valueToSet = 2;
    abstraction_setRegisterToValue(&myRegister, valueToSet);
    TEST_ASSERT_EQUAL_UINT8(valueToSet, myRegister);
}

void test_abstraction_setRegisterToValueDoesNotAffectMemory(void) {
    uint32_t myRegsiterAndSurroudingArea = 0x3212fe33;
    uint8_t* theActualRegister = ((uint8_t*) (&myRegsiterAndSurroudingArea) +1);
    uint8_t valueToSet = 1;
    //an 2. Stelle (also 3 und 4 letztes Byte) wird wert auf 1 gesetzt
    uint32_t expectedMemory = 0x32120133;
    abstraction_setRegisterToValue(theActualRegister, valueToSet);
    TEST_ASSERT_EQUAL_UINT32(expectedMemory, myRegsiterAndSurroudingArea);
}

//nicht gesetzt = 0, gesetzt = 4!
void test_abstraction_isBitSet(void) {
    uint8_t myRegister = 0x1F;
    uint8_t bitToRead = 2;
    uint8_t testBit = abstraction_isBitSet(&myRegister, bitToRead);
    TEST_ASSERT_EQUAL_UINT8(4, testBit);
}

void test_abstraction_isBitSetDoesNotAffectOtherBits(void) {
    uint8_t myRegister = 0x1F;
    uint8_t bitToRead = 2;
    uint8_t expected_value = (myRegister & (1 << bitToRead));
    uint8_t testBit = abstraction_isBitSet(&myRegister, bitToRead);
    TEST_ASSERT_EQUAL_UINT8(expected_value, testBit);
}

void test_abstraction_isBitSetDoesNotAffectMemory(void) {
    uint32_t myRegsiterAndSurroudingArea = 0x3212fe33;
    uint8_t* theActualRegister = ((uint8_t*) (&myRegsiterAndSurroudingArea) +1);
    uint8_t bitToRead = 1;
    uint32_t expectedMemory = 0x3212fe33;
    abstraction_isBitSet(theActualRegister, bitToRead);
    TEST_ASSERT_EQUAL_UINT32(expectedMemory, myRegsiterAndSurroudingArea);
}

void test_abstraction_getBit(void) {
    uint8_t myRegister = 0x34;
    uint8_t bitToRead = 3;
    uint8_t testBit = abstraction_getBit(&myRegister, bitToRead);
    TEST_ASSERT_EQUAL_UINT8(0, testBit);
}

void test_abstraction_getRegisterValue(void) {
    uint8_t myRegister = 0x1F;
    uint8_t registerValue = abstraction_getRegisterValue(&myRegister);
    TEST_ASSERT_EQUAL_UINT8(registerValue, myRegister);
}

void test_abstraction_getRegisterValueDoesNotAffectOtherBits(void) {
    uint8_t myRegister = 0x1F;
    uint8_t expectedRegisterValue = abstraction_getRegisterValue(&myRegister);
    TEST_ASSERT_EQUAL_UINT8(myRegister, expectedRegisterValue);
}

void test_abstraction_getRegisterValueDoesNotAffectMemory(void) {
    uint32_t myRegsiterAndSurroudingArea = 0x3212fe33;
    uint8_t* theActualRegister = ((uint8_t*) (&myRegsiterAndSurroudingArea) +1);
    uint32_t expectedMemory = 0x3212fe33;
    abstraction_getRegisterValue(theActualRegister);
    TEST_ASSERT_EQUAL_UINT32(expectedMemory, myRegsiterAndSurroudingArea);
}