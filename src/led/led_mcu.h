#ifndef ELASTICNODEMIDDLEWARE_LED_MCU_H
#define ELASTICNODEMIDDLEWARE_LED_MCU_H

#include <stdint.h>

void led_mcu_init(void);

void led_mcu_turnOn(uint8_t lednumber);

void led_mcu_turnOff(uint8_t lednumber);

void led_mcu_turnOnAll(void);

void led_mcu_turnOffAll(void);

#endif //ELASTICNODEMIDDLEWARE_LED_MCU_H
