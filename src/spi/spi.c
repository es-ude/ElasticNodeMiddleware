#include "src/spi/spi.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/interruptManager.h"
#include "src/xmem/xmem.h"

// this function is called when a byte is read from flash
// parameters are data and whether this is last byte
void (*readingCallback)(uint8_t, uint8_t) = NULL;

void (*finishedCallback)(void) = NULL;

void spiInit(void) {
    interruptManager_clearInterrupt();

    spiEnable();

    flashResetCallbacks();

    interruptManager_setInterrupt();
}

void spiEnable(void) {

    // ensure master mode (_SS)
    DDRB |= _BV(PB0);
    PORTB |= _BV(PB0);

    interruptManager_clearInterrupt();
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);

    // reset interrupts
    while (SPSR & _BV(SPIF)) {
        SPDR;
    }
    SPDR;
}

void spiDisable(void) {
    SPCR &= ~_BV(SPE);
}

void flashResetCallbacks(void) {
    readingCallback = NULL;
    finishedCallback = NULL;
}

void spiPerformTaskBlocking(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead, uint8_t *dataRead) {
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

void spiPerformSimpleTaskBlocking(uint8_t command, uint32_t numRead, uint8_t *dataRead) {
    spiPerformTaskBlocking(1, &command, numRead, dataRead);
}


uint8_t spiRead() {
    return SPI(0x00);
}

void spiWrite(uint8_t data) {
    SPI(data);
}

uint8_t SPI(uint8_t data) {
    interruptManager_clearInterrupt();
    SPDR = data;
    while (!(SPSR & _BV(SPIF))) {
    }

    return SPDR;
}

//in alwyns code in spiarbritration

void selectFlash(uint8_t mcuFlash) {
    if (mcuFlash) {
        deselectWireless();
        spiEnable();
        xmem_disableXmem();

        // many copies of this
        DDR_FLASH_CS |= _BV(P_FLASH_CS);
        PORT_FLASH_CS &= ~_BV(P_FLASH_CS);
    } else {
        spiEnable();
        xmem_disableXmem();

        PORT_FLASH_CS &= ~_BV(P_FLASH_CS);
    }

}

// placeholder function
void deselectWireless(void) {
    DDR_WIRELESS_CS |= _BV(P_WIRELESS_CS);
    PORT_WIRELESS_CS |= _BV(P_WIRELESS_CS);
}

void deselectFlash(uint8_t mcuFlash) {
    if (mcuFlash) {
        spiEnable();
        xmem_disableXmem();

        DDR_FLASH_CS |= _BV(P_FLASH_CS);
        PORT_FLASH_CS |= _BV(P_FLASH_CS);
    } else {
        spiEnable();
        xmem_disableXmem();

        PORT_FLASH_CS |= _BV(P_FLASH_CS);
    }

}

void fpgaFlashPerformTaskWithCallback(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead,
                                      void (*readingCallback)(uint8_t, uint8_t), void (*finishedCallback)(void)) {
    spiPerformTaskBlockingWithCallback(numWrite, dataWrite, numRead, readingCallback, finishedCallback);
}

void spiPerformTaskBlockingWithCallback(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead,
                                        void (*readingCallback)(uint8_t, uint8_t), void (*finishedCallback)(void)) {
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
