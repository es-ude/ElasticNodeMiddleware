#ifndef ELASTICNODEMIDDLEWARE_FLASH_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_FLASH_INTERNAL_H

#include <stdio.h>
#include <string.h>

#define FLASH_SPEED_LOW 1
#define SPI_BUFFER_SIZE 300 // page + command
#define FLASH_BUFFER_SIZE 256

void flashEnableInterface_internal(void);

void flashSetSpeed_internal(uint8_t speed);

void flashResetQueue_internal(void);

void flashResetWriteCount_internal(void);

void writeEnableFlash_internal(uint8_t mcuFlash);

uint8_t readStatus_internal(uint8_t mcuFlash);

void waitDoneFlash_internal(uint8_t mcuFlash);

#endif //ELASTICNODEMIDDLEWARE_FLASH_INTERNAL_H
