#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/spi/spi.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

// TODO: own mock?
//#include "src/spi/Mockspi.h"

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

/*
void test_spiInit(void) {

    test_spi_init();

    interruptManager_clearInterrupt_Expect();

    spiEnable_Expect();

    flashResetCallbacks_Expect();

    interruptManager_setInterrupt_Expect();

    spiInit_Expect();

    spiInit();
}
*/

void test_spiEnable(void) {

    test_spi_init();

    BitManipulation_setBit_Expect(DDR_SPI, P_FPGA_POWER_INT);
    BitManipulation_setBit_Expect(PORTB_SPI, P_FPGA_POWER_INT);

    interruptManager_clearInterrupt_Expect();
    BitManipulation_setBit_Expect(SPCR_SPI, SPE);
    BitManipulation_setBit_Expect(SPCR_SPI, MSTR);
    BitManipulation_setBit_Expect(SPCR_SPI, SPR0);

    spiEnable();
}

void test_spiDisable(void) {

    test_spi_init();

    BitManipulation_clearBit_Expect(SPCR_SPI, SPE);

    spiDisable();
}

void test_flashResetCallbacks(void) {


}

void test_spiPerformTaskBlocking(void) {


}

void test_spiRead(void) {


}

void test_SPI(void) {


}

// TODO: use own mock?
/*
void test_selectFlash(void) {

    test_spi_init();

    //deselectWireless_Expect();
    //spiEnable_Expect();
    xmem_disableXmem_Expect();

    // many copies of this
    BitManipulation_setBit_Expect(DDR_SPI, P_FLASH_CS);
    BitManipulation_clearBit_Expect(PORTB_SPI, P_FLASH_CS);
    selectFlash(1);

    //spiEnable_Expect();
    xmem_disableXmem_Expect();
    BitManipulation_clearBit_Expect(PORTB_SPI, P_FLASH_CS);
    selectFlash(0);

}
*/

void test_deselectWireless(void) {

    BitManipulation_setBit_Expect(DDRE_SPI, P_WIRELESS_CS);
    BitManipulation_setBit_Expect(PORTE_SPI, P_WIRELESS_CS);

    deselectWireless();
}

void test_deselectFlash(void) {


}

void test_fpgaFlashPerformTaskWithCallback(void) {


}

void test_spiPerformTaskBlockingWithCallback(void) {


}
