#include "EmbeddedUtilities/BitManipulation.h"

#include "src/flash/flash_internal.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/spi/spi.h"
#include "src/spi/spi_internal.h"
#include "src/delay/delay.h"

uint32_t mcuWriteCount = 0;
uint32_t fpgaWriteCount = 0;

volatile uint8_t *flashBufPtr;
uint8_t flashWriteBuf[FLASH_BUFFER_SIZE];
uint16_t flashQueueCount;

void flashEnableInterface_internal(void) {
    BitManipulation_setBit(DDRB_FLASH, P_FLASH_CS);
    BitManipulation_setBit(DDRB_FLASH, P_FLASH_MOSI);
    BitManipulation_setBit(DDRB_FLASH, P_FLASH_SCK);
    //DDR_FLASH_CS |= _BV(P_FLASH_CS) | _BV(P_FLASH_MOSI) | _BV(P_FLASH_SCK);
    BitManipulation_setBit(PORTB_FLASH, P_FLASH_CS);
    //PORT_FLASH_CS |= _BV(P_FLASH_CS);
}

void flashSetSpeed_internal(uint8_t speed) {
    BitManipulation_clearBit(SPCR_FLASH, SPR0);
    //SPCR &= ~_BV(SPR0);
    BitManipulation_setBit(SPCR_FLASH, SPR0);
    /*
    switch (speed) {
        //case FLASH_SPEED_HIGH:
        //break;
        case FLASH_SPEED_LOW:
        default:
            BitManipulation_setBit(SPCR_FLASH, SPR0);
            //SPCR |= _BV(SPR0);  // /16
            break;
    }
    SPDR;
    */
}

void flashResetQueue_internal(void) {
    flashBufPtr = flashWriteBuf;
    flashQueueCount = 0;
    // create header
    *flashBufPtr++ = 0x02;
    // next three are address
    flashBufPtr += 3;
    // data gets buffered from here
}

void flashResetWriteCount_internal(void) {
    mcuWriteCount = 0;
    fpgaWriteCount = 0;
}

void writeEnableFlash_internal(uint8_t mcuFlash) {
    flashResetCallbacks_internal();
    selectFlash(mcuFlash);

    spiPerformSimpleTaskBlocking(0x06, 0, NULL);

    deselectFlash(mcuFlash);
}

// perform RDSR
/* uint8_t readStatus_internal(uint8_t mcuFlash) {
    flashResetCallbacks_internal();

    selectFlash(mcuFlash);
    uint8_t status;

    spiPerformSimpleTaskBlocking(0x05, 1, &status);
    deselectFlash(mcuFlash);

    return status;
}
*/

void waitDoneFlash_internal(uint8_t mcuFlash) {
    uint16_t counter = 0;
    uint8_t status = 0xFF;
#ifndef TEST
    while (status & (1 << (0))) {
#endif
        //while (status & _BV(0)) {
        counter++;
        flashResetCallbacks_internal();
        selectFlash(mcuFlash);
        spiPerformSimpleTaskBlocking(0x05, 1, &status);
        deselectFlash(mcuFlash);

        _delay_ms(5);
#ifndef TEST
    }
#endif
}
