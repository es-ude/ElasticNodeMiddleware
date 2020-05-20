#include "lib/debug/debug.h"
//#include "lib/uart/uart_internal.h"
//#include "lib/uart/uart.h"

//missing!
//#include "lib/flash/flash.h"
#include "lib/flash_new/flash.h"

////#include "lib/leds/leds.h"
#include "EmbeddedUtilities/BitManipulation.h"

////#include "lib/fpga/fpga.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"

#include "lib/interruptManager/interruptManager.h"
//#include "lib/configuration_new/configuration.h"
#include <avr/interrupt.h>

#define BUFFER_SIZE 256
uint32_t configAddress, configSize, configRemaining;
uint8_t *buffer;

void readData(uint8_t *buffer, uint16_t num);

void readValue(uint32_t *destination)
{
    debugWriteLine("test1.1");
    _delay_ms(100);

    readData((uint8_t *) destination, sizeof(uint32_t));

    debugWriteLine("test1.2");
    _delay_ms(100);

    debugWriteStringLength(destination, sizeof(uint32_t));
    //uart_WriteStringLengthBlock(destination, sizeof(uint32_t));
}

void readData(uint8_t *buffer, uint16_t num)
{
    uint8_t *ptr = buffer;
    for (uint16_t i = 0; i < num; i++) {

        debugWriteLine("test1.1.2");
        _delay_ms(100);

        *ptr++ = (uint8_t) debugReadCharBlock();
        //*ptr++ = (uint8_t) uart_ReceiveCharBlocking_internal();

        debugWriteLine("test1.1.3");
        _delay_ms(100);
    }

}

void configurationUartFlash(void) {

    ////fpgaPower(0);
    elasticnode_fpgaPowerOff();
    ////setFpgaHardReset();
    debugWriteLine("fpga hard reset...");
    _delay_ms(100);
    elasticnode_fpgaHardReset();
    debugWriteLine("... works");
    _delay_ms(100);
    ////setLed(0, 1);
    BitManipulation_setBit(&PORTD, PD7);


    debugWriteLine("test1");
    _delay_ms(100);

    // getting address
    readValue(&configAddress);

    debugWriteLine("test2");
    _delay_ms(100);


    // getting size
    readValue(&configSize);

    buffer = (uint8_t *) malloc(BUFFER_SIZE);

    flashEnableInterface();

    uint16_t blockSize = BUFFER_SIZE;

    ////setLed(1, 1);
    BitManipulation_setBit(&PORTD, PD6);

    debugWriteString("Erasing flash... ");
    //uart_WriteStringBlock("Erasing flash... ");

    uint16_t numBlocks4K = ceil((float)(configSize) / 0x1000);

    debugWriteDec16(numBlocks4K);
    /*char *buf = (char *) malloc(10);
    sprintf(buf, "%u", numBlocks4K);
    uart_WriteStringBlock(buf);
    free(buf);*/

    debugWriteString(" ");
    //uart_WriteStringBlock(" ");

    debugWriteDec32(configSize);
    /*char *buffer = (char *) malloc(10);
    sprintf(buf, "%lu", configSize);
    uart_WriteStringBlock(buffer);
    free(buffer);*/

    debugNewLine();
    //uart_WriteCharBlock_internal('\r');
    //uart_WriteCharBlock_internal('\n');

    uint32_t blockAddress;
    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
    {
        blockAddress = configAddress + ((uint32_t) blockCounter) * 0x1000;
        eraseSectorFlash(blockAddress, 1);
    }


    debugReady();
    //uart_WriteStringBlock("\n%%");
    //uart_WriteCharBlock_internal('\r');
    //uart_WriteCharBlock_internal('\n');

    ////setLed(1, 0);
    BitManipulation_clearBit(&PORTD, PD6);
    ////setLed(0, 0);
    BitManipulation_clearBit(&PORTD, PD7);

    uint32_t currentAddress = configAddress;
    configRemaining = configSize;


    while(configRemaining > 0) {
        if (configRemaining < BUFFER_SIZE)
            blockSize = configRemaining;
        ////setLed(2, 1);
        BitManipulation_setBit(&PORTD, PD5);

        readData(buffer, blockSize);
        ////setLed(3, 1);
        BitManipulation_setBit(&PORTD, PD4);

        writeDataFlash(currentAddress, buffer, blockSize, 1);
        debugAck(buffer[blockSize - 1]);
        //uart_WriteCharBlock_internal(buffer[blockSize - 1]);

        ////setLed(3, 0);
        BitManipulation_clearBit(&PORTD, PD4);
        ////setLed(2, 0);
        BitManipulation_clearBit(&PORTD, PD5);

        currentAddress += blockSize;
        configRemaining -= blockSize;


        debugDone();
        //uart_WriteStringBlock("\n$$");
        //uart_WriteCharBlock_internal('\r');
        //uart_WriteCharBlock_internal('\n');
    }
    free(buffer);

    debugDone();
    //uart_WriteStringBlock("\n$$");
    //uart_WriteCharBlock_internal('\r');
    //uart_WriteCharBlock_internal('\n');

    interruptManager_setInterrupt();
}