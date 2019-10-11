//
// Created by annika on 10.10.19.
//
#include "unity.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"
#include "elasticnodemiddleware/Mockreconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware.h"
#include "elasticnodemiddleware/Mockxmem.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/reconfigure_multiboot.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"

uint8_t fpga_done_int_reg;
uint8_t fpga_done_int_control_reg;

uint8_t* FPGA_DONE_INT_REG = &fpga_done_int_reg;
uint8_t* FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;

volatile uint8_t fpgaDoneResponse;
//XMEM!!!
//volatile uint8_t *multiboot = (uint8_t *) (XMEM_OFFSET + 0x05);

void initalise_reconfigure_multiboot_mockRegister(void) {
    FPGA_DONE_INT_REG = &fpga_done_int_reg;
    FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
}

void test_initMultiboot(void) {
    initalise_reconfigure_multiboot_mockRegister();

    abstraction_setRegisterBitsLow_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    fpgaMultibootClearComplete_internal_Expect();
    fpgaDoneResponse = FPGA_DONE_NOTHING;
    initMultiboot();
}

void test_fpgaMultiboot(void) {
    uint32_t address = 0;

    elasticnode_fpgaPowerOn_Expect();

    //xmem mock with test flag!
    enableXmem_Expect();

    fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    fpgaMultibootClearComplete_internal_Expect();

    //for loop --> how test?
    /*uint8_t tmp0 = (uint8_t) (0xff & (address >> (0 * 8)));

    TEST_ASSERT_EQUAL_UINT8((*(multiboot)), tmp0);*/

    disableXmem_Expect();

    fpgaMultiboot(address);
}

void test_fpgaMultibootClearComplete(void) {

    fpgaMultibootClearComplete();

    uint8_t tmp = *ptr_fpgaMultibootCompleteFlag;
    TEST_ASSERT_EQUAL_UINT8(0, tmp);
}

void test_fpgaMultibootComplete(void) {

    uint8_t expected = *ptr_fpgaMultibootCompleteFlag;
    uint8_t flag = fpgaMultibootComplete();

    TEST_ASSERT_EQUAL_UINT8(expected, flag);
}

void test_fpgaSetDoneReponse(void) {
    uint8_t response = 123;
    fpgaSetDoneReponse(response);

    TEST_ASSERT_EQUAL_UINT8(response, (*ptr_fpgaMultibootCompleteFlag));
}