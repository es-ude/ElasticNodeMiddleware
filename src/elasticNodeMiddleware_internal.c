//
// Created by annika on 26.09.19.
//

#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"


volatile uint8_t *reset_fpga = (uint8_t*) (XMEM_OFFSET + 0x04);

void elasticnode_fpgaPowerOn_internal(){

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
    BitManipulation_setBit(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));

    BitManipulation_setBit(DDR_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    BitManipulation_clearBit(PORT_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    BitManipulation_setBit(DDR_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    BitManipulation_clearBit(PORT_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    BitManipulation_setBit(DDR_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    BitManipulation_clearBit(PORT_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    BitManipulation_setBit(DDR_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));
    BitManipulation_clearBit(PORT_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));

    //single control --> register do not exist

    //disable interface
    BitManipulation_clearBit(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}

void elasticnode_fpgaPowerOff_internal(){
    BitManipulation_clearBit(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));

    //enable interface
    BitManipulation_setBit(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    BitManipulation_setBit(DDR_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    BitManipulation_setBit(PORT_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    BitManipulation_setBit(DDR_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    BitManipulation_setBit(PORT_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    BitManipulation_setBit(DDR_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    BitManipulation_setBit(PORT_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    BitManipulation_setBit(DDR_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));
    BitManipulation_setBit(PORT_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
    BitManipulation_clearBit(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}

void elasticnode_setFpgaSoftReset_internal(void)
{
    *reset_fpga = 0x1;
}

void elasticnode_clearFpgaSoftReset_internal() {
    *reset_fpga = 0x0;
}