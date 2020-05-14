#include <util/delay.h>
#include <stdbool.h>
#include "PeripheralInterface/LufaUsartImpl.h"

int
main(void)
{
    setUpUsbSerial();
    while (true)
    {
        _delay_ms(500);
        lufaUsart_writeByte(1);
        _delay_ms(500);
        lufaUsart_writeByte('w');
        lufaUsart_writeByte('\n');
    }
}