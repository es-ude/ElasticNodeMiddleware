#include "controlmanager_internal.h"

#include "src/xmem/xmem.h"

#include "src/debug/debug.h"

volatile uint8_t *userlogic_reset_addr = (uint8_t *) (XMEM_OFFSET + 0x04);
volatile uint8_t *userlogic_id_addr = (uint8_t *) (XMEM_USERLOGIC_OFFSET + 1500);

/* function below is also the middleware level, for enabling the user_logic,
 * which is above the middleware.
 * TODO from saph: I am not entirely sure why we are having this function here.
 * If user logic has to be enabled for all types of user logic interactions, we might as well
 * */
void userlogic_enable_internal(void) {
    xmem_enableXmem();
    *userlogic_reset_addr = 0; // add _delay_ms(1); if something goes wrong
    xmem_disableXmem();
}

/* *
 * returns the userlogic id of the currently loaded bitfile
 * */
void userlogic_read_id_internal(void) {
    uint8_t id;
    xmem_enableXmem();
    id = *userlogic_id_addr;
#ifndef TEST
    debugWriteLine("User_logic_ID: ");
    debugWriteHex8(id);
    debugWriteLine("\r\n");
#endif
    xmem_disableXmem();
}

uint8_t check_for_acks_internal(char c) {

    uint8_t synced = 0;
    uint8_t iter = 1;

    while (iter) {
#ifdef LUFA
        debugTask();
#endif

        if (debugReadCharAvailable()) {
            uint8_t data = debugGetChar();
            debugAck(data);
            if (data == c) {
                synced = 1;
            }
            iter = 0;
#ifdef UART
            debugReadCharProcessed();
#endif
        }

    }

    return synced;
}
