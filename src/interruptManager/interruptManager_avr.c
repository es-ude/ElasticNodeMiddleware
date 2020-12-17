#include "src/interruptManager/interruptManager.h"
#include "src/interruptManager/interruptManager_avr_Mock.h"

void interruptManager_clearInterrupt(void) {
    cli();
}

void interruptManager_setInterrupt(void) {
    sei();
}