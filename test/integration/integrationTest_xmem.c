#include "EmbeddedUtilities/BitManipulation.h"

#include "src/xmem/xmem.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/debug/debug.h"

//test if Bits are set
//4th, 5th and 6th LED on
int main() {
    debugInit(NULL);
    DDRD = 0xff;
    debugWriteLine("integration test xmem");

    //xmem_initXmem();
    //if(BitManipulation_bitIsSetOnArray(XMCRA, (SRW11 | SRW10)) && BitManipulation_bitIsSetOnArray(XMCRB, (XMBK | XMM1))) {
    //    BitManipulation_setBit(&PORTD, PD4);
    //    debugWriteLine("xmem_initXmem() successful");
    //}

    xmem_enableXmem();
    if (!BitManipulation_bitIsSetOnArray(SPCR, SPE) && BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR) &&
        BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) &&
        BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE) &&
        (*DDR_XMEM_A == XMEM_A_MASK) && (*DDR_XMEM_ALE == XMEM_AD_MASK)) {
        BitManipulation_setBit(&PORTD, PD5);
        debugWriteLine("xmem_emableXmem() successful");
    }

    xmem_disableXmem();
    if (!(BitManipulation_bitIsSetOnArray(DDR_XMEM_WR, P_XMEM_WR)) &&
        BitManipulation_bitIsSetOnArray(DDR_XMEM_RD, P_XMEM_RD) &&
        !(BitManipulation_bitIsSetOnArray(DDR_XMEM_ALE, P_XMEM_ALE)) && (*DDR_XMEM_A == 0x00) &&
        (*DDR_XMEM_AD == 0x00)) {
        BitManipulation_setBit(&PORTD, PD6);
        debugWriteLine("xmem_disableXmem() successful");
    }

}
