#include "lib/interruptManager/interruptManager.h"
#include "lib/interruptManager/interruptManager_avr_Mock.h"

void interruptManager_clearInterrupt(void) {
    cli();
}

void interruptManager_setInterrupt(void) {
    sei();
}