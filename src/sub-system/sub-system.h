#ifndef SUB_SYSTEM_H
#define SUB_SYSTEM_H

#define BITMASK_RUNNING_STATE 0b00001111
#define BITMASK_SAMPLE_RATE 0b11110000

#define MY_IIC_ADDRESS 64

#include <stdint.h>

//uint8_t state_of_the_mcu;
//uint8_t sample_rate;

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
            RECONFIG_4 = 13, 
            SEND_INPUT_4 = 14,
            EXECUTION_4 = 15,
            RECEIVE_FINAL_RES = 16,

}mcu_state;


// typedef enum{


// }sample_rate;


void change_state(mcu_state new_state, uint8_t *state_of_the_mcu);

void change_sample_rate(uint8_t new_sample_rate, uint8_t *state_of_the_mcu);

 






















#endif