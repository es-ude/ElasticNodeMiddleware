//
// Created by annika on 28.10.19.
//

#include "unity.h"
#include "elasticnodemiddleware/leds.h"
#include "elasticnodemiddleware/Mockleds_internal.h"

void test_setLed(){

    uint8_t led = 0;
    uint8_t value = 1;
    if(ledsEnabled && (currentLedsMode != ledsDemo)) {
        changeLedValue_internal_Expect(led, value);
    }

    setLed(led, value);

    //besser testen (if-Anweisung)
}

void test_changeLedValue(){

}