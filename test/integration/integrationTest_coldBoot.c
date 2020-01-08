//
// Created by annika on 08.01.20.
//

#include <stdio.h>
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
#include "EmbeddedUtilities/BitManipulation.h"

//4th LED blinks 2x
int main() {

    DDRD = 0xff;

    // cold boot experiment --> case 'c'
    //debug print in alwyns code

    BitManipulation_setBit(&PORTD, PD4);

    elasticnode_fpgaPowerOff();

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_MULTIBOOT);

    //startExperiment(1, 0);
    //not implemented
    //timer_start(1);

    _delay_ms(500);
    BitManipulation_setBit(&PORTD, PD4);

    elasticnode_fpgaPowerOn();

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);
    // response in fpga done isr --> missing, not implemented
}
