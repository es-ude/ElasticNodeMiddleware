#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"

#include "src/pinDefinition/fpgaPins.h"

//#include "src/reconfigure_multiboot_avr/Mockreconfigure_multiboot_internal_avr.h"
#include "src/xmem/Mockxmem.h"
#include "src/interruptManager/MockinterruptManager.h"
#include "src/elasticNodeMiddleware/MockelasticNodeMiddleware_internal.h"
#include "src/delay/Mockdelay.h"

uint8_t pin_fpga_done;

uint8_t FPGA_DONE_INT_REG = 0;
uint8_t FPGA_DONE_INT_CONTROL_REG = 0;
uint8_t *PIN_FPGA_DONE = &pin_fpga_done;
uint8_t memoryarea[2000];
const uint8_t *externalMockMemory = memoryarea;

extern volatile uint8_t fpgaDoneResponse;
extern volatile uint8_t *AddressMultiboot;

void initalise_reconfigure_multiboot_mockRegister(void) {
    PIN_FPGA_DONE = &pin_fpga_done;
}

void writeMultiboot(uint32_t address) {
    //32 address
    TEST_ASSERT_EQUAL_UINT8(*(AddressMultiboot + 0), (uint8_t)(0xff & (address >> (0 * 8))));

    TEST_ASSERT_EQUAL_UINT8(*(AddressMultiboot + 0), (uint8_t)(0xff & (address >> (1 * 8))));

    TEST_ASSERT_EQUAL_UINT8(*(AddressMultiboot + 0), (uint8_t)(0xff & (address >> (2 * 8))));
}

void test_reconfigure_fpgaMultiboot(void) {
    uint32_t address = 0;

    elasticnode_fpgaPowerOn_internal_Expect();

    //xmem mock with test flag!
    xmem_enableXmem_Expect();

    //reconfigure_fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    //reconfigure_fpgaMultibootClearComplete_internal_Expect();

    xmem_disableXmem_Expect();

    interruptManager_setInterrupt_Expect();

    reconfigure_fpgaMultiboot(address);

    writeMultiboot(address);
}

void test_getMultibootAddress(void) {
    uint32_t address = (uint32_t)(*(AddressMultiboot) + *(AddressMultiboot + 1) + *(AddressMultiboot + 2));
    TEST_ASSERT_EQUAL_UINT32(address, reconfigure_getMultibootAddress());
}

void test_reconfigure_fpgaMultibootComplete(void) {
    //reconfigure_fpgaMultibootComplete_internal_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL(((*PIN_FPGA_DONE & (1 << P_FPGA_DONE)) != 0), reconfigure_fpgaMultibootComplete());
}

void test_interruptSR_case1(void) {
    initalise_reconfigure_multiboot_mockRegister();

    //make if condition successful
    *(PIN_FPGA_DONE) |= (1 << P_FPGA_DONE);

    //reconfigure_fpgaSetDoneReponse_internal_Expect(1);

    //BEFORE: fpgaDoneResponse = FPGA_DONE_NOTHING
    fpgaDoneResponse = FPGA_DONE_PRINT;
    interruptManager_clearInterrupt_Expect();
    interruptManager_setInterrupt_Expect();

    reconfigure_interruptSR();
}

void test_interruptSR_case2(void) {
    initalise_reconfigure_multiboot_mockRegister();

    //make if condition successful
    *(PIN_FPGA_DONE) |= (1 << P_FPGA_DONE);

    //reconfigure_fpgaSetDoneReponse_internal_Expect(1);

    fpgaDoneResponse = FPGA_DONE_MULTIBOOT;

    elasticnode_setFpgaSoftReset_internal_Expect();
    _delay_ms_Expect(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal_Expect();

    //here: reconfigure_fpgaMultiboot(0);
    uint32_t address = 0;
    elasticnode_fpgaPowerOn_internal_Expect();
    //xmem mock with test flag!
    xmem_enableXmem_Expect();
    //reconfigure_fpgaSetDoneReponse_internal_Expect(FPGA_DONE_PRINT);
    //reconfigure_fpgaMultibootClearComplete_internal_Expect();
    xmem_disableXmem_Expect();
    interruptManager_setInterrupt_Expect();
    //till here

    interruptManager_setInterrupt_Expect();

    reconfigure_interruptSR();

    writeMultiboot(address);
}

void test_interruptSR_case3(void) {
    initalise_reconfigure_multiboot_mockRegister();

    //make if condition successful
    *(PIN_FPGA_DONE) |= (1 << P_FPGA_DONE);

    //reconfigure_fpgaSetDoneReponse_internal_Expect(1);

    fpgaDoneResponse = FPGA_DONE_NOTHING;
    interruptManager_setInterrupt_Expect();

    reconfigure_interruptSR();
}
