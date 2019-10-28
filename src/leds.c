//
// Created by annika on 28.10.19.
//

#include "elasticnodemiddleware/leds.h"
#include "elasticnodemiddleware/leds_internal.h"

void setLed(uint8_t led, uint8_t value) {

    /*if (!ledsEnabled || (currentLedsMode == ledsDemo))
        return;*/
    if(ledsEnabled && (currentLedsMode != ledsDemo)) {
        changeLedValue_internal(led, value);
    }
}
void changeLedValue(uint8_t led, uint8_t value) {

}