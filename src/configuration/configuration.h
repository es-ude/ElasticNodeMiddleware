#ifndef ELASTICNODEMIDDLEWARE_CONFIGURATION_H

#include <stdint.h>

#define BUFFER_SIZE 256

void configurationUartFlash(void);

void verifyConfigurationFlash(uint8_t mcuFlash);

#endif //ELASTICNODEMIDDLEWARE_CONFIGURATION_H
