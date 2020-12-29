////*******************************************************************//////////////////
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "src/led/led_mcu.h"
#include "src/sub-system/sub-system.h"

#include <compat/twi.h>

uint8_t system_state = 0;
int main(void)
{
    IIC_slave_init(MY_IIC_ADDRESS);

    led_mcu_init();

    while (true) {

        //start delay
        _delay_ms(5000);

        led_mcu_turnOn(0);
        change_sample_rate(CURRENT_SAMPLE_TIME_2ms5);
        change_running_state(RECONFIG_1);
        _delay_ms(1000);


        led_mcu_turnOn(1);
        change_sample_rate(CURRENT_SAMPLE_TIME_10ms);
        change_running_state(SEND_INPUT_1);
        _delay_ms(1000);

        led_mcu_turnOn(2);
        change_sample_rate(CURRENT_SAMPLE_TIME_80ms);
        change_running_state(EXECUTION_1);
        _delay_ms(1000);

        led_mcu_turnOn(3);
        change_sample_rate(CURRENT_SAMPLE_TIME_160ms);
        change_running_state(RECEIVE_RES_1);
        _delay_ms(1000);

        led_mcu_turnOffAll();
        change_running_state(SLEEP_MODE);
        change_sample_rate(1);

    }

    return 0;
}
