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
uint8_t pin_fpga_done;

uint8_t* FPGA_DONE_INT_REG = &fpga_done_int_reg;
uint8_t* FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
uint8_t* PIN_FPGA_DONE = &pin_fpga_done;

uint8_t memoryarea[2000];
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

    fpgaMultibootClearComplete_internal_Expect();
    initMultiboot();

    TEST_ASSERT_EQUAL_UINT8((*ptr_fpgaDoneResponse), fpgaDoneResponse);
}

void test_initPtrMultiboot(void) {
    initalise_reconfigure_multiboot_mockRegister();
    initPtrMultiboot();
    TEST_ASSERT_EQUAL_UINT8((&multiboot), XMEM_OFFSET);
}

void test_fpgaMultiboot(void) {
    initalise_reconfigure_multiboot_mockRegister();
    uint32_t address = 0;

    elasticnode_fpgaPowerOn_Expect();

    //xmem mock with test flag!
    enableXmem_Expect();

    fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    fpgaMultibootClearComplete_internal_Expect();

    disableXmem_Expect();

    initPtrMultiboot_internal_Expect();

    sei_Expect();

    uint8_t tmp0 = (uint8_t) (0xff & (address >> (0 * 8)));
    TEST_ASSERT_EQUAL_UINT8((*(multiboot+0)), tmp0);

    uint8_t tmp1 = (uint8_t) (0xff & (address >> (1 * 8)));
    TEST_ASSERT_EQUAL_UINT8((*(multiboot+1)), tmp1);

    uint8_t tmp2 = (uint8_t) (0xff & (address >> (2 * 8)));
    TEST_ASSERT_EQUAL_UINT8((*(multiboot+2)), tmp2);

    fpgaMultiboot(address);
}

void test_fpgaMultibootClearComplete(void) {
    initalise_reconfigure_multiboot_mockRegister();

    fpgaMultibootClearComplete();

    uint8_t tmp = *ptr_fpgaMultibootCompleteFlag;
    TEST_ASSERT_EQUAL_UINT8(0, tmp);
}

void test_fpgaMultibootComplete(void) {
    initalise_reconfigure_multiboot_mockRegister();

    uint8_t expected = *ptr_fpgaMultibootCompleteFlag;
    uint8_t flag = fpgaMultibootComplete();

    TEST_ASSERT_EQUAL_UINT8(expected, flag);
}

void test_fpgaSetDoneReponse(void) {
    initalise_reconfigure_multiboot_mockRegister();

    uint8_t response = 123;
    fpgaSetDoneReponse(response);

    TEST_ASSERT_EQUAL_UINT8(response, (*ptr_fpgaMultibootCompleteFlag));
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
                fpgaMultiboot_internal_Expect(0);
                break;
            case 0:
            default:
                break;
        }
        sei_Expect();

    interruptSR();
    TEST_ASSERT_EQUAL_UINT8((*ptr_fpgaMultibootCompleteFlag), 1);
}