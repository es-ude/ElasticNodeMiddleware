#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
//test if Bits are set
//4th, 5th, 6th and 7th LED on
int main() {
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test elasticNodeMiddleware_internal");
    uart_WaitUntilDone();

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
        uart_WriteLine("elasticnode_fpgaPowerOn_internal() successful");
        uart_WaitUntilDone();
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
        uart_WriteLine("elasticnode_fpgaPowerOff_internal() successful");
        uart_WaitUntilDone();
    }

    elasticnode_setFpgaHardReset_internal();
    if(BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
    !BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {
        BitManipulation_setBit(&PORTD, PD6);
        uart_WriteLine("elasticnode_setFpgaHardReset_internal() successful");
        uart_WaitUntilDone();
    }

    elasticnode_clearFpgaHardReset_internal();
    if(BitManipulation_bitIsSetOnArray(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B) &&
       !BitManipulation_bitIsSetOnArray(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B)) {
        BitManipulation_setBit(&PORTD, PD7);
        uart_WriteLine("elasticnode_clearFpgaHardReset_internal() successful");
        uart_WaitUntilDone();
    }
}


ISR(USART1_RX_vect) {
        uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
}