//
// Created by annika on 28.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_LEDS_H
#define ELASTICNODEMIDDLEWARE_LEDS_H

#include <stdint.h>

static uint8_t ledsEnabled;
typedef enum ledsModeTmp { ledsDebug, ledsDemo } ledsMode;
ledsMode currentLedsMode;

void setLed(uint8_t led, uint8_t value);
void changeLedValue(uint8_t led, uint8_t value);

#endif //ELASTICNODEMIDDLEWARE_LEDS_H
