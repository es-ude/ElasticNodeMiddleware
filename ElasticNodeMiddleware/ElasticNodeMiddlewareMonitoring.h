#ifndef SUB_SYSTEM_H
#define SUB_SYSTEM_H

#define BITMASK_RUNNING_STATE 0b00001111
#define BITMASK_SAMPLE_RATE 0b11110000

#define MY_IIC_ADDRESS 64

//TODO: When to define
//#define TEMPORAL_ACCELERATOR 1

#include <stdint.h>

#ifdef TEST
extern uint8_t* TWAR_MON;
extern uint8_t* TWCR_MON;
extern uint8_t* TWDR_MON;
extern uint8_t* STATE_AND_SAMPLE_RATE_OF_THE_MCU;

#define TW_STATUS_MON 5

#define TWIE_MON 1
#define TWEA_MON 2
#define TWINT_MON 3
#define TWEN_MON 4

#define TW_ST_SLA_ACK_MON 5
#define TW_ST_DATA_ACK_MON 5

#else

uint8_t state_and_sample_rate_of_the_mcu;

#define TWAR_MON &TWAR
#define TWCR_MON &TWCR
#define TWDR_MON &TWDR
#define TW_STATUS_MON TW_STATUS

#define TWIE_MON TWIE
#define TWEA_MON TWEA
#define TWINT_MON TWINT
#define TWEN_MON TWEN

#define TW_ST_SLA_ACK_MON TW_ST_SLA_ACK
#define TW_ST_DATA_ACK_MON TW_ST_DATA_ACK

#define STATE_AND_SAMPLE_RATE_OF_THE_MCU &state_and_sample_rate_of_the_mcu

#endif

typedef enum {
    CURRENT_SAMPLE_TIME_2ms5,
    CURRENT_SAMPLE_TIME_5ms,
    CURRENT_SAMPLE_TIME_10ms,
    CURRENT_SAMPLE_TIME_20ms,
    CURRENT_SAMPLE_TIME_40ms,
    CURRENT_SAMPLE_TIME_80ms,
    CURRENT_SAMPLE_TIME_160ms,
    CURRENT_SAMPLE_TIME_320ms,
    CURRENT_SAMPLE_TIME_DEFAULT = 5
} elasticnode_monitoring_sample_rate;

void elasticnode_monitoring_change_running_state(uint8_t new_state);

void elasticnode_monitoring_change_sample_rate(elasticnode_monitoring_sample_rate new_sample_rate);

void IIC_slave_init(uint8_t address);

void elasticnode_monitoring_ISR(void);

#endif
