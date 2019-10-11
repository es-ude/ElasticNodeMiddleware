//
// Created by annika on 17.09.19.
//

#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/fpgaRegisters.h"
#include "elasticnodemiddleware/registerAbstraction.h"
#include "elasticnodemiddleware/xmem.h"




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

void elasticnode_configure(){

}

uint8_t elasticnode_getLoadedConfiguration(){

}

void elasticnode_writeDataBlocking(uint8_t* address, uint8_t data){

}

uint8_t elasticnode_readDataBlocking(){

}

void elasticnode_writeDataNonBlocking(uint8_t* address, uint8_t data){
    uint8_t* ptr = XMEM_OFFSET + (uint8_t)address;
    *ptr = data;
}

uint8_t elasticnode_readDataNonBlocking(uint8_t* address){
    volatile *ptr = XMEM_OFFSET + (uint8_t)address;
    return (*ptr);
}