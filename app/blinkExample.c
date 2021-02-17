#include <util/delay.h>
#include <stdbool.h>

#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"

int main(void) {

    led_mcu_init();
    led_mcu_turnOnAll();
    _delay_ms(500);
    led_mcu_turnOffAll();
    _delay_ms(500);

    while (true) {
        led_mcu_turnOn(0);
        _delay_ms(500);
        led_mcu_turnOn(1);
        _delay_ms(500);
        led_mcu_turnOn(2);
        _delay_ms(500);
        led_mcu_turnOn(3);
        _delay_ms(500);

        led_mcu_turnOff(0);
        _delay_ms(500);
        led_mcu_turnOff(1);
        _delay_ms(500);
        led_mcu_turnOff(2);
        _delay_ms(500);
        led_mcu_turnOff(3);
        _delay_ms(500);

    }
}