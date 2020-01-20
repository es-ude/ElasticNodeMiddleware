//
// Created by annika on 18.12.19.
//
#include "lib/xmem/xmem.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "EmbeddedUtilities/BitManipulation.h"
#include "lib/uart/uart.h"
//test if Bits are set
//4th, 5th and 6th LED on
int main()
{
    uart_Init(NULL);
	uart_WriteLine("Starting...");

    DDRD = 0xff;
    xmem_initXmem();
    if(BitManipulation_bitIsSetOnArray(XMCRA, (SRW11 | SRW10)) && BitManipulation_bitIsSetOnArray(XMCRB, (XMBK | XMM1))) {
        BitManipulation_setBit(&PORTD, PD4);
    }

    xmem_enableXmem();
    if(BitManipulation_bitIsSetOnArray(SPCR, SPE) && BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR) &&
    BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) && BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE) &&
    BitManipulation_bitIsSetOnArray(XMEM_ENABLE_REG, XMEM_ENABLE_P) && (*DDR_XMEM_A == XMEM_A_MASK) && (*DDR_XMEM_ALE == XMEM_AD_MASK)) {
        BitManipulation_setBit(&PORTD, PD5);
    }

    xmem_disableXmem();
    if(!(BitManipulation_bitIsSetOnArray(XMEM_ENABLE_REG, XMEM_ENABLE_P)) && !(BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR)) &&
       BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) && !(BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE)) &&
        (*DDR_XMEM_A == 0x00) && (*DDR_XMEM_AD == 0x00)) {
        BitManipulation_setBit(&PORTD, PD6);
    }

}