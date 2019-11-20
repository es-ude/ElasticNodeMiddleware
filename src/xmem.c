//
// Created by annika on 01.10.19.
//

#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "EmbeddedUtilities/BitManipulation.h"

void initXmem(void) {

    /*XMCRA = (1 << SRW11 ) | (1 << SRW10);
    XMCRB = (1 << XMBK) | (1 << XMM1);*/
    //SRW10 in MCUCR

    BitManipulation_setBit(XMCRA, ((1 << SRW11) | (1 << SRW10)));
    BitManipulation_setBit(XMCRB, ((1 << XMBK) | (1 << XMM1)));

}

void enableXmem(void){

    //spi disable
    BitManipulation_setBit(SPCR, (1 << SPE));

    *DDR_XMEM_A = XMEM_A_MASK;
    *DDR_XMEM_ALE = XMEM_AD_MASK;

    BitManipulation_setBit(DDR_XMEM_WR, (1 << P_XMEM_WR));
    BitManipulation_setBit(DDR_XMEM_RD, (1 << P_XMEM_RD));
    BitManipulation_setBit(DDR_XMEM_ALE, (1 << P_XMEM_ALE));

    BitManipulation_setBit(XMEM_ENABLE_REG, (1 << XMEM_ENABLE_P));
}

void disableXmem(void){

    BitManipulation_clearBit(XMEM_ENABLE_REG, (1 << XMEM_ENABLE_P));

    *DDR_XMEM_A = 0x00;
    *DDR_XMEM_AD = 0x00;

    BitManipulation_clearBit(DDR_XMEM_WR, (1 << P_XMEM_WR));
    BitManipulation_setBit(PORT_XMEM_RD, (1 << P_XMEM_RD));
    BitManipulation_clearBit(DDR_XMEM_ALE, (1 << P_XMEM_ALE));
}