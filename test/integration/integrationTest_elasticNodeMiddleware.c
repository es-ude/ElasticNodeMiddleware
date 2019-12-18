//
// Created by annika on 18.12.19.
//

#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"


//4th LED on
int main() {
    DDRD = 0xff;
    //elasticNode_fpgaPowerOn_internal in elasticnode_initialise--> tested in other integration test
    elasticnode_initialise();

    if(BitManipulation_bitIsSetOnArray(DDR_FPGA_CCLK, P_FPGA_CCLK) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_INIT_B, P_FPGA_INIT_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_INIT_B, P_FPGA_INIT_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_DONE, P_FPGA_DONE) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_DONE, P_FPGA_DONE) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {

        BitManipulation_setBit(&PORTD, PD4);
    }

}