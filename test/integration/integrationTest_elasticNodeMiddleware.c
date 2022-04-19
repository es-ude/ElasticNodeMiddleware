#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include <avr/interrupt.h>

#include "src/debug/debug.h"

//test if Bits are set
//4th LED on
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test elasticnodemiddleware");

    elasticnode_initialise();

    if (BitManipulation_bitIsSetOnArray(DDR_FPGA_CCLK, P_FPGA_CCLK) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_INIT_B, P_FPGA_INIT_B) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_INIT_B, P_FPGA_INIT_B) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_DONE, P_FPGA_DONE) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_DONE, P_FPGA_DONE) &&
        BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
        !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {

        BitManipulation_setBit(&PORTD, PD4);
        debugWriteLine("elasticnode_initialise() successful");
    } else {
        debugWriteLine("elasticnode_initialise() NOT successful");
    }

}
