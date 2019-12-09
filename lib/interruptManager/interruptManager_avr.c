//
// Created by annika on 18.11.19.
//

#include "lib/interruptManager/interruptManager.h"
#include "lib/interruptManager/interruptManager_avr_Mock.h"

void interruptManager_clearInterrupt(void) {
    cli();
}

void interruptManager_setInterrupt(void) {
    sei();
}