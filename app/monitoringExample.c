#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <compat/twi.h>

#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "ElasticNodeMiddleware/energyMonitoringInterface.h"

uint8_t state_of_the_mcu;

typedef enum {
    SLEEP_MODE = 0,
    LED_0 = 1,
    LED_0_1 = 2,
    LED_0_1_2 = 3,
    LED_0_1_2_3 = 4,
    END_MEASUREMENT = 15,
} mcu_running_state;

int main(void) {
    IIC_slave_init(MY_IIC_ADDRESS);

    change_running_state(SLEEP_MODE, &state_of_the_mcu);

    elasticnode_led_mcu_init();

    while (true) {

        //start delay
        _delay_ms(2000);

        change_sample_rate(CURRENT_SAMPLE_TIME_40ms);

        elasticnode_led_mcu_turnOn(0);
        change_running_state(LED_0, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOn(1);
        change_running_state(LED_0_1, &state_of_the_mcu);
        _delay_ms(1000);

        change_sample_rate(CURRENT_SAMPLE_TIME_10ms);

        elasticnode_led_mcu_turnOn(2);
        change_running_state(LED_0_1_2, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOn(3);
        change_running_state(LED_0_1_2_3, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOffAll();
        change_running_state(SLEEP_MODE, &state_of_the_mcu);
        change_sample_rate(CURRENT_SAMPLE_TIME_5ms);

    }

    change_running_state(END_MEASUREMENT, &state_of_the_mcu);

    return 0;
}
