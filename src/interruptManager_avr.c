//
// Created by annika on 18.11.19.
//

#include "elasticnodemiddleware/interruptManager.h"
#include "elasticnodemiddleware/interruptManager_avr_Mock.h"

void interruptManager_clearInterrupt() {
    cli();
}

void interruptManager_setInterrupt() {
    sei();
}