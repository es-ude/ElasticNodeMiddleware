#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include <util/delay.h>

#include <avr/interrupt.h>

#include "src/debug/debug.h"

//test if Bits are set + testing multiboot
//5th LED blink
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test reconfigure multiboot");

    //variable initialisation tested in unit tests
    //reconfigure_initMultiboot();

    //testing multiboot --> case 'X'
    BitManipulation_setBit(&PORTD, PD5);

    reconfigure_fpgaMultiboot(0);
    debugWriteLine("reconfigure_fpgaMultiboot() tested");

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD5);
}
