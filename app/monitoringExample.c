////*******************************************************************//////////////////
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "src/debug/debug.h"
#include "src/led/led_mcu.h"
#include "src/sub-system/sub-system.h"

#include <compat/twi.h>

void IIC_slave_init(uint8_t address);
uint8_t system_state = 0;
extern uint8_t state_of_the_mcu;

int main(void)
{
    IIC_slave_init(MY_IIC_ADDRESS);
    uint16_t timer_1_sec=0;
    led_mcu_init();
    state_of_the_mcu = 0;

    // init debug --> debugging with lufa via USB
    debugInit(NULL);

    while (true) {

        _delay_ms(10000);

        led_mcu_turnOn(0);
        change_state(RECONFIG_1, &state_of_the_mcu);
        change_sample_rate(0,&state_of_the_mcu);
        _delay_ms(1000);
        led_mcu_turnOn(1);
        //state_of_the_mcu = 2;

        change_state(SEND_INPUT_1, &state_of_the_mcu);
        change_sample_rate(4, &state_of_the_mcu);
        _delay_ms(500);
        led_mcu_turnOn(2);
        //state_of_the_mcu = 3;

        change_state(EXECUTION_1, &state_of_the_mcu);
        change_sample_rate(0, &state_of_the_mcu);
        _delay_ms(1000);
        led_mcu_turnOn(3);
        //state_of_the_mcu = 4;

        change_state(RECEIVE_RES_1, &state_of_the_mcu);
        change_sample_rate(0, &state_of_the_mcu);
        _delay_ms(1000);
        led_mcu_turnOffAll();
        //state_of_the_mcu = 0;

        change_state(SLEEP_MODE, &state_of_the_mcu);
        change_sample_rate(1,&state_of_the_mcu);
        _delay_ms(1000);
    }


    return 0;
}

void IIC_slave_init(uint8_t address){
    cli();
    //load address into TWI address register
    TWAR = address;
    TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
    sei();
}