#ifndef SUB_SYSTEM_H
#define SUB_SYSTEM_H

#define BITMASK_RUNNING_STATE 0b00001111
#define BITMASK_SAMPLE_RATE 0b11110000

#define MY_IIC_ADDRESS 64

//TODO: When to define
//#define TEMPORAL_ACCELERATOR 1

#include <stdint.h>

//uint8_t state_of_the_mcu;
//uint8_t sample_rate;

#if TEMPORAL_ACCELERATOR
typedef enum {
    SLEEP_MODE = 0,
    RECONFIG_1 = 1,
    SEND_INPUT_1 = 2,
    EXECUTION_1 = 3,
    RECEIVE_RES_1 = 4,
    RECONFIG_2 = 5,
    SEND_INPUT_2 = 6,
    EXECUTION_2 = 7,
    RECEIVE_RES_2 = 8,
    RECONFIG_3 = 9,
    SEND_INPUT_3 = 10,
    EXECUTION_3 = 11,
    RECEIVE_RES_3 = 12,
    FREE_TO_CHANGE1 = 13,
    FREE_TO_CHANGE2 = 14,
    END_MEASUREMENT = 15,

}mcu_running_state;
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
} sample_rate;

#if TEMPORAL_ACCELERATOR
void change_running_state(mcu_running_state new_state);
#else

void change_running_state(uint8_t new_state, uint8_t *state_of_the_mcu);

#endif

void change_sample_rate(sample_rate new_sample_rate);

void IIC_slave_init(uint8_t address);

#endif
