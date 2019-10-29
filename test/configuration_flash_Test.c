//
// Created by annika on 28.10.19.
//

#include "unity.h"
#include "elasticnodemiddleware/configuration_flash.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware.h"
#include "elasticnodemiddleware/Mockleds.h"
#include "elasticnodemiddleware/Mocklufa.h"
#include "elasticnodemiddleware/Mockconfiguration_flash_internal.h"

void test_readData(){

    //statt debugReadCharBlock --> lufaReadByteBlocking

    uint8_t tmp;
    uint8_t* buffer = &tmp;
    uint16_t num = 3;

    uint8_t *ptr = buffer;

    lufaReadByteBlocking_ExpectAndReturn((*ptr));
    lufaReadByteBlocking_ExpectAndReturn((*ptr+1));
    lufaReadByteBlocking_ExpectAndReturn((*ptr+2));

    readData(buffer, num);
}

void test_readValue() {
    uint32_t* destination;

    readData_internal_Expect((uint8_t*) destination, sizeof(uint32_t));

    lufaWriteStringLength_Expect(destination, sizeof(uint32_t));

    readValue(destination);
}

void test_configuration_flash_Flash() {

    cli_Expect();

    setLed_Expect(0, 1);

    readValue_internal_Expect(&configAddress);
    // getting size
    readValue_internal_Expect(&configSize);
    // getting destination
    readValue_internal_Expect(&configDestination);

    setLed_Expect(1, 1);

    //NOT TESTED!!!

    /*printf("Erasing flash... ");

    uint16_t numBlocks4K = ceil(configSize / 0x1000);

    //debugWriteDec16(numBlocks4K);
        char *buf = (char *) malloc(10);
        sprintf(buf, "%u", numBlocks4K);
        //debugWriteString(buf);
            printf(buf);
        free(buf);
    //debugNewLine()
        //debugWriteChar('\r');
        //debugWriteChar('\n');
    putchar('\r');
    putchar('\n');
     */

    uint32_t blockAddress;

    //TODO: implement
    //eraseSectorFlash(blockAddress, 0); not implement ->missing (lots of dependencies with other interfaces)

    /*//debugReady()
        //debugWriteLine("\n%%");
            //debugWriteString(s);
                printf("\n%%");
            //debugWriteChar('\n');
                putchar('\n');
         */

    setLed_Expect(1, 0);

    configuration_flash_Flash();

    //example numBlocks4K here: 0
    //configSize = 0
    //numBlocks4K = ceil(configSize / 0x1000);

    blockAddress = configAddress;
    TEST_ASSERT_EQUAL_UINT16((*ptr_numBlock4K), blockAddress);


}