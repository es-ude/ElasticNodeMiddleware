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

void test_initFlash(void) {
    flashEnableInterface_internal_Expect();
    spiInit_Expect();
    flashSetSpeed_internal_Expect(FLASH_SPEED_LOW);
    // TODO: test
    //spi_buffer[0] = 0x00;
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
    // TODO
}

void test_writeDataFlash(void) {
    // TODO
}

void test_readDataFlash(void) {
    // TODO
}

void test_unlockFlash(void) {
    uint8_t mcuFlash = 0;

    writeEnableFlash_internal_Expect(mcuFlash);
    selectFlash_Expect(mcuFlash);
    spiPerformSimpleTaskBlocking_Expect(0x98, 0, NULL);
    deselectFlash_Expect(mcuFlash);

    unlockFlash(0);
}
