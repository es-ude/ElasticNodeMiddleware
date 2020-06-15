#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/debug/debug.h"

int main(void)
{

    uint16_t timer_1_sec=0;
    DDRD = 0xff;

    // init debug --> debugging with lufa via USB
    debugInit(NULL);

    while (true) {

        // Period task, blink the LED per 3 seconds
        if(timer_1_sec<=1500)
        {
            BitManipulation_setBit(&PORTD, PD4);
        }
        else if(timer_1_sec<=3000)
        {
            BitManipulation_clearBit(&PORTD, PD4);
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
