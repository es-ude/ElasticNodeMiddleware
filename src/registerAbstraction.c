//
// Created by annika on 17.09.19.
//

#include "elasticnodemiddleware/registerAbstraction.h"

#include <stdio.h>

void abstraction_setRegisterToValue(volatile uint8_t* targetRegister, uint8_t valueToSet) {
    (*targetRegister) = valueToSet;
}

uint8_t abstraction_isBitSet(volatile uint8_t* targetRegister, uint8_t bitToRead) {
    return ((*targetRegister) & (1 << bitToRead));
}

uint8_t abstraction_getBit(volatile uint8_t* targetRegister, uint8_t bitToRead) {
    return ((*targetRegister) & (1 << bitToRead)) != 0;
}
uint8_t abstraction_getRegisterValue(volatile uint8_t* targetRegister) {
    return *targetRegister;
}