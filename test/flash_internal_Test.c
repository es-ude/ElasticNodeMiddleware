#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/flash/flash_internal.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/spi/Mockspi.h"
#include "src/spi/Mockspi_internal.h"

uint8_t ddrb_flash;
uint8_t portb_flash;
uint8_t spcr_flash;

uint8_t *DDRB_FLASH = &ddrb_flash;
uint8_t *PORTB_FLASH = &portb_flash;
uint8_t *SPCR_FLASH = &spcr_flash;

void test_flashEnableInterface_internal(void) {
    BitManipulation_setBit_Expect(DDRB_FLASH, P_FLASH_CS);
    BitManipulation_setBit_Expect(DDRB_FLASH, P_FLASH_MOSI);
    BitManipulation_setBit_Expect(DDRB_FLASH, P_FLASH_SCK);
    BitManipulation_setBit_Expect(PORTB_FLASH, P_FLASH_CS);

    flashEnableInterface_internal();
}

void test_flashSetSpeed_internal(void) {
    BitManipulation_clearBit_Expect(SPCR_FLASH, SPR0);
    BitManipulation_setBit_Expect(SPCR_FLASH, SPR0);
    flashSetSpeed_internal(FLASH_SPEED_LOW);
}

void test_flashResetQueue_internal(void) {
    // TODO
}

void test_flashResetWriteCount_internal(void) {
    // TODO
}

void test_writeEnableFlash_internal(void) {
    uint8_t mcuFlash = 0;

    flashResetCallbacks_internal_Expect();
    selectFlash_Expect(mcuFlash);
    spiPerformSimpleTaskBlocking_Expect(0x06, 0, NULL);
    deselectFlash_Expect(mcuFlash);

    writeEnableFlash_internal(mcuFlash);
}

void test_readStatus_internal(void) {
    uint8_t mcuFlash = 0;

    flashResetCallbacks_internal_Expect();
    selectFlash_Expect(mcuFlash);
    uint8_t status;
    spiPerformSimpleTaskBlocking_Expect(0x05, 1, &status);
    deselectFlash_Expect(mcuFlash);

    TEST_ASSERT_EQUAL(status, readStatus_internal(mcuFlash));
}

void test_waitDoneFlash_internal(void) {
    // TODO
}
