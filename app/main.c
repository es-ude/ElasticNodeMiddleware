#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "EmbeddedUtilities/BitManipulation.h"
//#include <stdio.h>
#include "lib/uart/uart.h"
#include "lib/uart/uart_internal.h"
#include "lib/debug/debug.h"
#include <avr/interrupt.h>
#include "lib/xmem/xmem.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"

#include "lib/flash_new/flash.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/configuration_new/configuration.h"
//
//#include "lib/uartmanager/uartManager.h"

//#include "PeripheralInterface/LufaUsartImpl.h"

//the following ISR's have to be comment in by programmer

/* for using uart
 */
ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}

/* for using reconfigure
*/
ISR(FPGA_DONE_INT_VECTOR) {
    reconfigure_interruptSR();
}

int main(void)
{

    DDRD = 0xff;
    xmem_initXmem();
    xmem_enableXmem();

    debugInit(NULL);

    initFlash();
    fpgaFlashInit();

    elasticnode_initialise();
    reconfigure_initMultiboot();

    while (true) {

        if(debugReadCharAvailable()) {
            uint8_t data = debugGetChar();
            if(data == 'F') {
                debugAck(data);
                configurationUartFlash();

            }
        }

        /*_delay_ms(500);
        if(debugReadCharAvailable()) {
            BitManipulation_setBit(&PORTD, PD4);
            _delay_ms(500);
            uint8_t data = debugGetChar();

            if(data == '1') {
                debugWriteChar(data);
                BitManipulation_setBit(&PORTD, PD5);
                _delay_ms(500);
            }
            if(data != '1') {
                debugWriteChar(data);
                BitManipulation_setBit(&PORTD, PD6);
                _delay_ms(500);
            }


        }
        _delay_ms(500);
        BitManipulation_clearBit(&PORTD, PD4);
        BitManipulation_clearBit(&PORTD, PD5);
        BitManipulation_clearBit(&PORTD, PD6);
*/

    }


    return 0;
}
