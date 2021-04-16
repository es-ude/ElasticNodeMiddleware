#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "stdint.h"
#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"

#ifdef DEBUG
/// Needed for the custom led commands using the example bitfiles ///
uint8_t *data;
volatile uint8_t *addr_led;
/// Needed for the custom led commands using the example bitfiles ///
#endif

// These are potential ISRs, programmers can uncomment to have custom interrupt
// handling on receiving and sending via UART.
#ifdef UART
ISR(USART1_RX_vect) {
        elasticnode_uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
        elasticnode_uart_ISR_Transmit();
}
#endif

// Uncomment this, when requiring a custom interrupt handler on a reconfiguration complete.
// Don't forget to enable the corresponding ISR settings.
//ISR(FPGA_DONE_INT_VECTOR) {
//    reconfigure_interruptSR();
//}

void handleCharInput(uint8_t currentData);

int main(void) {
    elasticnode_led_mcu_init();
#ifdef DEBUG
    elasticnode_debugInit(NULL);
    elasticnode_control_setUserHandle(&handleCharInput);
    // Needed for the custom led commands using the example bitfiles
    addr_led = (uint8_t *) (elasticnode_xmem_offset() + 0x03);

    elasticnode_debugWriteString("Welcome to the development. To enter user mode commands, press 'u'\r\n");
#endif
    elasticnode_initialise();
    elasticnode_fpgaPowerOff();
    elasticnode_led_mcu_turnOn(1);
    elasticnode_led_mcu_turnOn(0);
    _delay_ms(3000); // Gives everything time to initialise properly.
    elasticnode_led_mcu_turnOff(1);
    elasticnode_xmem_initXmem();
    elasticnode_fpgaPowerOn();
    _delay_ms(1500);
    // Configures to address 0x0 at start up
    if (!elasticnode_configureFPGA_wait_for_finish(0x0)) {
#ifdef DEBUG
        elasticnode_debugWriteLine("init configuration of FPGA failed!");
#endif
    }
    elasticnode_led_mcu_turnOff(0);
    while (true) {

        //TODO: your implementation goes shere

#ifdef DEBUG
        // have after each cycle a moment to see if we have a user interaction
        if (elasticnode_debugReadCharAvailable()) {
            uint8_t data = elasticnode_debugGetChar();
            elasticnode_control_handleChar(data);
#ifdef UART
            elasticnode_debugReadCharProcessed();
#endif
        }
#ifndef UART
        elasticnode_debugTask(); // This only prints a part of the transmit buffer. Beware when sending a lot of strings
#endif
#endif
    }
    return 0;
}

#ifdef DEBUG
void handleCharInput(uint8_t currentData) {
    // TODO: Please fill this with custom command handling where necessary
    switch (currentData) {
        case 't':
            elasticnode_debugWriteString("\nuser mode test. You are a cool dev!\r\n");
            break;

        /// Custom led commands for the example bitfiles ///
        case 'L':
            elasticnode_enableFpgaInterface();
            *(addr_led) = (uint8_t) (0xff);
            *data = *(addr_led);
            elasticnode_debugWriteLine("led_data: ");
            elasticnode_debugWriteHex8(*data);
            elasticnode_debugWriteLine("\r\n");
            break;
        case 'l':
            elasticnode_enableFpgaInterface();
            *(addr_led) = (uint8_t) (0x00);
            *data = *(addr_led);
            elasticnode_debugWriteLine("led_data: ");
            elasticnode_debugWriteHex8(*data);
            elasticnode_debugWriteLine("\r\n");
            break;
        /// Custom led commands for the example bitfiles ///

        /// Manually change the configuration of the FPGA ///
        case 'r':
            if (elasticnode_configureFPGA_wait_for_finish(0x0)) {
                elasticnode_debugWriteLine("reconfigured FPGA to 0x0");
            }
            break;
        case 'R':
            // elasticnode_configureFPGA(0x90000); // alternative, does not check if succeeds
            // while(!elasticnode_reconfigure_fpgaMultibootComplete());  // check if reconfigure is complete manually
            if (elasticnode_configureFPGA_wait_for_finish(0x90000)) {
                elasticnode_debugWriteLine("reconfigured FPGA to 0x90000");
            }

            break;
        default:
            elasticnode_debugWriteString("unknown mode command received\r\n");
            break;
        /// Manually change the configuration of the FPGA ///
    }
#ifndef UART
    elasticnode_debugTask();
#endif
}
#endif
