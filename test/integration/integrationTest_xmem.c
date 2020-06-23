#include "lib/xmem/xmem.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/uart/uart.h"
#include <avr/interrupt.h>
//test if Bits are set
//4th, 5th and 6th LED on
int main()
{
    uart_Init(NULL);
    DDRD = 0xff;
    uart_WriteLine("integration test xmem");
    uart_WaitUntilDone();

    xmem_initXmem();
    if(BitManipulation_bitIsSetOnArray(XMCRA, (SRW11 | SRW10)) && BitManipulation_bitIsSetOnArray(XMCRB, (XMBK | XMM1))) {
        BitManipulation_setBit(&PORTD, PD4);
        uart_WriteLine("xmem_initXmem() successful");
        uart_WaitUntilDone();
    }

    xmem_enableXmem();
    if(!BitManipulation_bitIsSetOnArray(SPCR, SPE) && BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR) &&
        BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) && BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE) &&
        (*DDR_XMEM_A == XMEM_A_MASK) && (*DDR_XMEM_ALE == XMEM_AD_MASK)) {
        BitManipulation_setBit(&PORTD, PD5);
        uart_WriteLine("xmem_emableXmem() successful");
        uart_WaitUntilDone();
    }

    xmem_disableXmem();
    if(!(BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR)) && BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) &&
        !(BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE)) && (*DDR_XMEM_A == 0x00) && (*DDR_XMEM_AD == 0x00)) {
        BitManipulation_setBit(&PORTD, PD6);
        uart_WriteLine("xmem_disableXmem() successful");
        uart_WaitUntilDone();
    }

}

ISR(USART1_RX_vect) {
        uart_ISR_Receive();
}

ISR(USART1_TX_vect) {
        uart_ISR_Transmit();
}