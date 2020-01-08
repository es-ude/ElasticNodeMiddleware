//
// Created by annika on 18.12.19.
//

#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"

//test if Bits are set
//4th and 5th LED on
int main() {
    DDRD = 0xff;
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
    }
}