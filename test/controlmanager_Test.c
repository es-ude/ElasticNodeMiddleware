#include "unity.h"

#include "src/controlmanager/controlmanager.h"

#include "src/configuration/Mockconfiguration.h"
#include "src/configuration/Mockconfiguration_internal.h"
#include "src/debug/Mockdebug.h"
#include "src/flash/Mockflash.h"
#include "src/controlmanager/Mockcontrolmanager_internal.h"

void test_control_setUserHandle(void) {
    control_setUserHandle(0);
}

void test_control_handleChar_u(void) {
    debugAck_Expect('u');
    debugReadCharAvailable_ExpectAndReturn(1);
    debugGetChar_ExpectAndReturn('e');
    control_handleChar('u');
}

void test_control_handleChar_F_True(void) {
    debugAck_Expect('F');
    char st[] = "lashFPGA";
    for (u_int8_t i = 0; i <= 7; i++) {
        check_for_acks_internal_ExpectAndReturn(st[i], 1);
    }
    initFlash_Expect();
    unlockFlash_Expect(0);

    configurationFlash_Expect(readData_internal);
    control_handleChar('F');
}

void test_control_handleChar_F_False(void) {
    // test if aboard if check_for_acks_internal returns False
    debugAck_Expect('F');
    char st2[] = "lash";
    for (u_int8_t i = 0; i <= 3; i++) {
        check_for_acks_internal_ExpectAndReturn(st2[i], 1);
    }
    check_for_acks_internal_ExpectAndReturn('F', 0);
    control_handleChar('F');
}

void test_control_handleChar_i(void) {
    userlogic_enable_internal_Expect();
    userlogic_read_id_internal_Expect();
    control_handleChar('i');

    debugAck_Expect(1);
    control_handleChar(0);
}
