#ifndef ELASTICNODEMIDDLEWARE_FPGAFLASH_H
#define ELASTICNODEMIDDLEWARE_FPGAFLASH_H

#include <stdio.h>

void fpgaFlashInit(void);
void fpgaFlashPerformSimpleTask(uint8_t command, uint16_t numRead, uint8_t *dataRead);
void fpgaFlashPerformTask(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead, uint8_t *dataRead);

#endif //ELASTICNODEMIDDLEWARE_FPGAFLASH_H
