#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/spi_new/spi.h"

void fpgaFlashInit(void)
{
    spiInit();
}

void fpgaFlashPerformSimpleTask(uint8_t command, uint16_t numRead, uint8_t *dataRead)
{
    spiPerformSimpleTaskBlocking(command, numRead, dataRead);
}

void fpgaFlashPerformTask(uint16_t numWrite, uint8_t *dataWrite, uint16_t numRead, uint8_t *dataRead)
{
    spiPerformTaskBlocking(numWrite, dataWrite, numRead, dataRead);
}
