//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot_avr.h"
#include "elasticnodemiddleware/reconfigure_multiboot_internal_avr.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/interruptManager.h"
#include "EmbeddedUtilities/BitManipulation.h"

volatile uint8_t fpgaDoneResponse = FPGA_DONE_NOTHING;
volatile uint8_t *AddressMultiboot = (uint8_t *) (XMEM_OFFSET + 0x05);

void reconfigure_initMultiboot() {

    BitManipulation_clearBit(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    BitManipulation_setBit(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    BitManipulation_setBit(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    reconfigure_fpgaMultibootClearComplete_internal();
    fpgaDoneResponse = FPGA_DONE_NOTHING;

}

void reconfigure_fpgaMultiboot(uint32_t address) {

    elasticnode_fpgaPowerOn();

    xmem_enableXmem();

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal();

    for (uint8_t i = 0; i < 3; i++)
    {
        *(AddressMultiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
    }

    xmem_disableXmem();

    //platform unabhängig, enable beabsichtigt?
    interruptManager_setInterrupt();
}

//new
uint32_t reconfigure_getMultibootAddress() {
    return (uint32_t ) (*(AddressMultiboot) + *(AddressMultiboot+1) + *(AddressMultiboot +2));
}

//new
uint8_t reconfigure_fpgaMultibootComplete() {
    return reconfigure_fpgaMultibootComplete_internal();
}

void reconfigure_interruptSR() {

    if ((abstraction_getBit(PIN_FPGA_DONE, P_FPGA_DONE)) != 0) {
        //float duration;
        reconfigure_fpgaSetDoneReponse_internal(1);
        switch (fpgaDoneResponse) {
            case FPGA_DONE_PRINT:
                //duration = -1;
                interruptManager_clearInterrupt();

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
        interruptManager_setInterrupt();
    }
}

ISR(FPGA_DONE_INT_VECTOR)
{
    reconfigure_interruptSR();
}