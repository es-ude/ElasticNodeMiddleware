//
// Created by annika on 17.09.19.
//
#include "unity.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"

uint8_t port_fpga_program_b;
uint8_t ddr_fpga_program_b;
uint8_t p_fpga_program_b;

uint8_t fpga_power_control_multi;

uint8_t port_fpga_program_sram;
uint8_t ddr_fpga_program_sram;
uint8_t p_fpga_program_sram;
uint8_t port_fpga_program_aux;
uint8_t ddr_fpga_program_aux;
uint8_t p_fpga_program_aux;
uint8_t port_fpga_program_io;
uint8_t ddr_fpga_program_io;
uint8_t p_fpga_program_io;
uint8_t port_fpga_program_int;
uint8_t ddr_fpga_program_int;
uint8_t p_fpga_program_int;

void test_elasticnode_initialise(void) {

    elasticnode_initialise();
}

void test_elasticnode_fpgaPowerOn(void) {

    //set pin high of program B
    abstraction_setRegisterBitsHigh_Expect(port_fpga_program_b, (1 << p_fpga_program_b));
    abstraction_setRegisterBitsHigh_Expect(ddr_fpga_program_b, (1 << p_fpga_program_b));

    // ELSE CASE --> REGISTER NOT FOUNDED
    #warning "FPGA Power Control Multi"
    abstraction_setRegisterBitsHigh_Expect(ddr_fpga_program_sram, (1 << p_fpga_program_sram));
    abstraction_setRegisterBitsLow_Expect(port_fpga_program_sram, (1 << p_fpga_program_sram));
    abstraction_setRegisterBitsHigh_Expect(ddr_fpga_program_aux, (1 << p_fpga_program_aux));
    abstraction_setRegisterBitsLow_Expect(port_fpga_program_aux, (1 << p_fpga_program_aux));
    abstraction_setRegisterBitsHigh_Expect(ddr_fpga_program_io, (1 << p_fpga_program_io));
    abstraction_setRegisterBitsLow_Expect(port_fpga_program_io, (1 << port_fpga_program_io));
    abstraction_setRegisterBitsHigh_Expect(ddr_fpga_program_int, (1 << p_fpga_program_int));
    abstraction_setRegisterBitsLow_Expect(port_fpga_program_int, (1 << p_fpga_program_int));

    #warning "FPGA Power Control Single"

    //register gibt es nicht

    //function fehlt!!!

    abstraction_setRegisterBitsHigh_Expect(port_fpga_program_b, (1 << p_fpga_program_b));

    elasticnode_fpgaPowerOn();
}

void test_elasticnode_fpgaPowerOff(void) {

    elasticnode_fpgaPowerOff();
}