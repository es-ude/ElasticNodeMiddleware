#include <avr/io.h>
#include <util/delay.h>
#include "EmbeddedUtilities/BitManipulation.h"
#include <stdio.h>
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
#include "lib/xmem/xmem.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"

#include "lib/flash_new/flash.h"
#include "lib/fpgaFlash_new/fpgaFlash.h"
#include "lib/configuration_new/configuration.h"

#include "lib/uartmanager/uartManager.h"

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

    debug_init(NULL);

    initFlash();
    fpgaFlashInit();

    elasticnode_initialise();

    reconfigure_initMultiboot();

    uartProcessInput('F');

    while (true) {

        uartProcessInput('F');

     /*
      _delay_ms(500);
      uart_WriteString("testing uart");
      BitManipulation_setBit(&PORTD, PD4);
      BitManipulation_setBit(&PORTD, PD5);
      BitManipulation_setBit(&PORTD, PD6);
      BitManipulation_setBit(&PORTD, PD7);
      _delay_ms(500);
      BitManipulation_clearBit(&PORTD, PD4);
      BitManipulation_clearBit(&PORTD, PD5);
      BitManipulation_clearBit(&PORTD, PD6);
      BitManipulation_clearBit(&PORTD, PD7);
*/
      //char 'F' in uartmanager
      //configurationUartFlash();
      //leds werden darin gesetzt und hier in der main! --> comment
    }


    return 0;
}
