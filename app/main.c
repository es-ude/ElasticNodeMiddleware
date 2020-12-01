#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "lib/debug/debug.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/flash/flash.h"
#include "lib/led/led_mcu.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/xmem/xmem.h"

#ifdef DEBUG

#include "lib/controlmanager/controlmanager.h"

#endif

//the following ISR's have to be comment in by programmer

/* for using uart

ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}
*/
//// IMPORTANT:
//// Please check
/* for using reconfigure

ISR(FPGA_DONE_INT_VECTOR) {
    reconfigure_interruptSR();
}
*/

void handleCharInput(uint8_t currentData);

int main(void) {
    led_mcu_init();
#ifdef DEBUG
    debugInit(NULL);
    control_setUserHandle(&handleCharInput);
#endif

    // need a delay for initialise and reboot the mcu
    // first led should blink

    elasticnode_initialise();
    elasticnode_fpgaPowerOff();
    led_mcu_turnOn(0);
    _delay_ms(3000);
    led_mcu_turnOff(0);
    elasticnode_fpgaPowerOn();

    while (true) {

        //your implementation

#ifdef DEBUG
        // have after each cycle a moment to see if we have a user interaction
        if (debugReadCharAvailable()) {
            uint8_t data = debugGetChar();
            // acknowledge when ready to receive again
//            debugAck(data);
            control_handleChar(data);
        }
        debugTask();
#endif
    }
    return 0;

}

void handleCharInput(uint8_t currentData) {
    //TODO: Please fill this with custom command handling where necessary
    switch (currentData) {
        default:
            debugWriteLine("default user level command handle\r\n");
            break;
    }
}