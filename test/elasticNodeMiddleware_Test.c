//
// Created by annika on 17.09.19.
//
#include "unity.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"
#include "elasticnodemiddleware/Mockreconfigure_multiboot.h"

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

uint8_t memoryarea[2000];
const uint8_t* externalMockMemory = &memoryarea;

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

    //XMEM_OFFSET = &xmem_offset;
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

void test_elasticnode_configure() {
    initialise_mockRegister();
    uint32_t address = 0x23409;

    //Test not possible, flag
    //fpgaMultiboot_Expect(address);
    //elasticnode_configure(address);
}

void test_elasticnode_getConfiguration() {
    initialise_mockRegister();

    uint8_t address = elasticnode_getLoadedConfiguration();

    TEST_ASSERT_EQUAL_UINT8((&multiboot), address);
}

//block until it has written all of the data to the file
void test_elasticnode_writeDataBlocking(void){
    initialise_mockRegister();
    uint8_t address = 0;
    uint8_t data = 34;

    elasticnode_writeDataBlocking(address, data);

    TEST_ASSERT_EQUAL_UINT8((*ptr_xmem_offset), data);
}


// waits until at least one byte is available to return to the application
void test_elasticnode_readDataBlocking(void) {
    initialise_mockRegister();
    uint8_t address = 0;
    uint8_t size= 3;
    uint8_t destination;
    uint8_t* ptr_return = &destination;

    elasticnode_readDataBlocking(address, size, ptr_return);

    TEST_ASSERT_EQUAL_UINT8((*ptr_return), (*ptr_xmem_offset));
    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 1), (*ptr_xmem_offset + 1));
    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 2), (*ptr_xmem_offset  + 2));
}

void test_elasticnode_initReset_FPGA() {
    initialise_mockRegister();
    elasticnode_initReset_FPGA();
    TEST_ASSERT_EQUAL_UINT8((*reset_fpga), XMEM_OFFSET);
}
void test_elasticnode_setFpgaSoftReset(void) {
    initialise_mockRegister();
    elasticnode_setFpgaSoftReset();
    TEST_ASSERT_EQUAL_UINT8((*reset_fpga), 0x1);
}

void test_elasticnode_clearFpgaSoftReset(void) {
    initialise_mockRegister();
    elasticnode_clearFpgaSoftReset();
    TEST_ASSERT_EQUAL_UINT8((*reset_fpga), 0x0);
}

void test_fpgaSoftReset(void) {
    initialise_mockRegister();
    elasticnode_setFpgaSoftReset_internal_Expect();

    //in internal for testing
    _delay_ms_Expect(RESET_DELAY);

    elasticnode_clearFpgaSoftReset_internal_Expect();

    elasticnode_fpgaSoftReset();
}