//
// Created by annika on 26.09.19.
//

#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"


volatile uint8_t *reset_fpga = (uint8_t*) (XMEM_OFFSET + 0x04);

void elasticnode_fpgaPowerOn_internal(){

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsHigh(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));

    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));

    //single control --> register do not exist

    //disable interface
    abstraction_setRegisterBitsLow(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}

void elasticnode_fpgaPowerOff_internal(){
    abstraction_setRegisterBitsLow(PORT_FPGA_PROGRAM_B, ( 1 << P_FPGA_PROGRAM_B));

    //enable interface
    abstraction_setRegisterBitsHigh(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_SRAM, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_SRAM, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_AUX, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_AUX, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_IO, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_IO, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_INT, ( 1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_INT, ( 1 << P_FPGA_POWER_INT));

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, ( 1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsLow(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}

void elasticnode_setFpgaSoftReset_internal(void)
{
    *reset_fpga = 0x1;
}

void elasticnode_clearFpgaSoftReset_internal() {
    *reset_fpga = 0x0;
}