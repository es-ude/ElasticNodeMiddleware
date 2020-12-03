#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "src/xmem/xmem.h"
#include "src/interruptManager/interruptManager.h"

#include <util/delay.h>

//#include "EmbeddedUtilities/BitManipulation.h"

volatile uint8_t fpgaDoneResponse = FPGA_DONE_NOTHING;
volatile uint8_t *AddressMultiboot = (uint8_t *) (XMEM_OFFSET + 0x05);

void reconfigure_initMultiboot(void) {

    FPGA_DONE_INT_REG &= ~FPGA_DONE_INT;
    FPGA_DONE_INT_REG |= (1<<FPGA_DONE_INT);
    FPGA_DONE_INT_CONTROL_REG |= (1<<FPGA_DONE_INT_CONTROL);

    reconfigure_fpgaMultibootClearComplete_internal();
    fpgaDoneResponse = FPGA_DONE_NOTHING;
}

void reconfigure_fpgaMultiboot(uint32_t address) {

    elasticnode_fpgaPowerOn_internal();
    xmem_enableXmem();
    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal();

    for (uint8_t i = 0; i < 3; i++)
    {
        *(AddressMultiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
    }

    xmem_disableXmem();

    //enable beabsichtigt?
    interruptManager_setInterrupt();
}

uint32_t reconfigure_getMultibootAddress(void) {
    return (uint32_t ) (*(AddressMultiboot) + *(AddressMultiboot+1) + *(AddressMultiboot +2));
}

uint8_t reconfigure_fpgaMultibootComplete(void) {
    return reconfigure_fpgaMultibootComplete_internal();
}

void reconfigure_interruptSR(void) {

    if((*PIN_FPGA_DONE & (1<<P_FPGA_DONE)) != 0) {
        //float duration;
        reconfigure_fpgaSetDoneReponse_internal(1);
        switch (fpgaDoneResponse) {
            case FPGA_DONE_PRINT:
                interruptManager_clearInterrupt();
                break;
            case FPGA_DONE_MULTIBOOT:
                elasticnode_setFpgaSoftReset_internal();
                _delay_ms(RESET_DELAY);
                elasticnode_clearFpgaSoftReset_internal();
                reconfigure_fpgaMultiboot(0);
                break;
            case 0:
            default:
                break;
        }
        interruptManager_setInterrupt();
    }
}
