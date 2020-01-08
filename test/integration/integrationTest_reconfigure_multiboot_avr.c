//
// Created by annika on 18.12.19.
//

#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include <util/delay.h>

//4th LED on + 5th LED blink
int main() {

    DDRD = 0xff;

    //variable initialisation tested in unit tests
    reconfigure_initMultiboot();

    if( BitManipulation_bitIsSetOnArray(FPGA_DONE_INT_REG, FPGA_DONE_INT) &&
    BitManipulation_bitIsSetOnArray(FPGA_DONE_INT_CONTROL_REG, FPGA_DONE_INT_CONTROL)) {

        BitManipulation_setBit(&PORTD, PD4);
    }

    //testing multiboot --> case 'X'
    BitManipulation_setBit(&PORTD, PD5);

    reconfigure_fpgaMultiboot(0);

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD5);

}