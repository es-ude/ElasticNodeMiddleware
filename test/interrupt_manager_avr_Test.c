//
// Created by annika on 18.11.19.
//

#include "unity.h"
#include "lib/interruptManager/interruptManager.h"
#include "lib/interruptManager/MockinterruptManager_avr_Mock.h"

void test_clearInterrupt(void) {
    cli_Expect();
    interruptManager_clearInterrupt();
}

void test_setInterrupt(void) {
    sei_Expect();
    interruptManager_setInterrupt();
}