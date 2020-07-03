#include "lib/led/led_mcu.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"

#include "EmbeddedUtilities/BitManipulation.h"

void led_mcu_init(void) {
    //DDRD = 0xff
    BitManipulation_setBit(DDR_LED, P_LED_0);
    BitManipulation_setBit(DDR_LED, P_LED_1);
    BitManipulation_setBit(DDR_LED, P_LED_2);
    BitManipulation_setBit(DDR_LED, P_LED_3);

}
void led_mcu_turnOn(uint8_t lednumber) {

    switch(lednumber) {
        case 0:
            BitManipulation_setBit(PORT_LED, P_LED_0);
            break;
        case 1:
            BitManipulation_setBit(PORT_LED, P_LED_1);
            break;
        case 2:
            BitManipulation_setBit(PORT_LED, P_LED_2);
            break;
        case 3:
            BitManipulation_setBit(PORT_LED, P_LED_3);
            break;
        default:
            break;
    }
}

void led_mcu_turnOff(uint8_t lednumber) {

    switch(lednumber) {
        case 0:
            BitManipulation_clearBit(PORT_LED, P_LED_0);
            break;
        case 1:
            BitManipulation_clearBit(PORT_LED, P_LED_1);
            break;
        case 2:
            BitManipulation_clearBit(PORT_LED, P_LED_2);
            break;
        case 3:
            BitManipulation_clearBit(PORT_LED, P_LED_3);
            break;
        default:
            break;
    }
}

void led_mcu_turnOnAll(void) {

    BitManipulation_setBit(PORT_LED, P_LED_0);
    BitManipulation_setBit(PORT_LED, P_LED_1);
    BitManipulation_setBit(PORT_LED, P_LED_2);
    BitManipulation_setBit(PORT_LED, P_LED_3);
}

void led_mcu_turnOffAll(void) {

    BitManipulation_clearBit(PORT_LED, P_LED_0);
    BitManipulation_clearBit(PORT_LED, P_LED_1);
    BitManipulation_clearBit(PORT_LED, P_LED_2);
    BitManipulation_clearBit(PORT_LED, P_LED_3);
}