#include "EmbeddedUtilities/BitManipulation.h"

#include "src/spi/spi.h"
#include "src/spi/spi_internal.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/interruptManager/interruptManager.h"
#include "src/xmem/xmem.h"

void spiInit(void) {
    interruptManager_clearInterrupt();

    spiEnable_internal();

    flashResetCallbacks_internal();

    interruptManager_setInterrupt();
}

void spiDisable(void) {
    BitManipulation_clearBit(SPCR_SPI, SPE);
    //SPCR &= ~_BV(SPE);
}

void spiPerformSimpleTaskBlocking(uint8_t command, uint32_t numRead, uint8_t *dataRead) {
    spiPerformTaskBlocking_internal(1, &command, numRead, dataRead);
}


uint8_t spiRead() {
    return SPI_internal(0x00);
}

void spiWrite(uint8_t data) {
    SPI_internal(data);
}

void selectFlash(uint8_t mcuFlash) {
    if (mcuFlash) {
        deselectWireless_internal();
        spiEnable_internal();
        xmem_disableXmem();

        // many copies of this
        BitManipulation_setBit(DDR_SPI, P_FLASH_CS);
        //DDR_FLASH_CS |= _BV(P_FLASH_CS);
        //PORT_FLASH_CS &= ~_BV(P_FLASH_CS);

    } else {
        spiEnable_internal();
        xmem_disableXmem();

        //PORT_FLASH_CS &= ~_BV(P_FLASH_CS);
    }
    BitManipulation_clearBit(PORTB_SPI, P_FLASH_CS);

}

void deselectFlash(uint8_t mcuFlash) {
    if (mcuFlash) {
        spiEnable_internal();
        xmem_disableXmem();

        BitManipulation_setBit(DDR_SPI, P_FLASH_CS);
        //DDR_FLASH_CS |= _BV(P_FLASH_CS);
        //PORT_FLASH_CS |= _BV(P_FLASH_CS);
    } else {
        spiEnable_internal();
        xmem_disableXmem();

        //PORT_FLASH_CS |= _BV(P_FLASH_CS);
    }
    BitManipulation_setBit(PORTB_SPI, P_FLASH_CS);
}

/*void fpgaFlashPerformTaskWithCallback(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead,
                                      void (*readingCallback)(uint8_t, uint8_t), void (*finishedCallback)(void)) {
    spiPerformTaskBlockingWithCallback_internal(numWrite, dataWrite, numRead, readingCallback, finishedCallback);
}
*/
