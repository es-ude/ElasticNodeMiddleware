#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/spi/spi.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/spi/Mockspi_internal.h"
#include "src/interruptManager/MockinterruptManager.h"
#include "src/xmem/Mockxmem.h"

uint8_t spcr_spi;
uint8_t portb_spi;
uint8_t ddr_spi;
uint8_t porte_spi;
uint8_t ddre_spi;
uint8_t spsr_spi;
uint8_t spdr_spi;

uint8_t *SPCR_SPI = &spcr_spi;
uint8_t *PORTB_SPI = &portb_spi;
uint8_t *DDR_SPI = &ddr_spi;
uint8_t *PORTE_SPI = &porte_spi;
uint8_t *DDRE_SPI = &ddre_spi;
uint8_t *SPSR_SPI = &spsr_spi;
uint8_t *SPDR_SPI = &spdr_spi;

void test_spi_init(void) {
    SPCR_SPI = &spcr_spi;
    SPSR_SPI = &spsr_spi;
    PORTB_SPI = &portb_spi;
    DDR_SPI = &ddr_spi;
    PORTE_SPI = &porte_spi;
    DDRE_SPI = &ddre_spi;
    SPDR_SPI = &spdr_spi;
}

void test_spiInit(void) {
    test_spi_init();

    interruptManager_clearInterrupt_Expect();

    spiEnable_internal_Expect();

    flashResetCallbacks_internal_Expect();

    interruptManager_setInterrupt_Expect();

    spiInit();
}

void test_spiDisable(void) {

    test_spi_init();

    BitManipulation_clearBit_Expect(SPCR_SPI, SPE);

    spiDisable();
}

void test_spiPerformTaskBlocking(void) {
    uint8_t command = 0;
    uint32_t numRead = 0;
    uint8_t *dataRead = 0;
    spiPerformTaskBlocking_internal_Expect(1, &command, numRead, dataRead);

    spiPerformSimpleTaskBlocking(command, numRead, dataRead);
}

void test_spiRead(void) {
    SPI_internal_ExpectAndReturn(0, 0);
    TEST_ASSERT_EQUAL(0, spiRead());
}

void test_selectFlash(void) {
    test_spi_init();

    deselectWireless_internal_Expect();
    spiEnable_internal_Expect();
    xmem_disableXmem_Expect();

    // many copies of this
    BitManipulation_setBit_Expect(DDR_SPI, P_FLASH_CS);
    BitManipulation_clearBit_Expect(PORTB_SPI, P_FLASH_CS);
    selectFlash(1);

    spiEnable_internal_Expect();
    xmem_disableXmem_Expect();
    BitManipulation_clearBit_Expect(PORTB_SPI, P_FLASH_CS);
    selectFlash(0);
}


void test_deselectFlash(void) {
    spiEnable_internal_Expect();
    xmem_disableXmem_Expect();
    BitManipulation_setBit_Expect(DDR_SPI, P_FLASH_CS);
    BitManipulation_setBit_Expect(PORTB_SPI, P_FLASH_CS);

    deselectFlash(1);

    spiEnable_internal_Expect();
    xmem_disableXmem_Expect();
    BitManipulation_setBit_Expect(PORTB_SPI, P_FLASH_CS);

    deselectFlash(0);
}

// Not needed anymore
/*
void test_fpgaFlashPerformTaskWithCallback(void) {
    uint16_t numWrite = 0;
    uint8_t *dataWrite = 0;
    uint16_t numRead = 0;
    void (*readingCallback)(uint8_t, uint8_t) = 0;
    void (*finishedCallback)(void) = 0;

    spiPerformTaskBlockingWithCallback_internal_Expect(numWrite, dataWrite, numRead, readingCallback, finishedCallback);

    fpgaFlashPerformTaskWithCallback(numWrite, dataWrite, numRead, readingCallback, finishedCallback);
}
*/
