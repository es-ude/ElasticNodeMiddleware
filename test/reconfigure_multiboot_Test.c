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
#include "elasticnodemiddleware/MockelasticNodeMiddleware_internal.h"

uint8_t fpga_done_int_reg;
uint8_t fpga_done_int_control_reg;
uint8_t pin_fpga_done;

uint8_t* FPGA_DONE_INT_REG = &fpga_done_int_reg;
uint8_t* FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
uint8_t* PIN_FPGA_DONE = &pin_fpga_done;

uint8_t memoryarea; //[2000]; why array?
const uint8_t* externalMockMemory = &memoryarea;

void initalise_reconfigure_multiboot_mockRegister(void) {
    FPGA_DONE_INT_REG = &fpga_done_int_reg;
    FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
    PIN_FPGA_DONE = &pin_fpga_done;
}

void test_initMultiboot(void) {
    initalise_reconfigure_multiboot_mockRegister();

    abstraction_setRegisterBitsLow_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    reconfigure_fpgaMultibootClearComplete_internal_Expect();
    reconfigure_initMultiboot();

    TEST_ASSERT_EQUAL_UINT8((*ptr_fpgaDoneResponse), fpgaDoneResponse);
}

//test besser benennen, ohne pointer, 2 tests
void test_fpgaMultibootComplete(void) {
    initalise_reconfigure_multiboot_mockRegister();

    //uint8_t expected = *ptr_fpgaMultibootCompleteFlag;
    //uint8_t flag = reconfigure_fpgaMultibootComplete();

    //TEST_ASSERT_EQUAL_UINT8(expected, flag);
}

void test_interruptSR(void) {
    initalise_reconfigure_multiboot_mockRegister();

    abstraction_getBit_ExpectAndReturn(PIN_FPGA_DONE, P_FPGA_DONE, 1);

        switch (fpgaDoneResponse) {
            case FPGA_DONE_PRINT:

                cli_Expect();

                //TODO: implement for debugging

                //debugWriteLine("FPGA Done INT");
                //debugWriteFloatFull(duration);
                //debugNewLine();
                //debugDone();

                break;
            case FPGA_DONE_MULTIBOOT:

                elasticnode_fpgaSoftReset_Expect();
                reconfigure_fpgaMultiboot_internal_Expect(0);
                break;
            case 0:
            default:
                break;
        }
        sei_Expect();

    reconfigure_interruptSR();
    //TEST_ASSERT_EQUAL_UINT8((*ptr_fpgaMultibootCompleteFlag), 1);
}