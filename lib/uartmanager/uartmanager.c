//#include "uartManager.h"
#include "lib/uartmanager/uartmanager.h"
//#include <fpgaPins.h>
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/xmem/xmem.h"
//#include "lib/flash/flash.h"
#include "lib/flash_new/flash.h"
#include "lib/spi_new/spi.h"
//#include "lib/fpga/fpga.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
//#include "lib/leds/leds.h"

//#include "lib/multiboot/multiboot.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"

//#include "lib/configuration/configuration.h"
#include "lib/configuration_new/configuration.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include <avr/sleep.h>
#include <math.h>

#include "lib/debug/debug.h"

uartReceiveMode currentUartReceiveMode = UART_IDLE;
loadingMode currentLoadingMode = LOADING_IDLE;

uint8_t *data;

volatile uint8_t *addr_led = (uint8_t *) (XMEM_OFFSET + 0x03);

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
                    xmem_disableXmem();
                    debugWriteLine("led_data: ");
                    debugWriteHex8(*data);
                    debugWriteLine("\r\n");
                    break;
                case 'l':

                    xmem_enableXmem();
                    *(addr_led) = (uint8_t) (0x00);
                    *data = *(addr_led);
                    xmem_disableXmem();
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


