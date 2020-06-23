#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/debug/debug.h"
#include "lib/uart/uart.h"

/* for using uart
*/
ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}

int main(void)
{
    uint16_t timer_1_sec=0;
    DDRD = 0xff;

    // init debug --> debugging with uart via ftdi adapter
    debugInit(NULL);

    while (true) {

        // Period task
        _delay_ms(500);
        BitManipulation_setBit(&PORTD, PD4);
        debugWriteLine("Hello. You debug with Uart.");
        _delay_ms(500);

        // check uart flag
        if(debugReadCharAvailable()) {
            uint8_t data = debugGetChar();

            // check if the key pressed is a
            if(data == 'a') {
                debugWriteLine("I know you pressed key a.");
                BitManipulation_setBit(&PORTD, PD5);
                _delay_ms(500);
            } else {
                debugWriteLine("Please press the a on your keyboard.");
                BitManipulation_setBit(&PORTD, PD6);
                _delay_ms(500);
            }

            // set uart flag back to 0
            debugReadCharProcessed();
        }

        // turn off leds
        _delay_ms(500);
        BitManipulation_clearBit(&PORTD, PD4);
        BitManipulation_clearBit(&PORTD, PD5);
        BitManipulation_clearBit(&PORTD, PD6);
    }


    return 0;
}
