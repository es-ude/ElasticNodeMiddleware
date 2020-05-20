#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/debug/debug.h"
#include "lib/uart/uart.h"

//the following ISR's have to be comment in by programmer

/* for using uart
*/
ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}
/* for using reconfigure
*/
/*ISR(FPGA_DONE_INT_VECTOR) {
    reconfigure_interruptSR();
}*/

int main(void)
{

    DDRD = 0xff;

    debugInit(NULL);

    while (true) {

        _delay_ms(500);
        if(debugReadCharAvailable()) {
            BitManipulation_setBit(&PORTD, PD4);
            _delay_ms(500);
            uint8_t data = debugGetChar();

            if(data == '1') {
                debugWriteChar(data);
                debugWriteLine("input = 1");
                BitManipulation_setBit(&PORTD, PD5);
                _delay_ms(500);
            }
            if(data != '1') {
                debugWriteChar(data);
                debugWriteLine("input != 1");
                BitManipulation_setBit(&PORTD, PD6);
                _delay_ms(500);
            }


        }
        _delay_ms(500);
        BitManipulation_clearBit(&PORTD, PD4);
        BitManipulation_clearBit(&PORTD, PD5);
        BitManipulation_clearBit(&PORTD, PD6);

    }


    return 0;
}
