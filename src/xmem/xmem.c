#include "EmbeddedUtilities/BitManipulation.h"

#include "src/xmem/xmem.h"

#include "src/pinDefinition/fpgaRegisters.h"
#include "src/pinDefinition/fpgaPins.h"

#include "src/delay/delay.h"

// TODO: Necessary?

void xmem_initXmem(void) {
    //SRW10 in MCUCR

    BitManipulation_setBit(XMEM_XMCRA, XMEM_SRW11);
    BitManipulation_setBit(XMEM_XMCRA, XMEM_SRW10);

    BitManipulation_setBit(XMEM_XMCRB, XMEM_XMBK);
    BitManipulation_setBit(XMEM_XMCRB, XMEM_XMM1);

    //XMCRA = (1 << SRW11) | (1 << SRW10);
    //XMCRB = (1 << XMBK) | (1 << XMM1);
}


uint16_t xmem_offset(void) {
    return (uint16_t) XMEM_OFFSET;
}

void xmem_enableXmem(void) {

    //spi disable
    BitManipulation_clearBit(SPCR_XMEM, SPE);
    //SPCR &= ~(1 << SPE);

    *DDR_XMEM_A = XMEM_A_MASK;
    *DDR_XMEM_ALE = XMEM_AD_MASK;

    BitManipulation_setBit(DDR_XMEM_WR, P_XMEM_WR);
    BitManipulation_setBit(DDR_XMEM_RD, P_XMEM_RD);
    BitManipulation_setBit(DDR_XMEM_ALE, P_XMEM_ALE);

    BitManipulation_setBit(XMEM_ENABLE_REG, XMEM_ENABLE_P);
    //XMEM_ENABLE_REG |= (1 << XMEM_ENABLE_P);
}

void xmem_disableXmem(void) {

    BitManipulation_clearBit(XMEM_ENABLE_REG, XMEM_ENABLE_P);
    //XMEM_ENABLE_REG &= ~(1 << XMEM_ENABLE_P);

    *DDR_XMEM_A = 0x00;
    *DDR_XMEM_AD = 0x00;

    BitManipulation_clearBit(DDR_XMEM_WR, P_XMEM_WR);
    BitManipulation_setBit(PORT_XMEM_RD, P_XMEM_RD);
    BitManipulation_clearBit(DDR_XMEM_ALE, P_XMEM_ALE);
}
