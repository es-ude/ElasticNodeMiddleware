#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/debug/debug.h"
#include <avr/interrupt.h>
#include "lib/xmem/xmem.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/flash/flash.h"
#include "lib/controlmanager/controlmanager.h"
#include "EmbeddedUtilities/BitManipulation.h"

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

    DDRD = 0xff;

    debugInit(NULL);

    // in uartmanger for specific use case
    // initialises the flash and spi together
//    initFlash();

//    reconfigure_initMultiboot();

    // need a delay for initialise and reboot the mcu
    // first led should blink
    elasticnode_initialise();
    elasticnode_fpgaPowerOff();
    BitManipulation_setBit(&PORTD, PD4);
    _delay_ms(3000);
    BitManipulation_clearBit(&PORTD, PD4);
    elasticnode_fpgaPowerOn();

    while (true) {
        if(debugReadCharAvailable())
        {
            uint8_t data = debugGetChar();

            // acknowledge when ready to receive again
//            debugAck(data);

            uartProcessInput(data);
        }
        debugTask();
    }
    return 0;
}
