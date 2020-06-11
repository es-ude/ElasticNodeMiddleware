//
// Created by annika on 17.09.19.
//

#include "lib/elasticNodeMiddleware/elasticNodeMiddleware.h"
#include "lib/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "lib/pinDefinition/fpgaPins.h"
#include "lib/pinDefinition/fpgaRegisters.h"
#include "lib/xmem/xmem.h"
#include "EmbeddedUtilities/BitManipulation.h"

volatile uint8_t* ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET);

void elasticnode_initialise(void){
    //initalise fpga

    elasticnode_fpgaPowerOn_internal();

    //enable interface
    BitManipulation_setBit(DDR_FPGA_CCLK, P_FPGA_CCLK);

    // inputs that only get setup once
    BitManipulation_setBit(PORT_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_clearBit(DDR_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_setBit(PORT_FPGA_INIT_B, P_FPGA_INIT_B);

    BitManipulation_clearBit(DDR_FPGA_DONE, P_FPGA_DONE);
    BitManipulation_setBit(PORT_FPGA_DONE, P_FPGA_DONE);

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);

}

void elasticnode_fpgaPowerOn(void) {
    elasticnode_fpgaPowerOn_internal();
}

void elasticnode_fpgaPowerOff(void) {
    elasticnode_fpgaPowerOff_internal();
}

void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data){
    *(ptr_xmem_offset + address) = data;
}

void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data){
    ptr_xmem_offset = ptr_xmem_offset + address;
    for(uint8_t k=0; k<size; k++) {
        *(ptr_xmem_offset+k) = *(ptr_data+k);
    }
}

uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address){
    return *(ptr_xmem_offset + address);
}

void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return){
    ptr_xmem_offset = ptr_xmem_offset + address;
    for(uint8_t i = 0; i < size; i++) {
        *(ptr_return + i) = *(ptr_xmem_offset + i);
    }
}

void elasticnode_fpgaSoftReset(void) {
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}

void elasticnode_fpgaHardReset(void) {
//    xmem_disableXmem();

    elasticnode_setFpgaHardReset_internal();
//    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaHardReset_internal();
}