#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "lib/controlmanager/controlmanager.h"
#include "lib/debug/debug.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/flash/flash.h"
#include "lib/led/led_mcu.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/xmem/xmem.h"

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

int main(void)
{

    led_mcu_init();

    debugInit(NULL);

    // in uartmanger for specific use case
    // initialises the flash and spi together
//    initFlash();

//    reconfigure_initMultiboot();

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
        if(debugReadCharAvailable())
        {
            uint8_t data = debugGetChar();
            // acknowledge when ready to receive again
//            debugAck(data);
            uartProcessInput(data);
        }
        debugTask();
#endif
    }
    return 0;

}
