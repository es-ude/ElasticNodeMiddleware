#ifndef ELASTICNODEMIDDLEWARE_CONFIGURATION_H

#include <stdint.h>

#define BUFFER_SIZE 256

void configurationFlash(void (*readData)(uint8_t *, uint16_t));

void verifyConfigurationFlash(uint8_t mcuFlash);

#endif //ELASTICNODEMIDDLEWARE_CONFIGURATION_H
