#include "configuration_internal.h"

#ifndef TEST

#include "src/debug/debug.h"

#endif

void readData_internal(uint8_t *buffer, uint16_t num) {
#ifndef TEST
    uint8_t *ptr = buffer;
    for (uint16_t i = 0; i < num; i++) {
        *ptr++ = (uint8_t) debugReadCharBlock();
    }
#endif
}

void readValue_internal(uint32_t *destination) {
    readData_internal((uint8_t *) destination, sizeof(uint32_t));
#ifndef TEST
    debugWriteStringLength((char *) destination, sizeof(uint32_t));
#endif
}
