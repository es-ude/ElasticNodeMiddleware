#include "EmbeddedUtilities/BitManipulation.h"

#include "ElasticNodeMiddleware/ElasticNodeMiddlewareMonitoring.h"

#include "src/interruptManager/interruptManager.h"

#ifndef TEST

#include <compat/twi.h>
#include <avr/interrupt.h>

#endif

void elasticnode_monitoring_change_running_state(uint8_t new_state) {

    if (new_state <= 15) {
        BitManipulation_setByte(STATE_AND_SAMPLE_RATE_OF_THE_MCU, 0b00001111, new_state);
    }

    //uint8_t tmp_samplerate = *state_of_the_mcu >> 4;
    //*state_of_the_mcu = (new_state & 0b00001111);
    //*state_of_the_mcu |= tmp_samplerate << 4;
}

void elasticnode_monitoring_change_sample_rate(uint8_t new_sample_rate) {

    if (new_sample_rate <= 15) {

        new_sample_rate = new_sample_rate << 4;

        BitManipulation_setByte(STATE_AND_SAMPLE_RATE_OF_THE_MCU, 0b11110000, new_sample_rate);
    }

    //uint8_t tmp_state = state_of_the_mcu << 4;
    //state_of_the_mcu = ((new_sample_rate << 4) & 0b11110000);
    //state_of_the_mcu |= tmp_state >> 4;
}

void IIC_slave_init(uint8_t address) {
    interruptManager_clearInterrupt();
    //cli();

    //load address into TWI address register
    *TWAR_MON = address;
    //TWAR = address;

    BitManipulation_setBit(TWCR_MON, TWIE_MON);
    BitManipulation_setBit(TWCR_MON, TWEA_MON);
    BitManipulation_setBit(TWCR_MON, TWINT_MON);
    BitManipulation_setBit(TWCR_MON, TWEN_MON);
    //TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);

    interruptManager_setInterrupt();
    //sei();
}

void elasticnode_monitoring_ISR(void) {

    /*switch (TW_STATUS) {
        case TW_SR_SLA_ACK:
        case TW_SR_DATA_ACK:
            // We have received data. This is now contained in the TWI data register (TWDR)
            // i2cEventReceived(TWDR);
            break;
        case TW_ST_SLA_ACK:
        case TW_ST_DATA_ACK:
            // Either slave selected (SLA_ACK) and data requested or data transmitted, ACK received and next data requested
            *TWDR_MON = state_of_the_mcu;
            break;
        default:
            break;

    }*/

    if (TW_STATUS_MON == TW_ST_SLA_ACK_MON || TW_STATUS_MON == TW_ST_DATA_ACK_MON) {
        *TWDR_MON = *STATE_AND_SAMPLE_RATE_OF_THE_MCU;
    }

    BitManipulation_setBit(TWCR_MON, TWIE_MON);
    BitManipulation_setBit(TWCR_MON, TWEA_MON);
    BitManipulation_setBit(TWCR_MON, TWINT_MON);
    BitManipulation_setBit(TWCR_MON, TWEN_MON);
    //TWCR = (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
}
