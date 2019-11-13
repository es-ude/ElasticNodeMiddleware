//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot_avr.h"
#include "elasticnodemiddleware/reconfigure_multiboot_internal_avr.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/xmem.h"

//abstraction aus embedded utilities
volatile uint8_t fpgaDoneResponse = FPGA_DONE_NOTHING;
volatile uint8_t *multiboot = (uint8_t *) (XMEM_OFFSET + 0x05);

void reconfigure_initMultiboot() {

    abstraction_setRegisterBitsLow(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    reconfigure_fpgaMultibootClearComplete_internal();
    fpgaDoneResponse = FPGA_DONE_NOTHING;

}

void reconfigure_fpgaMultiboot(uint32_t address) {

    elasticnode_fpgaPowerOn();

    enableXmem();

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal();

    //multiboot lokale Variable, anderer Name, volatile

    for (uint8_t i = 0; i < 3; i++)
    {
        *(multiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
    }

    disableXmem();

    //platform unabhängig, enable beabsichtigt?
    sei();
}
uint32_t reconfigure_getMultibootAddress() {
    uint32_t address = 0;
    for (uint8_t i = 0; i < 3; i++)
    {
        address = address + *(multiboot+i);
    }
    return address;
}
void reconfigure_interruptSR() {

    if (abstraction_getBit(PIN_FPGA_DONE, P_FPGA_DONE)){
        //float duration;
        reconfigure_fpgaSetDoneReponse_internal(1);
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
                reconfigure_fpgaMultiboot(0);
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