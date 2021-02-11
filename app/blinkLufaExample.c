#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "src/debug/debug.h"

int main(void)
{

    uint16_t timer_1_sec=0;
    led_mcu_init();

    // init debug --> debugging with lufa via USB
    debugInit(NULL);

    while (true) {

        // Period task, blink the LED per 3 seconds
        if(timer_1_sec<=1500)
        {
            led_mcu_turnOn(0);
        }
        else if(timer_1_sec<=3000)
        {
            led_mcu_turnOff(0);
        }else
        {
            timer_1_sec=0;
            // once 3 seconds reached, send hello.
            debugWriteLine("Hello. You debug with Lufa.");
        }

        // receive buffer check, only check if the recv
        // buffer is not empty.
        if(debugReadCharAvailable())
        {
            // receive buffer is empty, so read out 1 byte
            char read_char = debugGetChar();
            if (read_char=='a') // check if the key pressed is a
            {
                debugWriteLine("I know you pressed key a.");
            }
            else
            {
                debugWriteLine("Please press the a on your keyboard.");
            }
        }

        // Time base for period task
        _delay_ms(1);
        timer_1_sec++;

        // Lufa process, read and send happen in this process
        debugTask();
    }


    return 0;
}
