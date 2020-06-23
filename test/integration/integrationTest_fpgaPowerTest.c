//case P
//fpgaPowerTest(10)

#include <stdint.h>
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/uart/uart.h"
#include <avr/interrupt.h>

//4th LED should blink 10 times
int main() {
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test fpgaPowerTest");
    uart_WriteLine("the fpga should power on 10 times");
    uart_WaitUntilDone();

    for(uint8_t i = 0; i < 10; i++) {
        _delay_ms(1000);
        BitManipulation_setBit(&PORTD, PD4);
        elasticnode_fpgaPowerOff();
        uart_WriteLine("fpga power off ");
        //uart_WriteChar((char)i);
        uart_WaitUntilDone();

        _delay_ms(1000);
        BitManipulation_clearBit(&PORTD, PD4);
        elasticnode_fpgaPowerOn();
        uart_WriteLine("fpga power on");
        uart_WaitUntilDone();
    }
}

ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}