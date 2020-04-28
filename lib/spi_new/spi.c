#include "lib/interruptManager/interruptManager.h"
#include "lib/spi_new/spi.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/xmem/xmem.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"

// this function is called when a byte is read from flash
// parameters are data and whether this is last byte
void (*readingCallback) (uint8_t, uint8_t) = NULL;
void (*finishedCallback) (void) = NULL;

void spiInit(void)
{
    ////cli();
    interruptManager_clearInterrupt();

    spiEnable();

    flashResetCallbacks();

    ////sei();
    interruptManager_setInterrupt();
}

void flashResetCallbacks(void)
{
    readingCallback = NULL;
    finishedCallback = NULL;
}

void spiPerformTaskBlocking(uint32_t numWrite, uint8_t *dataWrite, uint32_t numRead, uint8_t *dataRead)
{
    uint8_t *ptr;

    ptr = dataWrite;
    for (uint32_t i = 0; i < numWrite; i++)
        spiWrite(*ptr++);
    ptr = dataRead;
    for (uint32_t i = 0; i < numRead; i++)
        *ptr++ = spiRead();
}

void spiPerformSimpleTaskBlocking(uint8_t command, uint32_t numRead, uint8_t *dataRead)
{
    spiPerformTaskBlocking(1, &command, numRead, dataRead);
}


void spiEnable(void)
{

    // ensure master mode (_SS)
    DDRB |= _BV(PB0);
    PORTB |= _BV(PB0);

    ////cli();
    interruptManager_clearInterrupt();
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);

    // reset interrupts
    while (SPSR & _BV(SPIF))
        SPDR;

    SPDR;
}

uint8_t spiRead()
{
    return SPI(0x00);
}

void spiWrite(uint8_t data)
{
    SPI(data);
}

uint8_t SPI(uint8_t data)
{
    ////cli();
    interruptManager_clearInterrupt();
    SPDR = data;
    while (!(SPSR & _BV(SPIF)))
    {
    }

    return SPDR;
}

//in alwyns code in spiarbritration

void selectFlash(uint8_t mcuFlash)
{
    if (mcuFlash)
    {
        deselectWireless();
        spiEnable();
        ////disableXmem();
        xmem_disableXmem();

        // many copies of this
        DDR_FLASH_CS |= _BV(P_FLASH_CS);
        PORT_FLASH_CS &= ~_BV(P_FLASH_CS);
    }
    else
        selectFpgaFlash();

}

// placeholder function
void deselectWireless(void)
{
    DDR_WIRELESS_CS |= _BV(P_WIRELESS_CS);
    PORT_WIRELESS_CS |= _BV(P_WIRELESS_CS);
}

void deselectFlash(uint8_t mcuFlash)
{
    if (mcuFlash)
    {
        spiEnable();
        ////disableXmem();
        xmem_disableXmem();

        DDR_FLASH_CS |= _BV(P_FLASH_CS);
        PORT_FLASH_CS |= _BV(P_FLASH_CS);
    }
    else
        deselectFpgaFlash();

}