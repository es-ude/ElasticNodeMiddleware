//
// Created by annika on 18.11.19.
//

#include "elas/interrupt_manager.h"
#include <avr/interrupt.h>

void clearInterrupt() {
    cli();
}

void setInterrupt() {
    sei();
}