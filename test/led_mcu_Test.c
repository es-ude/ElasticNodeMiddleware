#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/led/led_mcu.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

uint8_t ddr_led;
uint8_t port_led;

uint8_t *DDR_LED = &ddr_led;
uint8_t *PORT_LED = &port_led;

void test_led_mcu_init(void) {
    DDR_LED = &ddr_led;

    BitManipulation_setBit_Expect(&ddr_led, P_LED_0);
    BitManipulation_setBit_Expect(&ddr_led, P_LED_1);
    BitManipulation_setBit_Expect(&ddr_led, P_LED_2);
    BitManipulation_setBit_Expect(&ddr_led, P_LED_3);

    led_mcu_init();
}

void test_led_mcu_turnOn(void) {
    PORT_LED = &port_led;

    BitManipulation_setBit_Expect(&port_led, P_LED_0);
    led_mcu_turnOn(0);
    BitManipulation_setBit_Expect(&port_led, P_LED_1);
    led_mcu_turnOn(1);
    BitManipulation_setBit_Expect(&port_led, P_LED_2);
    led_mcu_turnOn(2);
    BitManipulation_setBit_Expect(&port_led, P_LED_3);
    led_mcu_turnOn(3);

    for (uint8_t lednumber = 4; lednumber < 255; ++lednumber) {
        led_mcu_turnOn(lednumber);
    }
}

void test_led_mcu_turnOff(void) {
    PORT_LED = &port_led;

    BitManipulation_clearBit_Expect(&port_led, P_LED_0);
    led_mcu_turnOff(0);
    BitManipulation_clearBit_Expect(&port_led, P_LED_1);
    led_mcu_turnOff(1);
    BitManipulation_clearBit_Expect(&port_led, P_LED_2);
    led_mcu_turnOff(2);
    BitManipulation_clearBit_Expect(&port_led, P_LED_3);
    led_mcu_turnOff(3);

    for (uint8_t lednumber = 4; lednumber < 255; ++lednumber) {
        led_mcu_turnOff(lednumber);
    }
}

void test_led_mcu_turnOnAll(void) {
    PORT_LED = &port_led;

    BitManipulation_setBit_Expect(&port_led, P_LED_0);
    BitManipulation_setBit_Expect(&port_led, P_LED_1);
    BitManipulation_setBit_Expect(&port_led, P_LED_2);
    BitManipulation_setBit_Expect(&port_led, P_LED_3);

    led_mcu_turnOnAll();
}

void test_led_mcu_turnOffAll(void) {
    PORT_LED = &port_led;

    BitManipulation_clearBit_Expect(&port_led, P_LED_0);
    BitManipulation_clearBit_Expect(&port_led, P_LED_1);
    BitManipulation_clearBit_Expect(&port_led, P_LED_2);
    BitManipulation_clearBit_Expect(&port_led, P_LED_3);

    led_mcu_turnOffAll();
}
