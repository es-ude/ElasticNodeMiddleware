#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include <compat/twi.h>

#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
#include "ElasticNodeMiddleware/ElasticNodeMiddlewareMonitoring.h"

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

    elasticnode_monitoring_change_running_state(SLEEP_MODE, &state_of_the_mcu);

    elasticnode_led_mcu_init();

    while (true) {

        //start delay
        _delay_ms(2000);

        elasticnode_monitoring_change_sample_rate(CURRENT_SAMPLE_TIME_40ms);

        elasticnode_led_mcu_turnOn(0);
        elasticnode_monitoring_change_running_state(LED_0, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOn(1);
        elasticnode_monitoring_change_running_state(LED_0_1, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_monitoring_change_sample_rate(CURRENT_SAMPLE_TIME_10ms);

        elasticnode_led_mcu_turnOn(2);
        elasticnode_monitoring_change_running_state(LED_0_1_2, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOn(3);
        elasticnode_monitoring_change_running_state(LED_0_1_2_3, &state_of_the_mcu);
        _delay_ms(1000);

        elasticnode_led_mcu_turnOffAll();
        elasticnode_monitoring_change_running_state(SLEEP_MODE, &state_of_the_mcu);
        elasticnode_monitoring_change_sample_rate(CURRENT_SAMPLE_TIME_5ms);

    }

    elasticnode_monitoring_change_running_state(END_MEASUREMENT, &state_of_the_mcu);

    return 0;
}

ISR(TWI_vect) {
        elasticnode_monitoring_ISR();
}
