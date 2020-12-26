#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "src/debug/debug.h"
#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "src/led/led_mcu.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "src/xmem/xmem.h"

#ifdef DEBUG
#include "src/controlmanager/controlmanager.h"
#endif

#ifdef DEBUG
uint8_t *data;
volatile uint8_t *addr_led = (uint8_t *) (XMEM_OFFSET + 0x03);
#endif

//These are potential ISRs programmers can uncomment to have custom interrupt
// handling on receiving and sending via UART.
//ISR(USART1_RX_vect) {
//    uart_ISR_Receive();
//}
//
//ISR(USART1_TX_vect) {
//    uart_ISR_Transmit();
//}
//
// Uncomment this, when requiring a custom interrupt handler on a reconfiguration complete.
// Don't forget to enable the corresponding ISR settings.
//ISR(FPGA_DONE_INT_VECTOR) {
//    reconfigure_interruptSR();
//}

void handleCharInput(uint8_t currentData);

int main(void) {
    led_mcu_init();
#ifdef DEBUG
    debugInit(NULL);
    control_setUserHandle(&handleCharInput);
    // TODO: Does not work in some enviroments
    //debugWriteString("Welcome to the development. To enter user mode commands, press 'u'\r\n");
#endif
    elasticnode_initialise();
    elasticnode_fpgaPowerOff();
    led_mcu_turnOn(0); // Gives everything time to initialise properly.
    _delay_ms(3000);
    led_mcu_turnOff(0);
    elasticnode_fpgaPowerOn();
    while (true) {
        //TODO: your implementation goes here


#ifdef DEBUG
        // have after each cycle a moment to see if we have a user interaction
        if (debugReadCharAvailable()) {
            uint8_t data = debugGetChar();
            control_handleChar(data);
        }
        debugTask(); // This only prints a part of the transmit buffer. Beware when sending a lot of strings
#endif
    }
    return 0;
}

#ifdef DEBUG
void handleCharInput(uint8_t currentData) {
    //TODO: Please fill this with custom command handling where necessary
    switch (currentData) {
        case 't':
            debugWriteString("\nuser mode test. You are a cool dev!\r\n");
            break;
                case 'L':
                    xmem_enableXmem();
                    *(addr_led) = (uint8_t) (0xff);
                    *data = *(addr_led);
                    debugWriteLine("led_data: ");
                    debugWriteHex8(*data);
                    debugWriteLine("\r\n");
                    break;
                case 'l':
                    xmem_enableXmem();
                    *(addr_led) = (uint8_t) (0x00);
                    *data = *(addr_led);
                    debugWriteLine("led_data: ");
                    debugWriteHex8(*data);
                    debugWriteLine("\r\n");
                    break;
                case 'r':
                    reconfigure_fpgaMultiboot(0x0);
                    break;
                case 'R':
                    reconfigure_fpgaMultiboot(0x90001);
                    break;
        default:
            debugWriteString("unknown mode command received\r\n");
            break;
    }
    debugTask();
}
#endif