//
// Created by annika on 17.09.19.
//
#include "unity.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"


uint8_t port_fpga_program_b;
uint8_t ddr_fpga_program_b;

uint8_t port_fpga_power_sram;
uint8_t ddr_fpga_power_sram;
uint8_t port_fpga_power_aux;
uint8_t ddr_fpga_power_aux;
uint8_t port_fpga_power_io;
uint8_t ddr_fpga_power_io;
uint8_t port_fpga_power_int;
uint8_t ddr_fpga_power_int;

uint8_t ddr_fpga_cclk;

uint8_t port_fpga_init_b;
uint8_t ddr_fpga_init_b;

uint8_t port_fpga_done;
uint8_t ddr_fpga_done;

uint8_t xmem_offset;

uint8_t* DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
uint8_t* PORT_FPGA_PROGRAM_B = &port_fpga_program_b;

uint8_t* DDR_FPGA_POWER_SRAM =&ddr_fpga_power_sram;
uint8_t* PORT_FPGA_POWER_SRAM =&port_fpga_power_sram;
uint8_t* DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
uint8_t* PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
uint8_t* DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
uint8_t* PORT_FPGA_POWER_IO = &port_fpga_power_io;
uint8_t* DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
uint8_t* PORT_FPGA_POWER_INT = &port_fpga_power_int;

uint8_t* DDR_FPGA_CCLK = &ddr_fpga_cclk;

uint8_t* DDR_FPGA_INIT_B = &ddr_fpga_init_b;
uint8_t* PORT_FPGA_INIT_B=&port_fpga_init_b;

uint8_t* DDR_FPGA_DONE = &ddr_fpga_done;
uint8_t* PORT_FPGA_DONE = &port_fpga_done;

uint8_t* XMEM_OFFSET = &xmem_offset;

void initialise_mockRegister(void) {
    DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
    PORT_FPGA_PROGRAM_B = &port_fpga_program_b;

    DDR_FPGA_POWER_SRAM =&ddr_fpga_power_sram;
    PORT_FPGA_POWER_SRAM =&port_fpga_power_sram;
    DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
    PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
    DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
    PORT_FPGA_POWER_IO = &port_fpga_power_io;
    DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
    PORT_FPGA_POWER_INT = &port_fpga_power_int;

    DDR_FPGA_CCLK = &ddr_fpga_cclk;

    PORT_FPGA_INIT_B = &port_fpga_init_b;
    DDR_FPGA_INIT_B= &ddr_fpga_init_b;

    DDR_FPGA_DONE = &ddr_fpga_done;
    PORT_FPGA_DONE = &port_fpga_done;

    XMEM_OFFSET = &xmem_offset;
}

void test_elasticnode_initialise(void) {
    initialise_mockRegister();


    //fpgaPowerOn
    elasticnode_fpgaPowerOn_internal_Expect();

    //single control --> register do not exist

    //enable interface
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_cclk, (1 << P_FPGA_CCLK));

    // inputs that only get setup once
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_init_b, ( 1 << P_FPGA_INIT_B));
    abstraction_setRegisterBitsLow_Expect(&ddr_fpga_init_b, (1 << P_FPGA_INIT_B));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_init_b, ( 1 << P_FPGA_INIT_B));

    abstraction_setRegisterBitsLow_Expect(&ddr_fpga_done, (1<<P_FPGA_DONE));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_done, (1<<P_FPGA_DONE));

    abstraction_setRegisterBitsHigh_Expect(&port_fpga_program_b, (1<<P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsLow_Expect(&ddr_fpga_program_b, (1<<P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_program_b, (1<<P_FPGA_PROGRAM_B));

    elasticnode_initialise();
}

void test_elasticnode_fpgaPowerOn(void) {
    initialise_mockRegister();

    //set pin high of program B
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_program_b, (1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_program_b, (1 << P_FPGA_PROGRAM_B));

    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_sram, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsLow_Expect(&port_fpga_power_sram, (1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_aux, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsLow_Expect(&port_fpga_power_aux, (1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_io, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsLow_Expect(&port_fpga_power_io, (1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_int, (1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsLow_Expect(&port_fpga_power_int, (1 << P_FPGA_POWER_INT));

    //single control --> register do not exist

    //disable interface
    abstraction_setRegisterBitsLow_Expect(&ddr_fpga_cclk, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh_Expect(&port_fpga_program_b, (1 << P_FPGA_PROGRAM_B));

    elasticnode_fpgaPowerOn();
}

void test_elasticnode_fpgaPowerOff(void) {
    initialise_mockRegister();

    abstraction_setRegisterBitsLow_Expect(&port_fpga_program_b, ( 1 << P_FPGA_PROGRAM_B));

    //enable interface
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_cclk, (1 << P_FPGA_CCLK));

    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_sram, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_power_sram, ( 1 << P_FPGA_POWER_SRAM));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_aux, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_power_aux, ( 1 << P_FPGA_POWER_AUX));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_io, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_power_io, ( 1 << P_FPGA_POWER_IO));
    abstraction_setRegisterBitsHigh_Expect(&ddr_fpga_power_int, ( 1 << P_FPGA_POWER_INT));
    abstraction_setRegisterBitsHigh_Expect(&port_fpga_power_int, ( 1 << P_FPGA_POWER_INT));

    abstraction_setRegisterBitsHigh_Expect(&port_fpga_program_b, ( 1 << P_FPGA_PROGRAM_B));
    abstraction_setRegisterBitsLow_Expect(&ddr_fpga_program_b, (1 << P_FPGA_PROGRAM_B));

    elasticnode_fpgaPowerOff();
}

//block until it has written all of the data to the file
void test_elasticnode_writeDataBlocking(void){

    //elasticnode_writeDataBlocking();

}


// waits until at least one byte is available to return to the application
void test_elasticnode_readDataBlocking(void) {

    //elasticnode_readDataBlocking();

}

//write as much data into the file as it can
//idea: pointer auf xmem_offset, im test initialisieren, überprüfen, ob data hingeschrieben
void test_elasticnode_writeDataNonBlocking(void) {

    //FAIL
    uint8_t* address = 0;
    uint8_t data = 45;

    elasticnode_writeDataNonBlocking(address, data);

    uint8_t* ptr = XMEM_OFFSET + (uint8_t)address;
    TEST_ASSERT_EQUAL_UINT8((*ptr), data);
}


//turn immediately, even if no bytes are available
void test_elasticnode_readDataNonBlocking(void) {
    //FAIL
    uint8_t* address = 0;

    uint8_t data = elasticnode_readDataNonBlocking(address);

    volatile *ptr = XMEM_OFFSET + (uint8_t)address;
    TEST_ASSERT_EQUAL_UINT8((*ptr), data);
}