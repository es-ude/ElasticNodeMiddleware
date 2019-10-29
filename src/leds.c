//
// Created by annika on 28.10.19.
//

#include "elasticnodemiddleware/leds.h"
#include "elasticnodemiddleware/leds_internal.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/fpgaPins.h"

void initLeds(void)
{
    abstraction_setRegisterBitsHigh(DDR_LED_0, (1 << P_LED_0));
    abstraction_setRegisterBitsHigh(DDR_LED_1, (1 << P_LED_1));
    abstraction_setRegisterBitsHigh(DDR_LED_2, (1 << P_LED_2));
    abstraction_setRegisterBitsHigh(DDR_LED_3, (1 << P_LED_3));

    currentLedsMode = ledsDebug;
    ptr_ledsMode = &currentLedsMode;

    ledsEnabled = 1;
    ptr_ledsEnabled = &ledsEnabled;
}

void setLed(uint8_t led, uint8_t value) {

    /*if (!ledsEnabled || (currentLedsMode == ledsDemo))
        return;*/
    if(ledsEnabled && (currentLedsMode != ledsDemo)) {
        changeLedValue_internal(led, value);
    }
}
void changeLedValue(uint8_t led, uint8_t value) {

    switch(led)
    {
        case 0:
            abstraction_setRegisterToValue(PORT_LED_0, (value << P_LED_0));
            break;
        case 1:
            abstraction_setRegisterToValue(PORT_LED_1, (value << P_LED_1));
            break;
        case 2:
            abstraction_setRegisterToValue(PORT_LED_2, (value << P_LED_2));
            break;
        case 3:
            abstraction_setRegisterToValue(PORT_LED_3, (value << P_LED_3));
            break;
        default:
            return;
    }
}