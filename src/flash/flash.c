#include "EmbeddedUtilities/BitManipulation.h"

#include "src/flash/flash.h"
#include "src/flash/flash_internal.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/spi/spi.h"
#include "src/spi/spi_internal.h"

#include "src/delay/delay.h"

#ifndef TEST

#include "src/debug/debug.h"

#endif

uint8_t spi_buffer[SPI_BUFFER_SIZE];

void initFlash(void) {
    flashEnableInterface_internal();

    spiInit();

    flashSetSpeed_internal(FLASH_SPEED_LOW);

    spi_buffer[0] = 0x00;

    flashResetQueue_internal();
    flashResetCallbacks_internal();

    flashResetWriteCount_internal();

    deselectFlash(1);
    deselectFlash(0);
}

void flashEnableInterface(void) {
    flashEnableInterface_internal();
}

void flashDisableInterface(void) {
    spiDisable();
    BitManipulation_clearBit(DDRB_FLASH, P_FLASH_CS);
    BitManipulation_clearBit(DDRB_FLASH, P_FLASH_MOSI);
    BitManipulation_clearBit(DDRB_FLASH, P_FLASH_SCK);
    //DDR_FLASH_CS &= ~(_BV(P_FLASH_CS) | _BV(P_FLASH_MOSI) | _BV(P_FLASH_SCK));
    BitManipulation_setBit(PORTB_FLASH, P_FLASH_CS);
    //PORT_FLASH_CS |= _BV(P_FLASH_CS);
}

void eraseSectorFlash(uint32_t address, uint8_t mcuFlash) {
    flashResetCallbacks_internal();

    writeEnableFlash_internal(mcuFlash);

    selectFlash(mcuFlash);

    spi_buffer[0] = 0x20;
    spi_buffer[1] = (uint8_t) (address >> 16);
    spi_buffer[2] = (uint8_t) (address >> 8);
    spi_buffer[3] = (uint8_t) (address >> 0);

    spiPerformTaskBlocking_internal(4, spi_buffer, 0, NULL);

    deselectFlash(mcuFlash);

    // wait until done
    waitDoneFlash_internal(mcuFlash);
}

void writeDataFlash(uint32_t address, uint8_t *data, uint16_t length, uint8_t mcuFlash) {
    if (length + 4 > SPI_BUFFER_SIZE) {
#ifndef TEST
        debugWriteLine("Cannot write data! Too large for buffer");
#endif
    } else {
        flashResetCallbacks_internal();

        writeEnableFlash_internal(mcuFlash);

        selectFlash(mcuFlash);

        // put instructions in buffer
        spi_buffer[0] = 0x02;
        spi_buffer[1] = (uint8_t) (address >> 16);
        spi_buffer[2] = (uint8_t) (address >> 8);
        spi_buffer[3] = (uint8_t) (address >> 0);

        memcpy(spi_buffer + 4, data, length);

        spiPerformTaskBlocking_internal(4 + length, spi_buffer, 0, NULL);

        deselectFlash(mcuFlash);
        // wait until done?
        waitDoneFlash_internal(mcuFlash);
    }
}

uint8_t *
readDataFlash(uint32_t address, uint32_t numBytes, uint8_t mcuFlash, void (*readingCallbackFunction)(uint8_t, uint8_t),
              void (*finishedCallbackFunction)(void)) {
    selectFlash(mcuFlash);

    // put instructions backward in buffer
    spi_buffer[0] = 0x03;
    spi_buffer[1] = (uint8_t) (address >> 16);
    spi_buffer[2] = (uint8_t) (address >> 8);
    spi_buffer[3] = (uint8_t) (address >> 0);


    if (readingCallbackFunction == NULL) {
        spiPerformTaskBlocking_internal(4, spi_buffer, numBytes, spi_buffer + 4);
        deselectFlash(mcuFlash);
    } else {
        spiPerformTaskBlockingWithCallback_internal(4, spi_buffer, numBytes, readingCallbackFunction,
                                                    finishedCallbackFunction);
    }

/*    if (mcuFlash) {
        // send commmand
        if (readingCallbackFunction == NULL) {
            spiPerformTaskBlocking_internal(4, spi_buffer, numBytes, spi_buffer + 4);
            deselectFlash(mcuFlash);
        } else {
            spiPerformTaskBlockingWithCallback_internal(4, spi_buffer, numBytes, readingCallbackFunction,
                                                        finishedCallbackFunction);

        }
    } else {
        // send commmand
        if (readingCallbackFunction == NULL) {
            spiPerformTaskBlocking_internal(4, spi_buffer, numBytes, spi_buffer + 4);
            deselectFlash(mcuFlash);
        } else {
            fpgaFlashPerformTaskWithCallback(4, spi_buffer, numBytes, readingCallbackFunction,
                                             finishedCallbackFunction);
        }
    }
*/
    return spi_buffer + 4;
}

// performs global block-protection unlock
void unlockFlash(uint8_t mcuFlash) {

    //flashResetCallbacks();
    writeEnableFlash_internal(mcuFlash);

    selectFlash(mcuFlash);

    spiPerformSimpleTaskBlocking(0x98, 0, NULL);

    deselectFlash(mcuFlash);
}
