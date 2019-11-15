//
// Created by annika on 10.10.19.
//
#include "unity.h"
#include "elasticnodemiddleware/reconfigure_multiboot_avr.h"
#include "elasticnodemiddleware/fpgaPins.h"
#include "elasticnodemiddleware/MockregisterAbstraction.h"
#include "elasticnodemiddleware/Mockreconfigure_multiboot_internal_avr.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware.h"
#include "elasticnodemiddleware/Mockxmem.h"



uint8_t fpga_done_int_reg;
uint8_t fpga_done_int_control_reg;
uint8_t pin_fpga_done;

uint8_t* FPGA_DONE_INT_REG = &fpga_done_int_reg;
uint8_t* FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
uint8_t* PIN_FPGA_DONE = &pin_fpga_done;

uint8_t memoryarea[2000];
const uint8_t* externalMockMemory = &memoryarea;

extern volatile uint8_t fpgaDoneResponse;
extern volatile uint8_t *multiboot;

void initalise_reconfigure_multiboot_mockRegister(void) {
    FPGA_DONE_INT_REG = &fpga_done_int_reg;
    FPGA_DONE_INT_CONTROL_REG = &fpga_done_int_control_reg;
    PIN_FPGA_DONE = &pin_fpga_done;
}

void writeMultiboot(uint32_t address) {

    //32 address
    TEST_ASSERT_EQUAL_UINT8(*(multiboot+0), (uint8_t) (0xff & (address >> (0 * 8))));

    TEST_ASSERT_EQUAL_UINT8(*(multiboot+0), (uint8_t) (0xff & (address >> (1 * 8))));

    TEST_ASSERT_EQUAL_UINT8(*(multiboot+0), (uint8_t) (0xff & (address >> (2 * 8))));
}

void test_initMultiboot(void) {
    initalise_reconfigure_multiboot_mockRegister();

    abstraction_setRegisterBitsLow_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_REG, (1 << FPGA_DONE_INT));
    abstraction_setRegisterBitsHigh_Expect(FPGA_DONE_INT_CONTROL_REG, (1 << FPGA_DONE_INT_CONTROL));

    reconfigure_fpgaMultibootClearComplete_internal_Expect();
    reconfigure_initMultiboot();

    TEST_ASSERT_EQUAL_UINT8(FPGA_DONE_NOTHING, fpgaDoneResponse);
}

void test_reconfigure_fpgaMultiboot(void) {
    uint32_t address = 0;

    elasticnode_fpgaPowerOn_Expect();

    //xmem mock with test flag!
    enableXmem_Expect();

    reconfigure_fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal_Expect();

    disableXmem_Expect();

    sei_Expect();

    reconfigure_fpgaMultiboot(address);

    //test for-loop in new function
    writeMultiboot(address);
}
void test_getMultibootAddress(void){

    uint32_t address = (uint32_t) (*(multiboot) + *(multiboot+1) + *(multiboot+2));
    TEST_ASSERT_EQUAL_UINT32(address, reconfigure_getMultibootAddress());
}

void test_reconfigure_fpgaMultibootComplete() {
    //beliebige Zahl?
    reconfigure_fpgaMultibootComplete_internal_ExpectAndReturn(0);
    reconfigure_fpgaMultibootComplete();
}

void test_interruptSR(void) {
    initalise_reconfigure_multiboot_mockRegister();

    abstraction_getBit_ExpectAndReturn(PIN_FPGA_DONE, P_FPGA_DONE, 1);
    reconfigure_fpgaSetDoneReponse_internal_Expect(1);
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
                //reconfigure_fpgaMultiboot(0);
                break;
            case 0:
            default:
                break;
        }
        sei_Expect();

    reconfigure_interruptSR();
}