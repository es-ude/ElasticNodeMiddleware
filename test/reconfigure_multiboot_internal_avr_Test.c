#include "unity.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_internal_avr.h"

extern volatile uint8_t fpgaMultibootCompleteState;

void test_reconfigure_fpgaMultibootClearComplete_internal(void) {

    reconfigure_fpgaMultibootClearComplete_internal();

    TEST_ASSERT_EQUAL_UINT8(0, fpgaMultibootCompleteState);
}

void test_reconfigure_fpgaSetDoneReponse_internal(void) {

    uint8_t response = 123;
    reconfigure_fpgaSetDoneReponse_internal(response);

    TEST_ASSERT_EQUAL_UINT8(response, fpgaMultibootCompleteState);
}

void test_fpgaMultibootCompleteState_Return(void) {

    fpgaMultibootCompleteState = FPGA_DONE_NOTHING;
    TEST_ASSERT_EQUAL_UINT8(FPGA_DONE_NOTHING, reconfigure_fpgaMultibootComplete_internal());
}

