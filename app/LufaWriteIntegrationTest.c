#include <util/delay.h>
#include <stdbool.h>
#include "PeripheralInterface/LufaUsartImpl.h"

int
main(void)
{
    setUpUsbSerial();
    char text[] = "hello, world!\n";
    while (true)
    {
        _delay_ms(500);
        for (int i=0; i<14; i++)
        {
            lufaUsart_writeByte(text+i);
        }
    }
}