// This class is needed to avoid cycle dependencies!
#include "lib/fpgaFlash_new/fpgaFlashSelect.h"
#include "lib/xmem/xmem.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/interruptManager/interruptManager.h"

void selectFpgaFlash(void) {
    spiEnable_internal();
    ////disableXmem();
    xmem_disableXmem();

    PORT_FLASH_CS &= ~_BV(P_FLASH_CS);
}


void deselectFpgaFlash(void)
{
    spiEnable_internal();
    ////disableXmem();
    xmem_disableXmem();

    PORT_FLASH_CS |= _BV(P_FLASH_CS);

}


void spiEnable_internal(void)
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