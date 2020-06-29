#include "lib/flash_new/flash.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/spi_new/spi.h"
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
void flashDisableInterface(void)
{
    spiDisable();
    DDR_FLASH_CS &= ~(_BV(P_FLASH_CS) | _BV(P_FLASH_MOSI) | _BV(P_FLASH_SCK));
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

    //// Changed!
    /*if (mcuFlash)
    {
        spiPerformTaskBlocking(4, spi_buffer, 0, NULL);
    }
    else
    {
        fpgaFlashPerformTask(4, spi_buffer, 0, NULL);
    }*/
    //// new
    spiPerformTaskBlocking(4, spi_buffer, 0, NULL);

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

        //// Changed!
        /*if (mcuFlash) {
            spiPerformTaskBlocking(4 + length, spi_buffer, 0, NULL);
        } else {
            fpgaFlashPerformTask(4 + length, spi_buffer, 0, NULL);
        }*/
        //// new
        spiPerformTaskBlocking(4 + length, spi_buffer, 0, NULL);

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

    //// Changed!
    /*if (mcuFlash)
    {
        spiPerformSimpleTaskBlocking(0x06, 0, NULL);
    }
    else
    {
        fpgaFlashPerformSimpleTask(0x06, 0, NULL);
    }*/
    //// new
    spiPerformSimpleTaskBlocking(0x06, 0, NULL);

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

    //// Changed!
    /*if (mcuFlash)
    {
        spiPerformSimpleTaskBlocking(0x05, 1, &status);
    }
    else
    {
        fpgaFlashPerformSimpleTask(0x05, 1, &status);
    }*/
    //// new
    spiPerformSimpleTaskBlocking(0x05, 1, &status);

    deselectFlash(mcuFlash);

    return status;
}
uint8_t *readDataFlash(uint32_t address, uint32_t numBytes, uint8_t mcuFlash, void (*readingCallbackFunction)(uint8_t, uint8_t), void (*finishedCallbackFunction)(void))
{
    selectFlash(mcuFlash);
    // spi_busy = 0x01;

    // put instructions backward in buffer
    spi_buffer[0] = 0x03;
    spi_buffer[1] = (uint8_t) (address >> 16);
    spi_buffer[2] = (uint8_t) (address >> 8);
    spi_buffer[3] = (uint8_t) (address >> 0);
    // spi_pointer = spi_buffer;

    // // set up callbacks
    // readingCallback = readingCallbackFunction;
    // finishedCallback = finishedCallbackFunction;

    if (mcuFlash)
    {
        // send commmand
        if (readingCallbackFunction == NULL)
        {
            spiPerformTaskBlocking(4, spi_buffer, numBytes, spi_buffer + 4);
            deselectFlash(mcuFlash);
        }
        else
            spiPerformTaskBlockingWithCallback(4, spi_buffer, numBytes, readingCallbackFunction, finishedCallbackFunction);


        // spi_send_remaining = 4;
        // spi_rec_remaining = numBytes;
        // SPDR = *spi_pointer++;

        // mcuReadCount += numBytes;
        // // if no reading callback, act as blocking
        // if (readingCallback == NULL)
        // {
        // 	while(spi_busy);

        // 	deselectFlash(mcuFlash);
        // }

    }
    else
    {
        // send commmand
        if (readingCallbackFunction == NULL)
        {
            //fpgaFlashPerformTask(4, spi_buffer, numBytes, spi_buffer + 4);
            spiPerformTaskBlocking(4, spi_buffer, numBytes, spi_buffer + 4);
            deselectFlash(mcuFlash);
        }
        else
            fpgaFlashPerformTaskWithCallback(4, spi_buffer, numBytes, readingCallbackFunction, finishedCallbackFunction);

    }

    return spi_buffer + 4;
}
uint16_t flashQueueCount;
uint16_t getFlashQueueCount(void) { return flashQueueCount; }

// queue one byte of data to be written, if buffer full return 1
uint8_t flashQueueData(uint8_t data)
{
    // setLed(1, 1);
    *flashBufPtr++ = data;
    flashQueueCount++;
    // setLed(0, 0);
    return flashQueueCount >= 256;
}

// write up to one page of data
void flashWriteQueue(uint32_t address, uint8_t mcuFlash)
{
    // return immediately if nothing to write
    if (flashQueueCount == 0)
    {
        debugWriteLine("nothing to be written to flash...");
        return;
    }

    // debugWriteString("queue: ");
    // debugWriteDec32(flashQueueCount);
    // debugNewLine();

    // writing buffer already setup
    // writeDataFlash(address, flashBuf, flashQueueCount());
    writeEnableFlash(mcuFlash);

    selectFlash(mcuFlash);
    // spi_busy = 0x01;

    flashWriteBuf[1] = (uint8_t) (address >> 16);
    flashWriteBuf[2] = (uint8_t) (address >> 8);
    flashWriteBuf[3] = (uint8_t) (address >> 0);

    // spi_pointer = flashWriteBuf;

    //// Changed!
    /*if (mcuFlash)
    {
        spiPerformTaskBlocking(4 + flashQueueCount, flashWriteBuf, 0, NULL);
    }
    else
    {
        fpgaFlashPerformTask(4 + flashQueueCount, flashWriteBuf, 0, NULL);
    }*/
    //// new
    spiPerformTaskBlocking(4 + flashQueueCount, flashWriteBuf, 0, NULL);

    deselectFlash(mcuFlash);
    // wait until done?
    waitDoneFlash(mcuFlash);

    // reset flash buffer
    flashResetQueue();
}

// performs global block-protection unlock
void unlockFlash(uint8_t mcuFlash)
{

    flashResetCallbacks();
    writeEnableFlash(mcuFlash);

    selectFlash(mcuFlash);

    //// Changed!
    /*if (mcuFlash)
    {
        spiPerformSimpleTaskBlocking(0x98, 0, NULL);
    }
    else
    {
        fpgaFlashPerformSimpleTask(0x98, 0, NULL);
    }*/
    //// new
    spiPerformSimpleTaskBlocking(0x98, 0, NULL);

    deselectFlash(mcuFlash);
}

uint32_t addressFlash;
// address where flash data is to be stored
void fpgaFlashSetWritingAddress(uint32_t destination)
{
    addressFlash = destination;
}