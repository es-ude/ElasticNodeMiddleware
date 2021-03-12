#include "unity.h"

#include "src/configuration/configuration.h"

#include "src/configuration/Mockconfiguration_internal.h"

#include "src/flash/Mockflash.h"
#include "src/interruptManager/MockinterruptManager.h"
#include "src/elasticNodeMiddleware/MockelasticNodeMiddleware_internal.h"
#include "src/delay/Mockdelay.h"

void test_configurationUartFlash(void) {
    // TODO: malloc test header
/*
    elasticnode_fpgaPowerOff_internal_Expect();
    elasticnode_setFpgaHardReset_internal_Expect();
    elasticnode_clearFpgaHardReset_internal_Expect();

    readValue_internal_Expect(&configAddress);

    readValue_internal_Expect(&configSize);

    buffer = (uint8_t *) malloc(BUFFER_SIZE);

    flashEnableInterface();

    uint16_t blockSize = BUFFER_SIZE;

    uint16_t numBlocks4K = ceilf((float) (configSize) / 0x1000);

    uint32_t blockAddress;
    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++) {
        blockAddress = configAddress + ((uint32_t) blockCounter) * 0x1000;
        eraseSectorFlash(blockAddress, 1);
    }

    uint32_t currentAddress = configAddress;
    configRemaining = configSize;

    while (configRemaining > 0) {
        if (configRemaining < BUFFER_SIZE) {
            blockSize = configRemaining;
        }

        readData_internal_Expect(buffer, blockSize);

        writeDataFlash(currentAddress, buffer, blockSize, 1);

        currentAddress += blockSize;
        configRemaining -= blockSize;

    }
    free(buffer);


    interruptManager_setInterrupt_Expect();

    configurationUartFlash();
    */
}

void test_verifyConfigurationFlash(void) {
    uint8_t mcuFlash = 3;
    uint32_t configAddress = 0;
    uint32_t configSize = 0;
    uint8_t *buffer = (uint8_t *) 3;

    elasticnode_setFpgaHardReset_internal_Expect();
    elasticnode_clearFpgaHardReset_internal_Expect();
    flashEnableInterface_Expect();

    readValue_internal_Expect(&configAddress);
    readValue_internal_Expect(&configSize);
    readDataFlash_ExpectAndReturn(configAddress, configSize, mcuFlash, NULL, NULL, buffer);

    interruptManager_setInterrupt_Expect();

    verifyConfigurationFlash(mcuFlash);

}
