#include <avr/io.h>
#include <util/delay.h>
#include "EmbeddedUtilities/BitManipulation.h"
#include <stdio.h>
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
/*
 * //have to be implemented by programmer
 * in uart
 *
 ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}
 * in reconfigure
 *
 ISR(FPGA_DONE_INT_VECTOR)
{
    reconfigure_interruptSR();
}
 */

int main(void)
{

   uart_Init(NULL);

   DDRD = 0xff;
  while (true)
  {
      uart_WriteChar('1');
      uart_WriteChar('c');
      uart_WriteChar('3');
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

ISR(USART1_RX_vect) {
    uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
    uart_ISR_Transmit();
}