#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "EmbeddedUtilities/BitManipulation.h"

#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"
#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"
#include "src/xmem/xmem.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"


//volatile uint8_t* ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET);
volatile uint8_t *xmemOffset = (uint8_t *) (XMEM_OFFSET);
volatile uint8_t *userLogicOffsetAddr = (uint8_t *) (XMEM_USERLOGIC_OFFSET);

void elasticnode_initialise(void) {
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

//TODO: Put these magic number offset values to somewhere more meaningful
#define USERLOGIC_RESET_OFFSET 0x04
#define USERLOGIC_RESET_VALUE 0x0

void elasticnode_enableFpgaInterface(void) {
    xmem_enableXmem();
}

void elasticnode_disableFpgaInterface(void) {
    xmem_disableXmem();
}

void elasticnode_configureFPGA(uint32_t address){
    reconfigure_fpgaMultiboot(address);
    while(!reconfigure_fpgaMultibootComplete());
}

uint32_t elasticnode_getLoadedConfigurationAddress(void){
    return reconfigure_getMultibootAddress();
}
void elasticnode_writeByteToUserlogic(uint8_t userlogicAddr, uint8_t data) {
    *(userLogicOffsetAddr + userlogicAddr) = data;
}

void elasticnode_writeBufferToUserlogic(uint8_t userlogicAddr, uint16_t size, const uint8_t *buffer) {
    for (uint16_t i = 0; i < size; i++) {
        *(userLogicOffsetAddr + userlogicAddr + i) = *(buffer + i);
    }
}

uint8_t elasticnode_readByteFromUserlogic(uint8_t userlogicAddr) {
    return *(userLogicOffsetAddr + userlogicAddr);
}

void elasticnode_readBufferFromUserlogic(uint8_t userlogicAddr, uint16_t size, uint8_t *buffer) {
    for (uint16_t i = 0; i < size; i++) {
        *(buffer + i) = *(userLogicOffsetAddr + userlogicAddr + i);
    }
}


//void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data) {
//    *(ptr_xmem_offset + address) = data;
//}
//void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data) {
//    uint8_t *targetAddress = ptr_xmem_offset + address;
//    for (uint8_t k = 0; k < size; k++) {
//        *(targetAddress + k) = *(ptr_data + k);
//    }
//}
//
//uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address) {
//    return *(ptr_xmem_offset + address);
//}
//
//void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return) {
//    ptr_xmem_offset = ptr_xmem_offset + address;
//    for (uint8_t i = 0; i < size; i++) {
//        *(ptr_return + i) = *(ptr_xmem_offset + i);
//    }
//}

void elasticnode_userlogicReset(void) {
    //this resets the userlogic
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}

void elasticnode_fpgaHardReset(void) {
    // This resets the FPGA using gpio pins
    elasticnode_setFpgaHardReset_internal();
    elasticnode_clearFpgaHardReset_internal();
}