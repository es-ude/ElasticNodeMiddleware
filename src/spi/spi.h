#ifndef ELASTICNODEMIDDLEWARE_SPI_H
#define ELASTICNODEMIDDLEWARE_SPI_H

#include <stdio.h>

#ifndef TEST

#include <avr/io.h>

#endif

void spiInit(void);

void spiDisable(void);

void spiPerformSimpleTaskBlocking(uint8_t command, uint32_t numRead, uint8_t *dataRead);

uint8_t spiRead(void);

void spiWrite(uint8_t data);

//flash
void selectFlash(uint8_t mcuFlash);

void deselectFlash(uint8_t mcuFlash);

// wireless

void fpgaFlashPerformTaskWithCallback(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead,
                                      void (*readingCallback)(uint8_t, uint8_t), void (*finishedCallback)(void));

#endif //ELASTICNODEMIDDLEWARE_SPI_H
