#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"

//#ifdef DEBUG
//uint8_t *data;
//volatile uint8_t *addr_led = (uint8_t *) (XMEM_OFFSET + 0x03);
//#endif

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
    elasticnode_led_mcu_init();
#ifdef DEBUG
    elasticnode_debugInit(NULL);
    elasticnode_control_setUserHandle(&handleCharInput);
    // TODO: Does not work in some enviroments
    //elasticnode_debugWriteString("Welcome to the development. To enter user mode commands, press 'u'\r\n");
#endif
    elasticnode_initialise();
    elasticnode_fpgaPowerOff();
    elasticnode_led_mcu_turnOn(0); // Gives everything time to initialise properly.
    _delay_ms(3000);
    elasticnode_led_mcu_turnOff(0);
    elasticnode_fpgaPowerOn();
    _delay_ms(1500);
    elasticnode_configureFPGA(0x0); // Configures to address 0x0 at start up

    while (true) {
        //TODO: your implementation goes here


#ifdef DEBUG
        // have after each cycle a moment to see if we have a user interaction
        if (elasticnode_debugReadCharAvailable()) {
            uint8_t data = elasticnode_debugGetChar();
            elasticnode_control_handleChar(data);
        }
        elasticnode_debugTask(); // This only prints a part of the transmit buffer. Beware when sending a lot of strings
#endif
    }
    return 0;
}

#ifdef DEBUG
void handleCharInput(uint8_t currentData) {
    //TODO: Please fill this with custom command handling where necessary
    switch (currentData) {
        case 't':
            elasticnode_debugWriteString("\nuser mode test. You are a cool dev!\r\n");
            break;
//        case 'L':
//            elasticnode_enableFpgaInterface();
//            *(addr_led) = (uint8_t) (0xff);
//            *data = *(addr_led);
//            elasticnode_debugWriteLine("led_data: ");
//            elasticnode_debugWriteHex8(*data);
//            elasticnode_debugWriteLine("\r\n");
//            break;
//        case 'l':
//            elasticnode_enableFpgaInterface();
//            *(addr_led) = (uint8_t) (0x00);
//            *data = *(addr_led);
//            elasticnode_debugWriteLine("led_data: ");
//            elasticnode_debugWriteHex8(*data);
//            elasticnode_debugWriteLine("\r\n");
//            break;
        case 'r':
            elasticnode_configureFPGA(0x0);
            elasticnode_debugWriteLine("reconfigured FPGA to 0x0");
            break;
        case 'R':
            elasticnode_configureFPGA(0x90000);
            elasticnode_debugWriteLine("reconfigured FPGA to 0x90000");
            break;
        default:
            elasticnode_debugWriteString("unknown mode command received\r\n");
            break;
    }
    elasticnode_debugTask();
}
#endif
