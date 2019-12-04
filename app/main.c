/*#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int
main(void)
{
  DDRB = _BV(5);
  while (true)
  {
    _delay_ms(500);
    PORTB ^= _BV(5);
  }
}*/

//#include "elasticnodemiddleware/fpgaRegisters.h"
//#include "elasticnodemiddleware/fpgaPins.h"
//#include <fpgaPins.h>
//#include <peripherals.h>

//#include "lib/io/pinio.h"
//#include "lib/spiArbitration/spiArbitration.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
//#include "lib/leds/leds.h"
//#include "lib/debug/debug.h"
//#include "lib/jtag/jtag.h"
//#include "lib/io/gpio.h"
//#include "lib/flash/flash.h"
//#include "lib/fpgaFlash/fpgaFlash.h"
//#include "lib/selectmap/selectmap.h"
#include "elasticnodemiddleware/xmem.h"
//#include "lib/currentMeasurement/current_sense.h"
//#include "lib/xor/xor.h"
//#include "lib/timer/timer.h"

//#include "lib/control/uartManager.h"

#include <stdio.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#include "elasticnodemiddleware/reconfigure_multiboot_avr.h"

//#include "lib/spi/spi.h"
//#include "lib/lufa/lufa.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void testXor(void);

uint8_t sleepFlag = 0x00;


int main()
{
    xmem_initXmem();
    xmem_enableXmem();
    //initLeds();
    //ledsFlash(1);
    //ledsSetMode(ledsDebug);

    //debugInit(NULL);

    //initFlash();
    //fpgaFlashInit();

    //initGpio();

    elasticnode_initialise();
    reconfigure_initMultiboot();

////	fpgaEnableFlashInterface();

    while (1)
    {
        //if(debugReadCharAvailable())
        //{
          //  uint8_t data = debugGetChar();
          //  uartProcessInput(data);
////            if (sleepFlag) // do not sleep if uart is active
////            {
////            }
        //}
        //debugTask();

    }
    return 0;
}



#pragma clang diagnostic pop