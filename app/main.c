#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "EmbeddedUtilities/BitManipulation.h"

#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/xmem/xmem.h"
#include "lib/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"


int main(void)
{
 DDRD = 0xff;
  while (true)
  {
      BitManipulation_setBit(&PORTD, PD4);
      BitManipulation_setBit(&PORTD, PD5);
      BitManipulation_setBit(&PORTD, PD6);
      BitManipulation_setBit(&PORTD, PD7);
      _delay_ms(1000);
      BitManipulation_clearBit(&PORTD, PD4);
      BitManipulation_clearBit(&PORTD, PD5);
      BitManipulation_clearBit(&PORTD, PD6);
      BitManipulation_clearBit(&PORTD, PD7);
      _delay_ms(1000);
  }
}
/*
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



#pragma clang diagnostic pop*/