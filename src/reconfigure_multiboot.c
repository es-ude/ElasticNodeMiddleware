//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot.h"
#include "elasticnodemiddleware/reconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"

//c datei avr nennen
//abstraction aus embedded utilities

void reconfigure_initMultiboot() {

    abstraction_setRegisterBitsLow(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    reconfigure_fpgaMultibootClearComplete_internal();
    fpgaDoneResponse = FPGA_DONE_NOTHING;

    //for testing, with suffix, raus, variable im test direkt lesen
    ptr_fpgaDoneResponse = &fpgaDoneResponse;
}
/*
void reconfigure_fpgaMultiboot(uint32_t address) {

    elasticnode_fpgaPowerOn_internal();

    enableXmem();

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal();

    //multiboot lokale Variable, anderer Name, volatile
    reconfigure_initPtrMultiboot_internal();
    for (uint8_t i = 0; i < 3; i++)
    {
        *(multiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
    }

    disableXmem();

    //platform unabhängig, enable beabsichtigt?
    sei();
}
*/
//wofür? ohne flag, state/response
uint8_t reconfigure_fpgaMultibootComplete(void) {
    return fpgaMultibootCompleteFlag;
}

void reconfigure_interruptSR() {

    if (abstraction_getBit(PIN_FPGA_DONE, P_FPGA_DONE)){
        //float duration;
        fpgaMultibootCompleteFlag = 1;
        switch (fpgaDoneResponse) {
            case FPGA_DONE_PRINT:
                //duration = -1;
                cli();

                //debugWriteLine("FPGA Done INT");
                //debugWriteFloatFull(duration);
                //debugNewLine();
                //debugDone();
                break;
            case FPGA_DONE_MULTIBOOT:

                elasticnode_fpgaSoftReset();
                reconfigure_fpgaMultiboot_internal(0);
                break;
            case 0:
            default:
                break;
        }
        sei();
    }
}

ISR(FPGA_DONE_INT_VECTOR)
{
    reconfigure_interruptSR();
}