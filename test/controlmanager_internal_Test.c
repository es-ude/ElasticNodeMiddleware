#include "unity.h"

#include "src/controlmanager/controlmanager_internal.h"

#include "src/xmem/Mockxmem.h"

#include "src/debug/Mockdebug.h"

uint8_t memoryarea[2000];
const uint8_t *externalMockMemory = (const uint8_t *) &memoryarea;

uint8_t id;
uint8_t *userlogic_reset_addr;
uint8_t *userlogic_id_addr;

void test_userlogic_enable(void) {
    xmem_enableXmem_Expect();
    xmem_disableXmem_Expect();

    userlogic_enable_internal();

    TEST_ASSERT_EQUAL(0, *userlogic_reset_addr);
}

void test_userlogic_read_id(void) {
    xmem_enableXmem_Expect();
    xmem_disableXmem_Expect();

    userlogic_read_id_internal();

    TEST_ASSERT_EQUAL(id, *userlogic_id_addr);
}
