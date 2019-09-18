//
// Created by annika on 17.09.19.
//

#include "elasticnodemiddleware/registerAbstraction.h"

#include <stdio.h>

void abstraction_setRegisterToValue(volatile uint8_t* targetRegister, uint8_t valueToSet) {
    (*targetRegister) = valueToSet;
}

void abstraction_setRegisterBitsHigh(volatile uint8_t* targetRegister, uint8_t bitmask) {
    *targetRegister |= bitmask;
}

void abstraction_setRegisterBitsLow(volatile uint8_t* targetRegister, uint8_t bitmask) {
    *targetRegister &= ~bitmask;
}

uint8_t abstraction_isBitSet(volatile uint8_t* targetRegister, uint8_t bitToRead) {
    return ((*targetRegister) & (1 << bitToRead));
}

uint8_t abstraction_getRegisterValue(volatile uint8_t* targetRegister) {
    return *targetRegister;
}