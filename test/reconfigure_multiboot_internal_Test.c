//
// Created by annika on 07.11.19.
//

#include "unity.h"
#include "elasticnodemiddleware/reconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/MockelasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/Mockxmem.h"


void test_reconfigure_fpgaMultibootClearComplete_internal(void) {
    //initalise_reconfigure_multiboot_mockRegister();

    reconfigure_fpgaMultibootClearComplete_internal();

    //TEST_ASSERT_EQUAL_UINT8(0, *ptr_fpgaMultibootCompleteFlag);
}

void test_reconfigure_fpgaSetDoneReponse_internal(void) {
    //initalise_reconfigure_multiboot_mockRegister();

    uint8_t response = 123;
    reconfigure_fpgaSetDoneReponse_internal(response);

    //TEST_ASSERT_EQUAL_UINT8(response, (*ptr_fpgaMultibootCompleteFlag));
}


void test_reconfigure_initPtrMultiboot_internal(void) {
    //initalise_reconfigure_multiboot_mockRegister();

    reconfigure_initPtrMultiboot_internal();
    //TEST_ASSERT_EQUAL_PTR(multiboot, (XMEM_OFFSET + 0x05));

}
/*
void test_reconfigure_fpgaMultiboot_internal(void) {
    //initalise_reconfigure_multiboot_mockRegister();
    uint32_t address = 0;

    elasticnode_fpgaPowerOn_internal_Expect();

    //xmem mock with test flag!
    enableXmem_Expect();

    reconfigure_fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal_Expect();

    disableXmem_Expect();

    reconfigure_initPtrMultiboot_internal_Expect();

    //hinzufügen!
    //sei_Expect();

    //umbennen, funktion schreiben, 32 address
    uint8_t tmp0 = (uint8_t) (0xff & (address >> (0 * 8)));
    //TEST_ASSERT_EQUAL_UINT8((*(multiboot+0)), tmp0);

    uint8_t tmp1 = (uint8_t) (0xff & (address >> (1 * 8)));
    //TEST_ASSERT_EQUAL_UINT8((*(multiboot+1)), tmp1);

    uint8_t tmp2 = (uint8_t) (0xff & (address >> (2 * 8)));
    //TEST_ASSERT_EQUAL_UINT8((*(multiboot+2)), tmp2);

    reconfigure_fpgaMultiboot_internal(address);
}*/