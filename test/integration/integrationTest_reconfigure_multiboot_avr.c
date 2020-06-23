#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include <util/delay.h>
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
//test if Bits are set + testing multiboot
//5th LED blink
int main() {
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test reconfigure multiboot");
    uart_WaitUntilDone();

    //variable initialisation tested in unit tests
    reconfigure_initMultiboot();

    //testing multiboot --> case 'X'
    BitManipulation_setBit(&PORTD, PD5);

    reconfigure_fpgaMultiboot(0);
    uart_WriteLine("reconfigure_fpgaMultiboot() tested");
    uart_WaitUntilDone();

    _delay_ms(500);
    BitManipulation_clearBit(&PORTD, PD5);

}


ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}
ISR(FPGA_DONE_INT_VECTOR)
{
    reconfigure_interruptSR();
    uart_WriteLine("interrupt service routine successful");
}