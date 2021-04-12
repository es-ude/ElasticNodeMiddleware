#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include <avr/interrupt.h>

#include "src/debug/debug.h"

//test if Bits are set
//4th, 5th, 6th and 7th LED on
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test elasticNodeMiddleware_internal");

    elasticnode_fpgaPowerOn_internal();
    if (BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM) &&
        !BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_AUX, P_FPGA_POWER_AUX) &&
        !BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_AUX, P_FPGA_POWER_AUX) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_IO, P_FPGA_POWER_IO) &&
        !BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_IO, P_FPGA_POWER_IO) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_INT, P_FPGA_POWER_INT) &&
        !BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_INT, P_FPGA_POWER_INT) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_CCLK, P_FPGA_CCLK) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {

        BitManipulation_setBit(&PORTD, PD4);
        debugWriteLine("elasticnode_fpgaPowerOn_internal() successful");
    } else {
        debugWriteLine("elasticnode_fpgaPowerOn_internal() NOT successful");
    }

    elasticnode_fpgaPowerOff_internal();
    if (BitManipulation_bitIsSetOnArray(DDR_FPGA_CCLK, P_FPGA_CCLK) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_SRAM, P_FPGA_POWER_SRAM) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_AUX, P_FPGA_POWER_AUX) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_AUX, P_FPGA_POWER_AUX) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_IO, P_FPGA_POWER_IO) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_IO, P_FPGA_POWER_IO) &&
        BitManipulation_bitIsSetOnArray(DDR_FPGA_POWER_INT, P_FPGA_POWER_INT) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_POWER_INT, P_FPGA_POWER_INT) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {

        //clearBit PORT_FPGA_PROGRAM_B not important, because after that the Port is set
        BitManipulation_setBit(&PORTD, PD5);
        debugWriteLine("elasticnode_fpgaPowerOff_internal() successful");
    } else {
        debugWriteLine("elasticnode_fpgaPowerOff_internal() NOT successful");
    }

    elasticnode_setFpgaHardReset_internal();
    if (BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        !BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {
        BitManipulation_setBit(&PORTD, PD6);
        debugWriteLine("elasticnode_setFpgaHardReset_internal() successful");
    } else {
        debugWriteLine("elasticnode_setFpgaHardReset_internal() NOT successful");
    }

    elasticnode_clearFpgaHardReset_internal();
    if (BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {
        BitManipulation_setBit(&PORTD, PD7);
        debugWriteLine("elasticnode_clearFpgaHardReset_internal() successful");
    } else {
        debugWriteLine("elasticnode_clearFpgaHardReset_internal() NOT successful");
    }

    debugWriteLine("integration test elasticNodeMiddleware_internal finished");
}
