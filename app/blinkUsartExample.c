#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "lib/debug/debug.h"
#include "lib/led/led_mcu.h"
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
    led_mcu_init();

    // init debug --> debugging with uart via ftdi adapter
    debugInit(NULL);

    while (true) {

        // Period task
        _delay_ms(500);
        led_mcu_turnOn(0);
        debugWriteLine("Hello. You debug with Uart.");
        _delay_ms(500);

        // check uart flag
        if(debugReadCharAvailable()) {
            uint8_t data = debugGetChar();

            // check if the key pressed is a
            if(data == 'a') {
                debugWriteLine("I know you pressed key a.");
                led_mcu_turnOn(1);
                _delay_ms(500);
            } else {
                debugWriteLine("Please press the a on your keyboard.");
                led_mcu_turnOn(2);
                _delay_ms(500);
            }

        }

        // set uart flag back to 0
        debugReadCharProcessed();
        // turn off leds
        _delay_ms(500);
        led_mcu_turnOff(0);
        led_mcu_turnOff(1);
        led_mcu_turnOff(2);
    }


    return 0;
}
