#include "src/configuration/configuration.h"
#include "src/configuration/configuration_internal.h"

#include "src/flash/flash.h"
#include "src/interruptManager/interruptManager.h"
#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"

#include <math.h>

#if defined TEST

#include <stdlib.h>

#else

#include "src/led/led_mcu.h"
#include "src/debug/debug.h"

#endif

uint32_t configAddress, configSize, configRemaining;
uint8_t *buffer;

void configurationUartFlash(void) {

    elasticnode_fpgaPowerOff_internal();
    elasticnode_setFpgaHardReset_internal();
    elasticnode_clearFpgaHardReset_internal();
//    led_mcu_turnOn(3);

    // getting address
    readValue_internal(&configAddress);
    // getting size
    readValue_internal(&configSize);

    buffer = (uint8_t *) malloc(BUFFER_SIZE);

    flashEnableInterface();

    uint16_t blockSize = BUFFER_SIZE;

//    led_mcu_turnOn(2);
#ifndef TEST
    debugWriteString("Erasing flash... ");
#endif
    uint16_t numBlocks4K = ceilf((float) (configSize) / 0x1000);
    //uint16_t numBlocks4K = ceil((float) (configSize) / 0x1000);
#ifndef TEST
    debugWriteDec16(numBlocks4K);
    debugWriteString(" ");
    debugWriteDec32(configSize);
    debugNewLine();
#endif
    uint32_t blockAddress;
    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++) {
        blockAddress = configAddress + ((uint32_t) blockCounter) * 0x1000;
        eraseSectorFlash(blockAddress, 1);
    }

    uint32_t currentAddress = configAddress;
    configRemaining = configSize;
#ifndef TEST
    debugReady();
#endif
    while (configRemaining > 0) {
        if (configRemaining < BUFFER_SIZE) {
            blockSize = configRemaining;
        }
        readData_internal(buffer, blockSize);
#ifndef TEST
        led_mcu_turnOn(0);
#endif
        writeDataFlash(currentAddress, buffer, blockSize, 1);
#ifndef TEST
        debugAck(buffer[blockSize - 1]);
#endif
        currentAddress += blockSize;
        configRemaining -= blockSize;

#ifndef TEST
        debugDone();
#endif
    }
    free(buffer);
#ifndef TEST
    debugDone();
#endif
    interruptManager_setInterrupt();
}

// not used until now
// TODO: eventually include verification of flash
void verifyConfigurationFlash(uint8_t mcuFlash) {
    elasticnode_setFpgaHardReset_internal();
    elasticnode_clearFpgaHardReset_internal();
    flashEnableInterface();

#ifndef TEST
    led_mcu_turnOn(0);
#endif
    readValue_internal(&configAddress);  // getting address
#ifndef TEST
    led_mcu_turnOn(1);
#endif
    readValue_internal(&configSize);     // getting size
#ifndef TEST
    led_mcu_turnOff(0);
    led_mcu_turnOff(1);
    led_mcu_turnOn(2);
#endif
    buffer = readDataFlash(configAddress, configSize, mcuFlash, NULL, NULL);
#ifndef TEST
    led_mcu_turnOn(3);
    debugWriteStringLength(buffer, configSize);
    led_mcu_turnOff(3);
    led_mcu_turnOff(2);
    debugReady();
    debugDone();
#endif
    interruptManager_setInterrupt();
}
