#include "unity.h"

#include "src/interruptManager/interruptManager.h"
#include "src/interruptManager/MockinterruptManager_avr_Mock.h"

void test_clearInterrupt(void) {
    cli_Expect();
    interruptManager_clearInterrupt();
}

void test_setInterrupt(void) {
    sei_Expect();
    interruptManager_setInterrupt();
}
