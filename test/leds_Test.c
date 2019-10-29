//
// Created by annika on 28.10.19.
//

#include "unity.h"
#include "elasticnodemiddleware/leds.h"
#include "elasticnodemiddleware/Mockleds_internal.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"

uint8_t ddr_led_0;
uint8_t ddr_led_1;
uint8_t ddr_led_2;
uint8_t ddr_led_3;

uint8_t port_led_0;
uint8_t port_led_1;
uint8_t port_led_2;
uint8_t port_led_3;

uint8_t* DDR_LED_0 = &ddr_led_0;
uint8_t* DDR_LED_1 = &ddr_led_1;
uint8_t* DDR_LED_2 = &ddr_led_2;
uint8_t* DDR_LED_3 = &ddr_led_3;

uint8_t* PORT_LED_0 = &port_led_0;
uint8_t* PORT_LED_1 = &port_led_1;
uint8_t* PORT_LED_2 = &port_led_2;
uint8_t* PORT_LED_3 = &port_led_3;

void test_initialise_LedMockRegister(void){

    DDR_LED_0 = &ddr_led_0;
    DDR_LED_1 = &ddr_led_1;
    DDR_LED_2 = &ddr_led_2;
    DDR_LED_3 = &ddr_led_3;

    PORT_LED_0 = &port_led_0;
    PORT_LED_1 = &port_led_1;
    PORT_LED_2 = &port_led_2;
    PORT_LED_3 = &port_led_3;
}

void test_initLeds(void)
{
    test_initialise_LedMockRegister();

    abstraction_setRegisterBitsHigh_Expect(DDR_LED_0, (1 << P_LED_0));
    abstraction_setRegisterBitsHigh_Expect(DDR_LED_1, (1 << P_LED_1));
    abstraction_setRegisterBitsHigh_Expect(DDR_LED_2, (1 << P_LED_2));
    abstraction_setRegisterBitsHigh_Expect(DDR_LED_3, (1 << P_LED_3));

   initLeds();

   TEST_ASSERT_EQUAL((*ptr_ledsMode), ledsDebug);
   TEST_ASSERT_EQUAL_UINT8((*ptr_ledsEnabled), 1);
}

void test_setLed(){

    test_initialise_LedMockRegister();

    uint8_t led = 0;
    uint8_t value = 1;
    changeLedValue_internal_Expect(led, value);

    setLed(led, value);
}

void test_changeLedValue(){

    test_initialise_LedMockRegister();

    uint8_t led = 0;
    uint8_t value = 1;

    abstraction_setRegisterToValue_Expect(PORT_LED_0, (value << P_LED_0));
    changeLedValue(led,value);

    led = 1;
    abstraction_setRegisterToValue_Expect(PORT_LED_1, (value << P_LED_1));
    changeLedValue(led,value);

    led = 2;
    abstraction_setRegisterToValue_Expect(PORT_LED_2, (value << P_LED_2));
    changeLedValue(led,value);

    led = 3;
    abstraction_setRegisterToValue_Expect(PORT_LED_3, (value << P_LED_3));
    changeLedValue(led,value);
}