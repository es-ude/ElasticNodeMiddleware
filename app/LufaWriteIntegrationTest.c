#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>
#include "PeripheralInterface/LufaUsartImpl.h"
#include "EmbeddedUtilities/BitManipulation.h"

int
main(void)
{
    DDRD = 0xff;
    setUpUsbSerial();
    char text[] = "hello, world!\n";
    while (true)
    {
        _delay_ms(500);
        BitManipulation_setBit(&PORTD, PD4);
        _delay_ms(500);
        BitManipulation_clearBit(&PORTD, PD4);
        for (int i=0; i<14; i++)
        {
            lufaUsart_writeByte(text+i);
        }
    }
}
