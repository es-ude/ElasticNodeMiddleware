#include "EmbeddedUtilities/BitManipulation.h"

#include "lib/configuration/configuration.h"
#include "lib/debug/debug.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "lib/flash/flash.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"
#include "lib/spi/spi.h"
#include "lib/controlmanager/controlmanager.h"
#include "lib/xmem/xmem.h"

//#include "lib/leds/leds.h"

uartReceiveMode currentUartReceiveMode = UART_IDLE;
loadingMode currentLoadingMode = LOADING_IDLE;

uint8_t *data;

volatile uint8_t *addr_led = (uint8_t *) (XMEM_OFFSET + 0x03);

//=================================================================================
/* function below is also the middleware level, for enabling the user_logic,
 * which is above the middleware.
 * */
volatile uint8_t *userlogic_reset_addr = (uint8_t *) (XMEM_OFFSET + 0x04);
void userlogic_enable(void)
{
    xmem_enableXmem();
    *userlogic_reset_addr = 0;
//    _delay_ms(1);
    xmem_disableXmem();
}

/* function below is get the id of the userlogic,
 * the id is set in userlogic
 * */
volatile uint8_t *userlogic_id_addr = (uint8_t *) (XMEM_USERLOGIC_OFFSET +1500);
void userlogic_read_id(void)
{
    uint8_t id;

    xmem_enableXmem();
    id = *userlogic_id_addr;
    debugWriteLine("User_logic_ID: ");
    debugWriteHex8(id);
    debugWriteLine("\r\n");
    xmem_disableXmem();
}
//=================================================================================


uint8_t isUartIdle(void)
{
    return currentUartReceiveMode != UART_IDLE;
}

void uartProcessInput(uint8_t currentData)
{

    switch(currentUartReceiveMode)
    {

        case UART_IDLE:
        default:

            // debugWriteString("Command: ");
            // debugWriteChar(currentData);
            // debugNewLine();
            switch(currentData)
            {
                // fetching latency
                // write to flash
                case 'F':
                    // certain assumptions made about addresses aligning
                    // addresses must be aligned to 4K blocks 0xFFF000

                    // calculate what blocks need to be erased
                    // TODO: use larger erase blocks
                    // TODO: using non async communication

                    // acknowledge when ready to receive again
                    debugAck(currentData);

                    // For configure the FLASH chip
                    initFlash(); // SPI interface init and ..? Todo
                    unlockFlash(0); // To write data on FLASH, must unlock the flash
                    configurationUartFlash();
                    break;

                case 'L':
//                    reconfigure_fpgaMultiboot(0);

                    xmem_enableXmem();
                    *(addr_led) = (uint8_t) (0xff);
                    *data = *(addr_led);
//                    xmem_disableXmem();
                    debugWriteLine("led_data: ");
                    debugWriteHex8(*data);
                    debugWriteLine("\r\n");
                    break;
                case 'l':

                    xmem_enableXmem();
                    *(addr_led) = (uint8_t) (0x00);
                    *data = *(addr_led);
//                    xmem_disableXmem();
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

                case 'i':
                    userlogic_enable();
                    userlogic_read_id(); //

                    break;


                    // indicate readiness
                default:
                    // use this to check that mcu is receiving
                    debugAck(currentData + 1);
                    // debugWriteLine("echo");
                    // uint8_t *ptr = malloc(1);
                    // debugWriteHex16((uint16_t) ptr);
                    // free(ptr);
                    // debugWriteChar(' ');

                    break;
            }
            break;

        case UART_FLASH:break;
    }
}


