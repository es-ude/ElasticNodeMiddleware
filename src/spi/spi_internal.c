#include "EmbeddedUtilities/BitManipulation.h"

#include "src/spi/spi.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/interruptManager.h"

// this function is called when a byte is read from flash
// parameters are data and whether this is last byte
void (*readingCallback)(uint8_t, uint8_t) = NULL;

void (*finishedCallback)(void) = NULL;

void spiEnable_internal(void) {

    // ensure master mode (_SS)
    BitManipulation_setBit(DDR_SPI, P_FPGA_POWER_INT);
    BitManipulation_setBit(PORTB_SPI, P_FPGA_POWER_INT);
    //DDRB |= _BV(PB0);
    //PORTB |= _BV(PB0);

    interruptManager_clearInterrupt();
    BitManipulation_setBit(SPCR_SPI, SPE);
    BitManipulation_setBit(SPCR_SPI, MSTR);
    BitManipulation_setBit(SPCR_SPI, SPR0);
    //SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);

    // TODO: how to use in tests
    // reset interrupts
    while (*SPSR_SPI & (uint8_t) (1 << (SPIF))) {
        //while (SPSR & _BV(SPIF)) {
        //    SPDR;
    }
    //SPDR;
}

void spiPerformTaskBlocking_internal(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead, uint8_t *dataRead) {
    uint8_t *ptr;

    ptr = dataWrite;
    for (uint32_t i = 0; i < numWrite; i++) {
        spiWrite(*ptr++);
    }
    ptr = dataRead;
    for (uint32_t i = 0; i < numRead; i++) {
        *ptr++ = spiRead();
    }
}

void flashResetCallbacks_internal(void) {
    readingCallback = NULL;
    finishedCallback = NULL;
}

void deselectWireless_internal(void) {
    BitManipulation_setBit(DDRE_SPI, P_WIRELESS_CS);
    BitManipulation_setBit(PORTE_SPI, P_WIRELESS_CS);
    //DDR_WIRELESS_CS |= _BV(P_WIRELESS_CS);
    //PORT_WIRELESS_CS |= _BV(P_WIRELESS_CS);
}

uint8_t SPI_internal(uint8_t data) {
    interruptManager_clearInterrupt();
    *SPDR_SPI = data;
#ifndef TEST
    while (!(*SPSR_SPI & (1 << (SPIF)))) {}
        //while (!(SPSR & _BV(SPIF))) {
#endif
    return *SPDR_SPI;
}

void spiPerformTaskBlockingWithCallback_internal(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead,
                                                 void (*readingCallback)(uint8_t, uint8_t),
                                                 void (*finishedCallback)(void)) {
    uint8_t *ptr;

    ptr = dataWrite;
    for (uint32_t i = 0; i < numWrite; i++) {
        spiWrite(*ptr++);
    }

    for (uint32_t i = 0; i < numRead; i++) {
        readingCallback(spiRead(), i == numRead - 1);
    }

    if (finishedCallback) {
        finishedCallback();
    }
}
