#include "ElasticNodeMiddleware/ElasticNodeMiddlewareMonitoring.h"

#include <stdio.h>
#include <stdint.h>
#include <compat/twi.h>
#include <avr/interrupt.h>

uint8_t state_of_the_mcu;

#if TEMPORAL_ACCELERATOR
void elasticnode_monitoring_change_running_state(mcu_running_state new_state){
    //to ensure that only values from 0 to 16 are allowed and otherwise the sample rate isnt ruined

    uint8_t tmp_samplerate = state_of_the_mcu >> 4;
    state_of_the_mcu = (new_state & 0b00001111);
    state_of_the_mcu |= tmp_samplerate << 4;
}
#else

void elasticnode_monitoring_change_running_state(uint8_t new_state, uint8_t *state_of_the_mcu) {
    //to ensure that only values from 0 to 16 are allowed and otherwise the sample rate isnt ruined

    uint8_t tmp_samplerate = *state_of_the_mcu >> 4;
    *state_of_the_mcu = (new_state & 0b00001111);
    *state_of_the_mcu |= tmp_samplerate << 4;
}

#endif

void elasticnode_monitoring_change_sample_rate(elasticnode_monitoring_sample_rate new_sample_rate) {

    uint8_t tmp_state = state_of_the_mcu << 4;
    state_of_the_mcu = ((new_sample_rate << 4) & 0b11110000);
    state_of_the_mcu |= tmp_state >> 4;
}

void IIC_slave_init(uint8_t address) {
    cli();
    //load address into TWI address register
    TWAR = address;
    TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
    sei();
}

// TODO: in main.c
ISR(TWI_vect){
        switch (TW_STATUS)
        {
            case TW_SR_SLA_ACK:
            case TW_SR_DATA_ACK:
                /*
                        We have received data. This is now contained in the TWI
                        data register (TWDR)
                    */
                //          i2cEventReceived(TWDR);
                break;
            case TW_ST_SLA_ACK:
            case TW_ST_DATA_ACK:
                /*
                            Either slave selected (SLA_ACK) and data requested or data transmitted, ACK received
                            and next data requested
                */
                TWDR = state_of_the_mcu;
            break;
            default:
                break;
        }
        TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}
