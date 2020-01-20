//
// Created by annika on 08.01.20.
//

//case P
//fpgaPowerTest(10)

#include <stdint.h>
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "EmbeddedUtilities/BitManipulation.h"

//4th LED should blink 10 times
int main() {
    DDRD = 0xff;

    for(uint8_t i = 0; i < 10; i++) {
        _delay_ms(1000);
        BitManipulation_setBit(&PORTD, PD4);
        elasticnode_fpgaPowerOff();
        _delay_ms(1000);
        BitManipulation_clearBit(&PORTD, PD4);
        elasticnode_fpgaPowerOn();
    }
}