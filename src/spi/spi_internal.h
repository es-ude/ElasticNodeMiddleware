#ifndef ELASTICNODEMIDDLEWARE_SPI_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_SPI_INTERNAL_H

#include <stdio.h>

#ifndef TEST

#include <avr/io.h>

#endif

void flashResetCallbacks_internal(void);

void spiPerformTaskBlocking_internal(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead, uint8_t *dataRead);

void spiEnable_internal(void);

uint8_t SPI_internal(uint8_t data);

// wireless
void deselectWireless_internal(void);

void spiPerformTaskBlockingWithCallback_internal(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead,
                                                 void (*readingCallback)(uint8_t, uint8_t),
                                                 void (*finishedCallback)(void));

#endif //ELASTICNODEMIDDLEWARE_SPI_INTERNAL_H
