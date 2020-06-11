#ifndef ELASTICNODEMIDDLEWARE_FLASH_H
#define ELASTICNODEMIDDLEWARE_FLASH_H

#include <stdio.h>
#include <string.h>

#ifdef TEST

#else

#include <util/delay.h>

#endif

#define FLASH_SPEED_LOW 1
#define SPI_BUFFER_SIZE 300 // page + command
#define FLASH_BUFFER_SIZE 256

#define P_FLASH_MOSI PB2
#define P_FLASH_SCK PB1

void initFlash(void);
void flashEnableInterface(void);
void flashDisableInterface(void);
void eraseSectorFlash(uint32_t address, uint8_t mcuFlash);
void writeDataFlash(uint32_t address, uint8_t *data, uint16_t length, uint8_t mcuFlash);
void flashSetSpeed(uint8_t speed);
void flashResetQueue(void);
void flashResetWriteCount(void);
void writeEnableFlash(uint8_t mcuFlash);
void waitDoneFlash(uint8_t mcuFlash);
uint8_t readStatus(uint8_t mcuFlash);


uint8_t flashQueueData(uint8_t data);
void flashWriteQueue(uint32_t address, uint8_t mcuFlash);
void unlockFlash(uint8_t mcuFlash);
void fpgaFlashFinishedHelper(void);
uint8_t *readDataFlash(uint32_t address, uint32_t numBytes, uint8_t mcuFlash, void (*readingCallbackFunction)(uint8_t, uint8_t), void (*finishedCallbackFunction)(void));
void fpgaFlashSetWritingAddress(uint32_t destination);
#endif //ELASTICNODEMIDDLEWARE_FLASH_H
