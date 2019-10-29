//
// Created by annika on 29.10.19.
//

#include "elasticnodemiddleware/configuration_flash_internal.h"
#include "elasticnodemiddleware/lufa.h"

void readData_internal(uint8_t *buffer, uint16_t num){
    uint8_t *ptr = buffer;
    for (uint16_t i = 0; i < num; i++) {
        *(ptr+i) = (uint8_t) lufaReadByteBlocking();
    }
}

void readValue_internal(uint32_t *destination){
    readData_internal((uint8_t*)destination, sizeof(uint32_t));

    lufaWriteStringLength(destination, sizeof(uint32_t));
}