#include "lib/flash_new/flash.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/spi_new/spi.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/debug/debug.h"

uint8_t spi_buffer[SPI_BUFFER_SIZE];
volatile uint8_t *flashBufPtr;
uint8_t flashWriteBuf[FLASH_BUFFER_SIZE];
uint16_t flashQueueCount;
uint32_t mcuWriteCount = 0;
uint32_t fpgaWriteCount = 0;

void initFlash()
{
    flashEnableInterface();

    spiInit();

    flashSetSpeed(FLASH_SPEED_LOW);

    spi_buffer[0] = 0x00;

    flashResetQueue();
    flashResetCallbacks();

    flashResetWriteCount();

    deselectFlash(1);
    deselectFlash(0);
}

void flashEnableInterface(void)
{
    DDR_FLASH_CS |= _BV(P_FLASH_CS) | _BV(P_FLASH_MOSI) | _BV(P_FLASH_SCK);
    PORT_FLASH_CS |= _BV(P_FLASH_CS);
}


void eraseSectorFlash(uint32_t address, uint8_t mcuFlash)
{
    flashResetCallbacks();

    writeEnableFlash(mcuFlash);

    selectFlash(mcuFlash);

    spi_buffer[0] = 0x20;
    spi_buffer[1] = (uint8_t) (address >> 16);
    spi_buffer[2] = (uint8_t) (address >> 8);
    spi_buffer[3] = (uint8_t) (address >> 0);

    if (mcuFlash)
    {
        spiPerformTaskBlocking(4, spi_buffer, 0, NULL);
    }
    else
    {
        fpgaFlashPerformTask(4, spi_buffer, 0, NULL);
    }

    deselectFlash(mcuFlash);

    // wait until done
    waitDoneFlash(mcuFlash);

}


void writeDataFlash(uint32_t address, uint8_t *data, uint16_t length, uint8_t mcuFlash)
{
    if (length + 4 > SPI_BUFFER_SIZE)
    {
        debugWriteLine("Cannot write data! Too large for buffer");
        //uart_WriteLine("Cannot write data! Too large for buffer");
    }
    else
    {
        flashResetCallbacks();

        writeEnableFlash(mcuFlash);

        selectFlash(mcuFlash);

        // put instructions in buffer
        spi_buffer[0] = 0x02;
        spi_buffer[1] = (uint8_t) (address >> 16);
        spi_buffer[2] = (uint8_t) (address >> 8);
        spi_buffer[3] = (uint8_t) (address >> 0);

        memcpy(spi_buffer + 4, data, length);


        if (mcuFlash) {
            spiPerformTaskBlocking(4 + length, spi_buffer, 0, NULL);
        } else {
            fpgaFlashPerformTask(4 + length, spi_buffer, 0, NULL);
        }

        deselectFlash(mcuFlash);
        // wait until done?
        waitDoneFlash(mcuFlash);
    }
}

void flashSetSpeed(uint8_t speed)
{
    SPCR &= ~_BV(SPR0);
    switch(speed)
    {
        //case FLASH_SPEED_HIGH:
            //break;
        case FLASH_SPEED_LOW:
        default:
            SPCR |= _BV(SPR0);  // /16
            break;
    }
    SPDR;
}

void flashResetQueue(void)
{
    flashBufPtr = flashWriteBuf;
    flashQueueCount = 0;
    // create header
    *flashBufPtr++ = 0x02;
    // next three are address
    flashBufPtr += 3;
    // data gets buffered from here
}

void flashResetWriteCount(void)
{
    mcuWriteCount = 0;
    fpgaWriteCount = 0;
}

void writeEnableFlash(uint8_t mcuFlash)
{
    flashResetCallbacks();
    selectFlash(mcuFlash);

    if (mcuFlash)
    {
        spiPerformSimpleTaskBlocking(0x06, 0, NULL);
    }
    else
    {
        fpgaFlashPerformSimpleTask(0x06, 0, NULL);
    }
    deselectFlash(mcuFlash);
}

void waitDoneFlash(uint8_t mcuFlash)
{

    uint16_t counter = 0;
    uint8_t status = 0xFF;
    while(status & _BV(0))
    {
        counter++;
        status = readStatus(mcuFlash);
        _delay_ms(5);
    }
}


// perform RDSR
uint8_t readStatus(uint8_t mcuFlash)
{
    flashResetCallbacks();

    selectFlash(mcuFlash);
    uint8_t status;

    if (mcuFlash)
    {
        spiPerformSimpleTaskBlocking(0x05, 1, &status);
    }
    else
    {
        fpgaFlashPerformSimpleTask(0x05, 1, &status);
    }
    deselectFlash(mcuFlash);

    return status;
}