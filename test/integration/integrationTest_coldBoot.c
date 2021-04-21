#include <stdio.h>
#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include <avr/interrupt.h>

#include "src/debug/debug.h"

//4th LED blinks 2x
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test cold boot");

    // cold boot experiment --> case 'c'

    BitManipulation_setBit(&PORTD, PD4);

    debugWriteLine("FPGA off");

    elasticnode_fpgaPowerOff();
    _delay_ms(100);
    debugWriteLine("1");

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);

    //startExperiment(1, 0);
    //not implemented
    //timer_start(1);

    _delay_ms(500);
    BitManipulation_setBit(&PORTD, PD4);

    elasticnode_fpgaPowerOn();

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);
    // response in fpga done isr?

    debugWriteLine("integration test cold boot finished");
}
