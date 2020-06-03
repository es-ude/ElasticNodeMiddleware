#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>
#include "PeripheralInterface/LufaUsartImpl.h"
#include "EmbeddedUtilities/BitManipulation.h"

int
main(void)
{
    uint16_t timer_1_sec=0;
    char text[] = "hello, world!\r\n";

    DDRD = 0xff;

    initLufa();                   // Initialize LUFA
    while(!lufaOutputAvailable()) // Blocked until LUFA available
    {
        _delay_ms(100);
    }

    while (true)
    {

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
            lufaWriteString(text);
        }

        // receive buffer check, only check if the recv
        // buffer is not empty.
        if(lufaReadAvailable())
        {
            // receive buffer is empty, so read out 1 byte
            char read_char = lufaGetChar();
            if (read_char=='a') // check if the key pressed is a
            {
                lufaWriteString("I know you pressed key a.\r\n");
            }
            else
            {
                lufaWriteString("\r\nPlease press the a on your keyboard.\r\n");
            }
        }

        // Time base for period task
        _delay_ms(1);
        timer_1_sec++;

        // Lufa process, read and send happen in this process
        lufaTask();

    }
}
