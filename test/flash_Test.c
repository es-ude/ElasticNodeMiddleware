#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/flash/flash.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/flash/Mockflash_internal.h"
#include "src/spi/Mockspi.h"
#include "src/spi/Mockspi_internal.h"

uint8_t ddrb_flash;
uint8_t portb_flash;
uint8_t spcr_flash;

uint8_t *DDRB_FLASH = &ddrb_flash;
uint8_t *PORTB_FLASH = &portb_flash;
uint8_t *SPCR_FLASH = &spcr_flash;

extern uint8_t spi_buffer[SPI_BUFFER_SIZE];

void test_initFlash(void) {
    flashEnableInterface_internal_Expect();
    spiInit_Expect();
    flashSetSpeed_internal_Expect(FLASH_SPEED_LOW);
    TEST_ASSERT_EQUAL(0x00, spi_buffer[0]);
    flashResetQueue_internal_Expect();
    flashResetCallbacks_internal_Expect();
    flashResetWriteCount_internal_Expect();
    deselectFlash_Expect(1);
    deselectFlash_Expect(0);

    initFlash();
}

void test_flashEnableInterface(void) {
    flashEnableInterface_internal_Expect();
    flashEnableInterface();
}

void test_flashDisableInterface(void) {
    spiDisable_Expect();

    BitManipulation_clearBit_Expect(DDRB_FLASH, P_FLASH_CS);
    BitManipulation_clearBit_Expect(DDRB_FLASH, P_FLASH_MOSI);
    BitManipulation_clearBit_Expect(DDRB_FLASH, P_FLASH_SCK);

    BitManipulation_setBit_Expect(PORTB_FLASH, P_FLASH_CS);

    flashDisableInterface();
}

void test_eraseSectorFlash(void) {
    uint32_t address = 0x0;
    uint8_t mcuFlash = 0;

    flashResetCallbacks_internal_Expect();
    writeEnableFlash_internal_Expect(mcuFlash);
    selectFlash_Expect(mcuFlash);
    spiPerformTaskBlocking_internal_Expect(4, spi_buffer, 0, NULL);
    deselectFlash_Expect(mcuFlash);
    waitDoneFlash_internal_Expect(mcuFlash);

    eraseSectorFlash(address, mcuFlash);

    TEST_ASSERT_EQUAL(0x20, spi_buffer[0]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 16), spi_buffer[1]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 8), spi_buffer[2]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 0), spi_buffer[3]);
}

void test_writeDataFlash_if(void) {
    uint32_t address = 0x0;
    uint8_t *data = 0;
    uint16_t length = SPI_BUFFER_SIZE - 3;
    uint8_t mcuFlash = 0;

    writeDataFlash(address, data, length, mcuFlash);
}

void test_writeDataFlash_else(void) {
    uint32_t address = 0x0;
    uint8_t *data = 0;
    uint8_t length = 0x0;
    uint8_t mcuFlash = 0;

    flashResetCallbacks_internal_Expect();
    writeEnableFlash_internal_Expect(mcuFlash);
    selectFlash_Expect(mcuFlash);

    // TODO: memcpy
    // memcpy(spi_buffer + 4, data, length);

    spiPerformTaskBlocking_internal_Expect(4 + length, spi_buffer, 0, NULL);
    deselectFlash_Expect(mcuFlash);
    waitDoneFlash_internal_Expect(mcuFlash);

    writeDataFlash(address, data, length, mcuFlash);

    TEST_ASSERT_EQUAL(0x02, spi_buffer[0]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 16), spi_buffer[1]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 8), spi_buffer[2]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 0), spi_buffer[3]);
}

void readDataFlash_test_spi(uint32_t address) {
    TEST_ASSERT_EQUAL(0x03, spi_buffer[0]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 16), spi_buffer[1]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 8), spi_buffer[2]);
    TEST_ASSERT_EQUAL((uint8_t)(address >> 0), spi_buffer[3]);
}

void test_readDataFlash_if(void) {
    uint32_t address = 0;
    uint32_t numBytes = 0;
    uint8_t mcuFlash = 0;
    void (*readingCallbackFunction)(uint8_t, uint8_t) = NULL;
    void (*finishedCallbackFunction)(void) = 0;

    selectFlash_Expect(mcuFlash);
    spiPerformTaskBlocking_internal_Expect(4, spi_buffer, numBytes, spi_buffer + 4);
    deselectFlash_Expect(mcuFlash);

    TEST_ASSERT_EQUAL(spi_buffer + 4,
                      readDataFlash(address, numBytes, mcuFlash, readingCallbackFunction, finishedCallbackFunction));

    readDataFlash_test_spi(address);
}

void test_readDataFlash_else(void) {
    uint32_t address = 0;
    uint32_t numBytes = 0;
    uint8_t mcuFlash = 1;
    void (*readingCallbackFunction)(uint8_t, uint8_t) = (void (*)(uint8_t, uint8_t)) 1;
    void (*finishedCallbackFunction)(void) = 0;

    selectFlash_Expect(mcuFlash);
    spiPerformTaskBlockingWithCallback_internal_Expect(4, spi_buffer, numBytes, readingCallbackFunction,
                                                       finishedCallbackFunction);

    TEST_ASSERT_EQUAL(spi_buffer + 4,
                      readDataFlash(address, numBytes, mcuFlash, readingCallbackFunction, finishedCallbackFunction));

    readDataFlash_test_spi(address);
}


void test_unlockFlash(void) {
    uint8_t mcuFlash = 0;

    writeEnableFlash_internal_Expect(mcuFlash);
    selectFlash_Expect(mcuFlash);
    spiPerformSimpleTaskBlocking_Expect(0x98, 0, NULL);
    deselectFlash_Expect(mcuFlash);

    unlockFlash(0);
}
