#include <stdio.h>
#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "src/uart/uart.h"
#include <avr/interrupt.h>

//4th LED blinks 2x
int main() {
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test cold boot");
    uart_WaitUntilDone();

    // cold boot experiment --> case 'c'

    BitManipulation_setBit(&PORTD, PD4);

    uart_WriteLine("FPGA off");
    uart_WaitUntilDone();
    elasticnode_fpgaPowerOff();
    _delay_ms(100);
    uart_WriteLine("1");

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_MULTIBOOT);

    //startExperiment(1, 0);
    //not implemented
    //timer_start(1);

    _delay_ms(500);
    BitManipulation_setBit(&PORTD, PD4);

    elasticnode_fpgaPowerOn();

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD4);
    // response in fpga done isr?
}


ISR(USART1_RX_vect) {
        uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
}