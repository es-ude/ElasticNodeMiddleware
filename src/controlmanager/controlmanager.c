#include "src/controlmanager/controlmanager.h"

#include "src/controlmanager/controlmanager_internal.h"

#include "src/configuration/configuration.h"
#include "src/xmem/xmem.h"
#include "src/flash/flash.h"

#include "src/debug/debug.h"

uartReceiveMode currentUartReceiveMode = UART_IDLE;
//loadingMode currentLoadingMode = LOADING_IDLE;

void dummyHandler(uint8_t currentData);

void (*userDefinedHandler)(uint8_t) = &dummyHandler;

// TODO: Necessary?
/*
uint8_t control_isUartIdle(void) {
    return currentUartReceiveMode != UART_IDLE;
}
*/

void dummyHandler(uint8_t currentData) {
    //dummy function, to never have the situation of having a function null pointer.
    // (unless the user wants to make his or her life miserable. Who am I to deny you that)
    //tbf, if the control manager is enabled, performance isn't really what we are looking for.
#ifndef TEST
    debugWriteString("Control Manager User mode handler has not been set. Please check your code.\r\n");
#endif
}

void control_setUserHandle(void (*userHandler)(uint8_t)) {
    userDefinedHandler = userHandler;
}

void control_handleChar(uint8_t currentData) {
#ifndef TEST
#ifdef UART
    debugReadCharProcessed();
#endif
#endif
    switch (currentUartReceiveMode) {
        case UART_IDLE:
        default:
            switch (currentData) {
                case 'u':
                    debugAck('u');
#ifndef TEST
                    debugWriteString("\n\rEntering user mode. To exit press letter \"e\"\r\n");
#endif
//                    _delay_ms(1000);
                    uint8_t userModeActive = 1;
                    while (userModeActive) {
                        if (debugReadCharAvailable()) {
                            char userModeData = debugGetChar();
                            if (userModeData == 'e') {
#ifndef TEST
                                debugWriteString("exiting user mode\r\n");
#endif
                                userModeActive = 0;
                            } else {
#ifndef TEST
                                (*userDefinedHandler)(userModeData);
#endif
                            }
#ifndef TEST
#ifdef UART
                            debugReadCharProcessed();
#endif
#endif
                        }
#ifndef TEST
#ifdef LUFA
                        debugTask();
#endif
#endif
                    }
                    break;
                case 'F':
                    //This the crucial case, enabled so that any debugging application can support writing new or
                    // different bitfiles to the flash
                    // certain assumptions made about addresses aligning
                    // addresses must be aligned to 4K blocks 0xFFF000
                    // calculate what blocks need to be erased
                    // acknowledge when ready to receive again
                    debugAck(currentData);
#ifdef UART
                    debugReadCharProcessed();
#endif
                    // For configure the FLASH chip
                    char st[] = "lashFPGA!";
                    uint8_t synced = 1;
                    uint8_t i = 0;
                    while (synced) {
                        if (st[i] == '!') {
                            break;
                        }
                        synced = check_for_acks_internal(st[i]);
                        i++;
                    }
                    if (synced) {
                        initFlash(); // SPI interface init and ..? Todo
                        unlockFlash(0); // To write data on FLASH, must unlock the flash
                        configurationUartFlash();
                    }
                    break;
                case 'i':
                    userlogic_enable_internal();
                    userlogic_read_id_internal();
                    break;
                default:
                    /* *
                     * The bit flash script REQUIRES an echo default case where a set on numbers are echoed back
                     * to perform a synchronisation. After this the MCU is considered ready to receive the commands
                     * to write the bit file into the flash.
                     * */
                    debugAck(currentData + 1);
                    break;
            }
            break;

        case UART_FLASH:
            break;
    }
}
