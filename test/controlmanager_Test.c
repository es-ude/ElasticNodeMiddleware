#include "unity.h"

#include "src/controlmanager/controlmanager.h"

#include "src/configuration/Mockconfiguration.h"
#include "src/xmem/Mockxmem.h"
#include "src/debug/Mockdebug.h"
#include "src/flash/Mockflash.h"

uint8_t *userlogic_reset_addr;
uint8_t *userlogic_id_addr;

uint8_t memoryarea[2000];
const uint8_t *externalMockMemory = (const uint8_t *) &memoryarea;

void test_userlogic_enable(void) {
    xmem_enableXmem_Expect();
    //*userlogic_reset_addr = 0;
    xmem_disableXmem_Expect();

    userlogic_enable();
}

void test_userlogic_read_id(void) {
    //uint8_t id;

    xmem_enableXmem_Expect();

    //id = *userlogic_id_addr;

    xmem_disableXmem_Expect();

    userlogic_read_id();
}

void test_control_setUserHandle(void) {

    control_setUserHandle(0);
}

void test_control_handleChar(void) {

    debugAck_Expect('u');
    debugReadCharAvailable_ExpectAndReturn(1);
    debugGetChar_ExpectAndReturn('e');
    //TODO: else
    control_handleChar( 'u');

    debugAck_Expect('F');
    initFlash_Expect();
    unlockFlash_Expect(0);
    configurationUartFlash_Expect();
    control_handleChar( 'F');

    //userlogic_enable_Expect();
    //userlogic_read_id_Expect();
    //control_handleChar( 'i');

    debugAck_Expect( 1);
    control_handleChar( 0);

}
