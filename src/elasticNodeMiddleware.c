//
// Created by annika on 17.09.19.
//

#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/reconfigure_multiboot.h"

//in header
//volatile uint8_t *reset_fpga = (uint8_t*) (XMEM_OFFSET + 0x04);


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

void elasticnode_fpgaPowerOn(){

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsHigh(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));

    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_SRAM, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_AUX, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_IO, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsLow(PORT_FPGA_POWER_INT, (1 << P_FPGA_POWER_INT));

    //single control --> register do not exist

    //disable interface
    abstraction_setRegisterBitsLow(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}

void elasticnode_fpgaPowerOff(){
    abstraction_setRegisterBitsLow(PORT_FPGA_PROGRAM_B, ( 1 << P_FPGA_PROGRAM_B));

    //enable interface
    abstraction_setRegisterBitsHigh(DDR_FPGA_CCLK, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_SRAM, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_SRAM, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_AUX, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_AUX, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_IO, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_IO, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh(DDR_FPGA_POWER_INT, ( 1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsHigh(PORT_FPGA_POWER_INT, ( 1 << P_FPGA_POWER_INT));

    abstraction_setRegisterBitsHigh(PORT_FPGA_PROGRAM_B, ( 1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsLow(DDR_FPGA_PROGRAM_B, (1 << P_FPGA_PROGRAM_B));
}
/* TODO: implement
void elasticnode_fpgaSleep(uint8_t sleepmode){
}*/

void elasticnode_configure(uint32_t address){
    reconfigure_fpgaMultiboot(address);
    while(!reconfigure_fpgaMultibootComplete());
}

uint8_t elasticnode_getLoadedConfiguration(){
    //letzte adresse zurückgeben
    multiboot = (uint8_t*) (XMEM_OFFSET + 0x05);
    return *multiboot;
}

void elasticnode_writeOneByteBlocking(uint8_t address, uint8_t data){
    ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET + address);
    for(uint8_t j=0; j<data; j++){
        *ptr_xmem_offset = data;
    }
}

void elasticnode_writeDataBlocking(uint8_t address, uint8_t size, uint8_t* ptr_data){
    ptr_xmem_offset = (uint8_t*) (XMEM_OFFSET + address);
    for(uint8_t k=0; k<size; k++) {
        *(ptr_xmem_offset+k) = *(ptr_data+k);
    }

}

uint8_t elasticnode_readOneByteBlocking(uint8_t address){
    ptr_xmem_offset = (uint8_t*) (XMEM_OFFSET + address);
    uint8_t byte = *ptr_xmem_offset;

    return byte;
}

void elasticnode_readDataBlocking(uint8_t address, uint8_t size, uint8_t* ptr_return){
    ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET + address);
    for(uint8_t i = 0; i < size; i++) {
        *(ptr_return + i) = *(ptr_xmem_offset + i);
    }
}

/*
void elasticnode_writeDataNonBlocking(uint8_t* address, uint8_t data){
}

uint8_t elasticnode_readDataNonBlocking(uint8_t* address){
}
*/


void elasticnode_initReset_FPGA() {
    reset_fpga = (uint8_t*) (XMEM_OFFSET + 0x04);
}
void elasticnode_setFpgaSoftReset(void){
    *reset_fpga = 0x1;
}

void elasticnode_clearFpgaSoftReset() {
    *reset_fpga = 0x0;
}

void elasticnode_fpgaSoftReset() {
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}