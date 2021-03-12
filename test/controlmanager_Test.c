#include "unity.h"

#include "src/controlmanager/controlmanager.h"

#include "src/configuration/Mockconfiguration.h"
#include "src/debug/Mockdebug.h"
#include "src/flash/Mockflash.h"
#include "src/controlmanager/Mockcontrolmanager_internal.h"

void test_control_setUserHandle(void) {
    control_setUserHandle(0);
}

void test_control_handleChar(void) {
    debugAck_Expect('u');
    debugReadCharAvailable_ExpectAndReturn(1);
    debugGetChar_ExpectAndReturn('e');
    control_handleChar('u');

    debugAck_Expect('F');
    initFlash_Expect();
    unlockFlash_Expect(0);
    configurationUartFlash_Expect();
    control_handleChar('F');

    userlogic_enable_internal_Expect();
    userlogic_read_id_internal_Expect();
    control_handleChar('i');

    debugAck_Expect(1);
    control_handleChar(0);
}
