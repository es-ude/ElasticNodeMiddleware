#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/spi/spi_internal.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/MockinterruptManager.h"
#include "src/spi/Mockspi.h"

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

uint8_t data;

void (*readingCallback)(uint8_t, uint8_t);

void (*finishedCallback)(void);

void test_spi_init(void) {
    SPCR_SPI = &spcr_spi;
    SPSR_SPI = &spsr_spi;
    PORTB_SPI = &portb_spi;
    DDR_SPI = &ddr_spi;
    PORTE_SPI = &porte_spi;
    DDRE_SPI = &ddre_spi;
    SPDR_SPI = &spdr_spi;
}

void test_spiEnable_internal(void) {
    test_spi_init();

    BitManipulation_setBit_Expect(DDR_SPI, P_FPGA_POWER_INT);
    BitManipulation_setBit_Expect(PORTB_SPI, P_FPGA_POWER_INT);

    interruptManager_clearInterrupt_Expect();
    BitManipulation_setBit_Expect(SPCR_SPI, SPE);
    BitManipulation_setBit_Expect(SPCR_SPI, MSTR);
    BitManipulation_setBit_Expect(SPCR_SPI, SPR0);

    spiEnable_internal();
}

void test_flashResetCallbacks_internal(void) {
    flashResetCallbacks_internal();

    TEST_ASSERT_EQUAL(NULL, readingCallback);
    TEST_ASSERT_EQUAL(NULL, finishedCallback);
}

void test_deselectWireless_internal(void) {
    BitManipulation_setBit_Expect(DDRE_SPI, P_WIRELESS_CS);
    BitManipulation_setBit_Expect(PORTE_SPI, P_WIRELESS_CS);

    deselectWireless_internal();
}

void test_SPI_internal(void) {
    // TODO: test while?
    interruptManager_clearInterrupt_Expect();

    TEST_ASSERT_EQUAL(data, SPI_internal(data));
    TEST_ASSERT_EQUAL(*SPSR_SPI, data);
}

// TODO: call internal functions
void test_spiPerformTaskBlockingWithCallback_internal(void) {
    uint16_t numWrite = 0;
    uint8_t *dataWrite = 0;
    uint16_t numRead = 0;

    spiPerformTaskBlockingWithCallback_internal(numWrite, dataWrite, numRead, readingCallback, finishedCallback);
}
