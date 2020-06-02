#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>
#include "PeripheralInterface/LufaUsartImpl.h"
#include "EmbeddedUtilities/BitManipulation.h"

int
main(void)
{
    uint16_t timer_1_sec=0;
    DDRD = 0xff;

    initLufa();
    while(!lufaOutputAvailable())
    {
        _delay_ms(100);
    }

    char text[] = "hello, world!\r\n";
    while (true)
    {
        _delay_ms(1);
        timer_1_sec++;
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
            lufaWriteString(text);
        }
        lufaWaitUntilDone();
        if(lufaReadAvailable())
        {
            char read_char = lufaGetChar();
            if (read_char=='a')
            {
                lufaWriteString("I know you pressed key a.\r\n");
            }
        }
    }
}
