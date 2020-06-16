#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "lib/debug/debug.h"
#include <avr/interrupt.h>
#include "lib/xmem/xmem.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/flash_new/flash.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/uartmanager/uartmanager.h"

//the following ISR's have to be comment in by programmer

/* for using uart

ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}
*/
//// IMPORTANT: by comment in these code the main runs 3 times (?, only the last time till the end)?!
//// Please check
/* for using reconfigure

ISR(FPGA_DONE_INT_VECTOR) {
    reconfigure_interruptSR();
}
*/

int main(void)
{

    DDRD = 0xff;
    xmem_initXmem();
    xmem_enableXmem();

    debugInit(NULL);

    initFlash();
    fpgaFlashInit();

    elasticnode_initialise();
//    reconfigure_initMultiboot();

    while (true) {
        if(debugReadCharAvailable())
        {
            uint8_t data = debugGetChar();

            // acknowledge when ready to receive again
//            debugAck(data);

            //configurationUartFlash();
            uartProcessInput(data);
        }
        debugTask();
    }
    return 0;
}
