//
// Created by annika on 18.12.19.
//

#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
//test if Bits are set
//4th LED on
int main() {
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test elasticnodemiddleware");
    uart_WaitUntilDone();

    elasticnode_initialise();

    if(BitManipulation_bitIsSetOnArray(DDR_FPGA_CCLK, P_FPGA_CCLK) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_INIT_B, P_FPGA_INIT_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_INIT_B, P_FPGA_INIT_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_DONE, P_FPGA_DONE) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_DONE, P_FPGA_DONE) &&
    BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
    !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {

        BitManipulation_setBit(&PORTD, PD4);
        uart_WriteLine("elasticnode_initialise() successful");
        uart_WaitUntilDone();
    }

}


ISR(USART1_RX_vect) {
        uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
}