//
// Created by annika on 28.10.19.
//

#include "elasticnodemiddleware/configuration_flash.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/leds.h"
#include "elasticnodemiddleware/lufa.h"
#include "elasticnodemiddleware/configuration_flash_internal.h"

void readValue(uint32_t *destination) {

    readData_internal((uint8_t*)destination, sizeof(uint32_t));

    lufaWriteStringLength(destination, sizeof(uint32_t));
}

void readData(uint8_t *buffer, uint16_t num){
    uint8_t *ptr = buffer;
    for (uint16_t i = 0; i < num; i++) {
        *(ptr+i) = (uint8_t) lufaReadByteBlocking();
    }
}

void configuration_flash_Flash(){
    cli();

    setLed(0, 1);

    readValue_internal(&configAddress);
    // getting size
    readValue_internal(&configSize);
    // getting destination
    readValue_internal(&configDestination);

    setLed(1, 1);

    //not tested
    printf("Erasing flash... ");

    uint16_t numBlocks4K = ceil(configSize / 0x1000);

    //debugWriteDec16(numBlocks4K);
        char *buf = (char *) malloc(10);
        sprintf(buf, "%u", numBlocks4K);
        //debugWriteString(buf);
            printf(buf);
        free(buf);

    //debugNewLine()
        /*debugWriteChar('\r');
          debugWriteChar('\n');*/
        putchar('\r');
        putchar('\n');

    uint32_t blockAddress;

    uint16_t init;
    ptr_numBlock4K = &init;

    for (uint16_t blockCounter = 0; blockCounter < numBlocks4K; blockCounter++)
    {
        blockAddress = configAddress + ((uint32_t) blockCounter) * 0x1000;

        *(ptr_numBlock4K + blockCounter) = blockAddress;

        //TODO!
        //MISSING!!!
        //eraseSectorFlash(blockAddress, 0);
    }

    //debugReady()
        //debugWriteLine("\n%%");
            //debugWriteString(s);
                printf("\n%%");
            //debugWriteChar('\n');
                putchar('\n');

    setLed(1, 0);

    //implementation missing

}