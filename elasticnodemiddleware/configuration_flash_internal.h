//
// Created by annika on 29.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_INTERNAL_H

#include <stdint.h>

void readValue_internal(uint32_t *destination);
void readData_internal(uint8_t *buffer, uint16_t num);

#endif //ELASTICNODEMIDDLEWARE_CONFIGURATION_FLASH_INTERNAL_H
