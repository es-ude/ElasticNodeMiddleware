#include "src/controlmanager/controlmanager.h"

#include "src/configuration/configuration.h"
#include "src/xmem/xmem.h"
#include "src/debug/debug.h"
#include "src/flash/flash.h"

uartReceiveMode currentUartReceiveMode = UART_IDLE;
//loadingMode currentLoadingMode = LOADING_IDLE;

//uint8_t *data;

//volatile uint8_t *addr_led = (uint8_t *) (XMEM_OFFSET + 0x03);


volatile uint8_t *userlogic_reset_addr = (uint8_t * )(XMEM_OFFSET + 0x04);
volatile uint8_t *userlogic_id_addr = (uint8_t * )(XMEM_USERLOGIC_OFFSET + 1500);

void dummyHandler(uint8_t currentData);

void (*userDefinedHandler)(uint8_t) = &dummyHandler;

/* function below is also the middleware level, for enabling the user_logic,
 * which is above the middleware.
 * TODO from saph: I am not entirely sure why we are having this function here.
 * If user logic has to be enabled for all types of user logic interactions, we might as well
 * */
void userlogic_enable(void) {
    xmem_enableXmem();
    *userlogic_reset_addr = 0; // add _delay_ms(1); if something goes wrong
    xmem_disableXmem();
}

/* *
 * returns the userlogic id of the currently loaded bitfile
 * */
void userlogic_read_id(void) {
    uint8_t id;

    xmem_enableXmem();
    id = *userlogic_id_addr;
    debugWriteLine("User_logic_ID: ");
    debugWriteHex8(id);
    debugWriteLine("\r\n");
    xmem_disableXmem();
}

uint8_t control_isUartIdle(void) {
    return currentUartReceiveMode != UART_IDLE;
}

void dummyHandler(uint8_t currentData) {
    //dummy function, to never have the situation of having a function null pointer.
    // (unless the user wants to make his or her life miserable. Who am I to deny you that)
    //tbf, if the control manager is enabled, performance isn't really what we are looking for.
    debugWriteString("Control Manager User mode handler has not been set. Please check your code.\r\n");
}

void control_setUserHandle(void (*userHandler)(uint8_t)) {
    userDefinedHandler = userHandler;
}

void control_handleChar(uint8_t currentData) {
    switch (currentUartReceiveMode) {
        case UART_IDLE:
        default:
            switch (currentData) {
                case 'u':
                    debugAck('u');
                    debugWriteString("\n\rEntering user mode. To exit press letter \"e\"\r\n");
//                    _delay_ms(1000);
                    uint8_t userModeActive = 1;
                    while (userModeActive) {
                        if (debugReadCharAvailable()) {
                            char userModeData = debugGetChar();
                            if (userModeData == 'e') {
                                debugWriteString("exiting user mode\r\n");
                                userModeActive = 0;
                            } else {
                                (*userDefinedHandler)(userModeData);
                            }
                        }
                        debugTask();
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
                    // For configure the FLASH chip
                    initFlash(); // SPI interface init and ..? Todo
                    unlockFlash(0); // To write data on FLASH, must unlock the flash
                    configurationUartFlash();
                    break;
                case 'i':
                    userlogic_enable();
                    userlogic_read_id();
                    break;
                default:
                    /* *
                     * The bit flash script REQUIRES an echo default case where a set on numbers are echoed back
                     * to perform a synchronisation. After this the MCU is considered ready to receive the commands
                     * to write the bit file into the flash.
                     * */
                    debugAck(currentData + 1);
                    break;
//
//                case 'L':
//                    xmem_enableXmem();
//                    *(addr_led) = (uint8_t) (0xff);
//                    *data = *(addr_led);
//                    debugWriteLine("led_data: ");
//                    debugWriteHex8(*data);
//                    debugWriteLine("\r\n");
//                    break;
//                case 'l':
//                    xmem_enableXmem();
//                    *(addr_led) = (uint8_t) (0x00);
//                    *data = *(addr_led);
//                    debugWriteLine("led_data: ");
//                    debugWriteHex8(*data);
//                    debugWriteLine("\r\n");
//                    break;
//                case 'r':
//                    reconfigure_fpgaMultiboot(0x0);
//                    break;
//                case 'R':
//                    reconfigure_fpgaMultiboot(0x90001);
//                    break;
//                    // indicate readiness

            }
            break;

        case UART_FLASH:
            break;
    }
}
