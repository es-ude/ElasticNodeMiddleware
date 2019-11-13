//
// Created by annika on 07.11.19.
//

#include "unity.h"
#include "elasticnodemiddleware/reconfigure_multiboot_internal_avr.h"

void test_reconfigure_fpgaMultibootClearComplete_internal(void) {

    reconfigure_fpgaMultibootClearComplete_internal();

    TEST_ASSERT_EQUAL_UINT8(0, fpgaMultibootCompleteState);
}

void test_reconfigure_fpgaSetDoneReponse_internal(void) {

    uint8_t response = 123;
    reconfigure_fpgaSetDoneReponse_internal(response);

    TEST_ASSERT_EQUAL_UINT8(response, fpgaMultibootCompleteState);
}


//test besser benennen, 2 tests
void test_fpgaMultibootCompleteState(void) {

    fpgaMultibootCompleteState = FPGA_DONE_NOTHING;
    uint8_t state = reconfigure_fpgaMultibootComplete_internal();

    TEST_ASSERT_EQUAL_UINT8(FPGA_DONE_NOTHING, state);
}

