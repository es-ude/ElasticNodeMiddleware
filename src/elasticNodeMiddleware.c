//
// Created by annika on 17.09.19.
//

#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/reconfigure_multiboot_avr.h"

extern volatile uint8_t* ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET);

void elasticnode_initialise(){
    //initalise fpga

    elasticnode_fpgaPowerOn_internal();

    //enable interface
    abstraction_setRegisterBitsHigh(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    // inputs that only get setup once
    abstraction_setRegisterBitsHigh(PORT_FPGA_INIT_B, ( 1 << P_FPGA_INIT_B));
    abstraction_setRegisterBitsLow(DDR_FPGA_INIT_B, (1 << P_FPGA_INIT_B));
    abstraction_setRegisterBitsHigh(PORT_FPGA_INIT_B, ( 1 << P_FPGA_INIT_B));

    abstraction_setRegisterBitsLow(DDR_FPGA_DONE, (1<<P_FPGA_DONE));
    abstraction_setRegisterBitsHigh(PORT_FPGA_DONE, (1<<P_FPGA_DONE));

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1<<P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsLow(DDR_FPGA_PROGRAM_B, (1<<P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1<<P_FPGA_PROGRAM_B));

}

void elasticnode_fpgaPowerOn() {
    elasticnode_fpgaPowerOn_internal();
}

void elasticnode_fpgaPowerOff() {
    elasticnode_fpgaPowerOff_internal();
}


/* TODO: implement
void elasticnode_fpgaSleep(uint8_t sleepmode){
}*/

void elasticnode_configureFrom(uint32_t address){
    reconfigure_fpgaMultiboot(address);
    while(!reconfigure_fpgaMultibootComplete());
}

uint32_t elasticnode_getLoadedConfiguration(){
    //letzte adresse zurückgeben
    return reconfigure_getMultibootAddress();
}

//lokale vaiable volatile die drauf zeigt
void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data){
    *(ptr_xmem_offset + address) = data;
}

void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data){
    ptr_xmem_offset = ptr_xmem_offset + address;
    for(uint8_t k=0; k<size; k++) {
        *(ptr_xmem_offset+k) = *(ptr_data+k);
    }
}

uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address){;
    return *(ptr_xmem_offset + address);
}

void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return){
    ptr_xmem_offset = ptr_xmem_offset + address;
    for(uint8_t i = 0; i < size; i++) {
        *(ptr_return + i) = *(ptr_xmem_offset + i);
    }
}

void elasticnode_fpgaSoftReset() {
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}