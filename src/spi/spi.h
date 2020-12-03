#ifndef ELASTICNODEMIDDLEWARE_SPI_H
#define ELASTICNODEMIDDLEWARE_SPI_H

#include <stdio.h>
#include <avr/io.h>

void spiInit(void);
void spiDisable(void);
void spiEnable(void);
void flashResetCallbacks(void);
void spiPerformSimpleTaskBlocking(uint8_t command, uint32_t numRead, uint8_t *dataRead);
void spiPerformTaskBlocking(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead, uint8_t *dataRead);
void spiEnable(void);
uint8_t spiRead(void);
void spiWrite(uint8_t data);
uint8_t SPI(uint8_t data);

//flash
void selectFlash(uint8_t mcuFlash);
void deselectFlash(uint8_t mcuFlash);

// wireless
void deselectWireless(void);
void spiPerformTaskBlockingWithCallback(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead,
                                        void (*readingCallback)(uint8_t, uint8_t), void (*finishedCallback)(void));
void fpgaFlashPerformTaskWithCallback(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead, void (*readingCallback)(uint8_t, uint8_t),  void (*finishedCallback)(void));
#endif //ELASTICNODEMIDDLEWARE_SPI_H
