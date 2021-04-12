//case P
//fpgaPowerTest(10)

#include <stdint.h>
#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include <util/delay.h>

#include "src/debug/debug.h"

//4th LED should blink 10 times
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test fpgaPowerTest");
    debugWriteLine("the fpga should power on 10 times");

    for (uint8_t i = 0; i < 10; i++) {
        _delay_ms(1000);
        BitManipulation_setBit(&PORTD, PD4);
        elasticnode_fpgaPowerOff_internal();
        debugWriteLine("fpga power off ");
        debugWriteDec8(i);
        debugNewLine();

        _delay_ms(1000);
        BitManipulation_clearBit(&PORTD, PD4);
        elasticnode_fpgaPowerOn_internal();
        debugWriteLine("fpga power on");
    }
    debugWriteLine("integration test fpgaPowerTest done");

}
