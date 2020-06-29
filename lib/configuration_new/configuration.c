#include "lib/debug/debug.h"
#include "lib/flash_new/flash.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/interruptManager/interruptManager.h"
#include "lib/configuration_new/configuration.h"
#include <avr/interrupt.h>

#define BUFFER_SIZE 256
#define BACKUP_ADDRESS 0x120000
uint32_t configAddress, configSize, configDestination, configRemaining;
uint8_t *buffer;

void readData(uint8_t *buffer, uint16_t num);

void readValue(uint32_t *destination)
{
    readData((uint8_t *) destination, sizeof(uint32_t));

    debugWriteStringLength(destination, sizeof(uint32_t));
}


void readData(uint8_t *buffer, uint16_t num)
{
    uint8_t *ptr = buffer;
    for (uint16_t i = 0; i < num; i++) {
        *ptr++ = (uint8_t) debugReadCharBlock();

        /*
         * Comment in to see blinking leds while the progress of uploading the bitfile

        if(i%2==0){
            BitManipulation_setBit(&PORTD, PD4);
        }
        else
        {
            BitManipulation_clearBit(&PORTD, PD4);
        }
         */
    }

}

void configurationUartFlash(void) {
    ////fpgaPower(0);
    elasticnode_fpgaPowerOff();
    ////setFpgaHardReset();
    elasticnode_fpgaHardReset();
    ////setLed(0, 1);
    BitManipulation_setBit(&PORTD, PD7);

    // getting address
    readValue(&configAddress);
    BitManipulation_setBit(&PORTD, PD6);
    // getting size
    readValue(&configSize);

    buffer = (uint8_t *) malloc(BUFFER_SIZE);

    flashEnableInterface();

    uint16_t blockSize = BUFFER_SIZE;

    ////setLed(1, 1);
    BitManipulation_setBit(&PORTD, PD6);

    debugWriteString("Erasing flash... ");
    uint16_t numBlocks4K = ceil((float)(configSize) / 0x1000);
    debugWriteDec16(numBlocks4K);
    debugWriteString(" ");
    debugWriteDec32(configSize);
    debugNewLine();
    uint32_t blockAddress;
    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
    {
        blockAddress = configAddress + ((uint32_t) blockCounter) * 0x1000;
        eraseSectorFlash(blockAddress, 1);
    }



    BitManipulation_clearBit(&PORTD, PD6);
    BitManipulation_clearBit(&PORTD, PD7);
    uint32_t currentAddress = configAddress;
    configRemaining = configSize;

    debugReady();
    while(configRemaining > 0) {
        if (configRemaining < BUFFER_SIZE)
            blockSize = configRemaining;
        BitManipulation_setBit(&PORTD, PD5);
        readData(buffer, blockSize);
//        while(read_a_block_of_data(buffer)!=blockSize);
        BitManipulation_setBit(&PORTD, PD4);

        writeDataFlash(currentAddress, buffer, blockSize, 1);
        debugAck(buffer[blockSize - 1]);

        ////setLed(3, 0);
        BitManipulation_clearBit(&PORTD, PD4);
        ////setLed(2, 0);
        BitManipulation_clearBit(&PORTD, PD5);

        currentAddress += blockSize;
        configRemaining -= blockSize;


        debugDone();
    }
    free(buffer);

    debugDone();

    interruptManager_setInterrupt();
}

void verifyConfigurationFlash(uint8_t mcuFlash)
{
//    fpgaPower(0);
    elasticnode_fpgaHardReset();
    flashEnableInterface();

//    setLed(0, 1);
    readValue(&configAddress);  // getting address
//    setLed(1, 1);
    readValue(&configSize);     // getting size
//    setLed(0, 0);
//    setLed(1, 0);

//    debugReady();
//    setLed(2, 1);
    buffer = readDataFlash(configAddress, configSize, mcuFlash, NULL, NULL);
//    setLed(3, 1);
    debugWriteStringLength(buffer, configSize);
//    setLed(3, 0);
//    setLed(2, 0);

    debugReady();
    debugDone();


    sei();
}