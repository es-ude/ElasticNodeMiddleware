#include "EmbeddedUtilities/BitManipulation.h"

#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/xmem/xmem.h"

volatile uint8_t *fpgaResetOffset = (uint8_t *) (XMEM_OFFSET + 0x04);

void elasticnode_fpgaPowerOn_internal(void) {

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_setBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);

    BitManipulation_setBit(DDR_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM);
    BitManipulation_clearBit(PORT_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM);
    BitManipulation_setBit(DDR_FPGA_POWER_AUX, P_FPGA_POWER_AUX);
    BitManipulation_clearBit(PORT_FPGA_POWER_AUX, P_FPGA_POWER_AUX);
    BitManipulation_setBit(DDR_FPGA_POWER_IO, P_FPGA_POWER_IO);
    BitManipulation_clearBit(PORT_FPGA_POWER_IO, P_FPGA_POWER_IO);
    BitManipulation_setBit(DDR_FPGA_POWER_INT, P_FPGA_POWER_INT);
    BitManipulation_clearBit(PORT_FPGA_POWER_INT, P_FPGA_POWER_INT);

    //single control --> register do not exist

    //disable interface
    BitManipulation_clearBit(DDR_FPGA_CCLK, P_FPGA_CCLK);

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
}

void elasticnode_fpgaPowerOff_internal(void) {
    BitManipulation_clearBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);

    //enable interface
    BitManipulation_setBit(DDR_FPGA_CCLK, P_FPGA_CCLK);

    BitManipulation_setBit(DDR_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM);
    BitManipulation_setBit(PORT_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM);
    BitManipulation_setBit(DDR_FPGA_POWER_AUX, P_FPGA_POWER_AUX);
    BitManipulation_setBit(PORT_FPGA_POWER_AUX, P_FPGA_POWER_AUX);
    BitManipulation_setBit(DDR_FPGA_POWER_IO, P_FPGA_POWER_IO);
    BitManipulation_setBit(PORT_FPGA_POWER_IO, P_FPGA_POWER_IO);
    BitManipulation_setBit(DDR_FPGA_POWER_INT, P_FPGA_POWER_INT);
    BitManipulation_setBit(PORT_FPGA_POWER_INT, P_FPGA_POWER_INT);

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
}


void elasticnode_setFpgaSoftReset_internal(void) {
    *fpgaResetOffset = 0x1;
}

void elasticnode_clearFpgaSoftReset_internal(void) {
    *fpgaResetOffset = 0x0;
}

void elasticnode_setFpgaHardReset_internal(void) {
    BitManipulation_setBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
}

void elasticnode_clearFpgaHardReset_internal(void) {
    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
}
