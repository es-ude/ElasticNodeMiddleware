#ifndef ELASTICNODEMIDDLEWARE_FPGAPINS_H
#define ELASTICNODEMIDDLEWARE_FPGAPINS_H


#ifdef TEST

#define P_FPGA_PROGRAM_B 2

#define P_FPGA_POWER_SRAM 5
#define P_FPGA_POWER_AUX 4
#define P_FPGA_POWER_IO 7
#define P_FPGA_POWER_INT 0
#define P_FPGA_CCLK 1

#define P_FPGA_INIT_B 3

#define P_FPGA_DONE 6

#define P_XMEM_WR 0
#define P_XMEM_RD 1
#define P_XMEM_ALE 2

#define SPE 6
#define SRW10 6
#define SRW11 1
#define XMBK 7
#define XMM1 1

#define XMEM_A_MASK 0xff
#define XMEM_AD_MASK 0xff

#define FPGA_DONE_INT_CONTROL 0
#define FPGA_DONE_INT 6

#define U2X1 1
#define RXEN1 4
#define TXEN1 3
#define RXCIE1 7
#define TXCIE1 6
#define USBS1 3
#define UCSZ10 1
#define UDRE1 5
#define RXC1 7

#else

#include <avr/io.h>


#define P_FPGA_PROGRAM_B PF2

#define P_FPGA_POWER_SRAM PE5
#define P_FPGA_POWER_AUX PE4
#define P_FPGA_POWER_IO PE7
//SPI Slave Select input
#define P_FPGA_POWER_INT PB0

//SPI Bus Serial Clock
#define P_FPGA_CCLK PB1

#define P_FPGA_INIT_B PF3

//Output Compare and PWM Output B for Timer/Counter1
#define P_FPGA_DONE PB6

#define P_XMEM_WR PE0
#define P_XMEM_RD PE1
#define P_XMEM_ALE PE2

#define XMEM_A_MASK 0xff
#define XMEM_AD_MASK 0xff

//Erkennung von Pegelwechseln frei (wenn Eins) oder sperrt diese (wenn Null).
//Ein erkannter Pegelwechsel führt zum Setzen der Interruptanforderung PCIE0 in PCICR und schließlich zum Interrupt, wenn freigegeben.
#define FPGA_DONE_INT_CONTROL PCIE0
//pin 6 external interrupt source
#define FPGA_DONE_INT PCINT6

#define P_FLASH_CS PB4
#define P_WIRELESS_CS PE6

//leds

#define P_LED_0 PD7
#define P_LED_1 PD6
#define P_LED_2 PD5
#define P_LED_3 PD4

#endif

#endif //ELASTICNODEMIDDLEWARE_FPGAPINS_H
