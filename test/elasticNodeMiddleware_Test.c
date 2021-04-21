#include "unity.h"

#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"

#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"

#include "src/pinDefinition/fpgaPins.h"

#include "src/elasticNodeMiddleware/MockelasticNodeMiddleware_internal.h"
#include "src/reconfigure_multiboot_avr/Mockreconfigure_multiboot_avr.h"
#include "src/led/Mockled_mcu.h"
#include "src/controlmanager/Mockcontrolmanager.h"
#include "src/debug/Mockdebug.h"
#include "src/xmem/Mockxmem.h"
#include "src/delay/Mockdelay.h"

uint8_t port_fpga_program_b;
uint8_t ddr_fpga_program_b;

uint8_t port_fpga_power_sram;
uint8_t ddr_fpga_power_sram;
uint8_t port_fpga_power_aux;
uint8_t ddr_fpga_power_aux;
uint8_t port_fpga_power_io;
uint8_t ddr_fpga_power_io;
uint8_t port_fpga_power_int;
uint8_t port_fpga_power_int;
uint8_t ddr_fpga_power_int;

uint8_t ddr_fpga_cclk;

uint8_t port_fpga_init_b;
uint8_t ddr_fpga_init_b;

uint8_t port_fpga_done;
uint8_t ddr_fpga_done;

uint8_t *DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
uint8_t *PORT_FPGA_PROGRAM_B = &port_fpga_program_b;

uint8_t *DDR_FPGA_POWER_SRAM = &ddr_fpga_power_sram;
uint8_t *PORT_FPGA_POWER_SRAM = &port_fpga_power_sram;
uint8_t *DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
uint8_t *PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
uint8_t *DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
uint8_t *PORT_FPGA_POWER_IO = &port_fpga_power_io;
uint8_t *DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
uint8_t *PORT_FPGA_POWER_INT = &port_fpga_power_int;

uint8_t *DDR_FPGA_CCLK = &ddr_fpga_cclk;

uint8_t *DDR_FPGA_INIT_B = &ddr_fpga_init_b;
uint8_t *PORT_FPGA_INIT_B = &port_fpga_init_b;

uint8_t *DDR_FPGA_DONE = &ddr_fpga_done;
uint8_t *PORT_FPGA_DONE = &port_fpga_done;

uint8_t memoryarea[2000];
const uint8_t *externalMockMemory = (const uint8_t *) &memoryarea;

volatile uint8_t *ptr_xmem_offset;

void initialise_mockRegister(void) {
    DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
    PORT_FPGA_PROGRAM_B = &port_fpga_program_b;

    DDR_FPGA_POWER_SRAM = &ddr_fpga_power_sram;
    PORT_FPGA_POWER_SRAM = &port_fpga_power_sram;
    DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
    PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
    DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
    PORT_FPGA_POWER_IO = &port_fpga_power_io;
    DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
    PORT_FPGA_POWER_INT = &port_fpga_power_int;

    DDR_FPGA_CCLK = &ddr_fpga_cclk;

    PORT_FPGA_INIT_B = &port_fpga_init_b;
    DDR_FPGA_INIT_B = &ddr_fpga_init_b;

    DDR_FPGA_DONE = &ddr_fpga_done;
    PORT_FPGA_DONE = &port_fpga_done;
}

void test_elasticnode_initialise(void) {
    initialise_mockRegister();

    BitManipulation_setBit_Expect(DDR_FPGA_POWER_SRAM, 6);
    BitManipulation_setBit_Expect(PORT_FPGA_POWER_SRAM, 6);
    BitManipulation_setBit_Expect(DDR_FPGA_POWER_INT, 5);
    BitManipulation_clearBit_Expect(PORT_FPGA_DONE, 5);

    //fpgaPowerOn
    elasticnode_fpgaPowerOn_internal_Expect();

    //single control --> register do not exist

    //enable interface
    BitManipulation_setBit_Expect(DDR_FPGA_CCLK, P_FPGA_CCLK);

    // inputs that only get setup once
    BitManipulation_setBit_Expect(PORT_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_clearBit_Expect(DDR_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_setBit_Expect(PORT_FPGA_INIT_B, P_FPGA_INIT_B);

    BitManipulation_clearBit_Expect(DDR_FPGA_DONE, P_FPGA_DONE);
    BitManipulation_setBit_Expect(PORT_FPGA_DONE, P_FPGA_DONE);

    BitManipulation_setBit_Expect(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit_Expect(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_setBit_Expect(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);

    elasticnode_initialise();
}

void test_elasticnode_fpgaPowerOn(void) {
    elasticnode_fpgaPowerOn_internal_Expect();
    elasticnode_fpgaPowerOn();
}

void test_elasticnode_fpgaPowerOff(void) {
    elasticnode_fpgaPowerOff_internal_Expect();
    elasticnode_fpgaPowerOff();
}

void test_elasticnode_setFpgaSoftReset(void) {
    elasticnode_setFpgaSoftReset_internal_Expect();
    elasticnode_setFpgaSoftReset();
}

void test_elasticnode_userlogicReset(void) {
    elasticnode_setFpgaSoftReset_internal_Expect();
    _delay_ms_Expect(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal_Expect();
    elasticnode_userlogicReset();
}

void test_elasticnode_fpgaHardReset(void) {
    elasticnode_setFpgaHardReset_internal_Expect();
    elasticnode_clearFpgaHardReset_internal_Expect();
    elasticnode_fpgaHardReset();
}

void test_elasticnode_led_mcu_init(void) {
    led_mcu_init_Expect();
    led_mcu_init();
}

void test_elasticnode_led_mcu_turnOn(void) {
    for (uint8_t lednumber = 0; lednumber < 255; ++lednumber) {
        led_mcu_turnOn_Expect(lednumber);
        elasticnode_led_mcu_turnOn(lednumber);
    }
}

void test_elasticnode_led_mcu_turnOff(void) {
    for (uint8_t lednumber = 0; lednumber < 255; ++lednumber) {
        led_mcu_turnOff_Expect(lednumber);
        elasticnode_led_mcu_turnOff(lednumber);
    }
}

void test_elasticnode_led_mcu_turnOnAll(void) {
    led_mcu_turnOnAll_Expect();
    elasticnode_led_mcu_turnOnAll();
}

void test_elasticnode_led_mcu_turnOffAll(void) {
    led_mcu_turnOffAll_Expect();
    elasticnode_led_mcu_turnOffAll();
}

void test_elasticnode_initFpgaInterface(void) {
    xmem_initXmem_Expect();
    xmem_enableXmem_Expect();
    xmem_disableXmem_Expect();
    _delay_ms_Expect(10);
    elasticnode_initFpgaInterface();
}

void test_elasticnode_xmem_offset(void) {
    xmem_offset_ExpectAndReturn(45);
    TEST_ASSERT_EQUAL(45, elasticnode_xmem_offset());
}

void test_elasticnode_enableFpgaInterface(void) {
    xmem_enableXmem_Expect();
    elasticnode_enableFpgaInterface();
}

void test_elasticnode_disableFpgaInterface(void) {
    xmem_disableXmem_Expect();
    elasticnode_disableFpgaInterface();
}

void test_elasticnode_configureFPGA(void) {
    uint32_t address = 123;

    reconfigure_fpgaMultiboot_Expect(address);
    elasticnode_configureFPGA(address);
}

void test_elasticnode_configureFPGA_wait_for_finish(void) {
    uint32_t address = 123;
    elasticnode_fpgaPowerOn_internal_Expect();
    reconfigure_fpgaMultiboot_Expect(address);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(0);
    _delay_ms_Expect(10);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(1);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(0);
    _delay_ms_Expect(10);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(1);
    _delay_ms_Expect(10);

    elasticnode_configureFPGA_wait_for_finish(address);
}

void test_elasticnode_reconfigure_interruptSR(void) {
    reconfigure_interruptSR_Expect();
    elasticnode_reconfigure_interruptSR();
}

void test_elasticnode_getLoadedConfigurationAddress(void) {
    uint32_t address = 123;
    reconfigure_getMultibootAddress_ExpectAndReturn(address);
    TEST_ASSERT_EQUAL(elasticnode_getLoadedConfigurationAddress(), address);
}

void test_elasticnode_reconfigure_fpgaMultibootComplete(void) {
    uint32_t complete = 0;
    _delay_ms_Expect(10);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(complete);
    TEST_ASSERT_EQUAL(elasticnode_reconfigure_fpgaMultibootComplete(), complete);
}

void test_elasticnode_control_setUserHandle(void) {
    void *test_handleCharInput;
    void (*userHandler)(uint8_t) = (void (*)(uint8_t)) &test_handleCharInput;
    control_setUserHandle_Expect(userHandler);
    elasticnode_control_setUserHandle((void (*)(uint8_t)) &test_handleCharInput);
}

void test_elasticnode_control_handleChar(void) {
    uint8_t currentData = 123;
    control_handleChar_Expect(currentData);
    elasticnode_control_handleChar(currentData);
}

void test_elasticnode_debugTask(void) {
    debugTask_Expect();
    elasticnode_debugTask();
}

void test_elasticnode_debugNumInputAvailable(void) {
    debugNumInputAvailable_ExpectAndReturn(0);
    TEST_ASSERT_EQUAL(elasticnode_debugNumInputAvailable(), 0);
}

void test_elasticnode_debugInit(void) {
    void (*receiveHandler)(uint8_t) = (void (*)(uint8_t)) 3;
    debugInit_Expect(receiveHandler);
    elasticnode_debugInit(receiveHandler);
}

void test_elasticnode_setDebugReceiveHandler(void) {
    void (*receiveHandler)(uint8_t) = (void (*)(uint8_t)) 3;
    setDebugReceiveHandler_Expect(receiveHandler);
    elasticnode_setDebugReceiveHandler(receiveHandler);
}

void test_elasticnode_debugNewLine(void) {
    debugNewLine_Expect();
    elasticnode_debugNewLine();
}

void test_elasticnode_debugWriteBool(void) {
    uint8_t input = 1;
    debugWriteBool_Expect(input);
    elasticnode_debugWriteBool(input);
}

void test_elasticnode_debugWriteLine(void) {
    char *s = "TEST";
    debugWriteLine_Expect(s);
    elasticnode_debugWriteLine(s);
}

void test_elasticnode_debugWriteString(void) {
    char *s = "TEST";
    debugWriteString_Expect(s);
    elasticnode_debugWriteString(s);
}

void test_elasticnode_debugWriteStringLength(void) {
    char *s = "TEST";
    uint16_t length = 4;
    debugWriteStringLength_Expect(s, length);
    elasticnode_debugWriteStringLength(s, length);
}

void test_elasticnode_debugWriteChar(void) {
    uint8_t c = 'T';
    debugWriteChar_Expect(c);
    elasticnode_debugWriteChar(c);
}

void test_elasticnode_debugWriteCharBlock(void) {
    uint8_t c = 'T';
    debugWriteCharBlock_Expect(c);
    elasticnode_debugWriteCharBlock(c);
}

void test_elasticnode_debugReadCharAvailable(void) {
    uint8_t c = 'T';
    debugReadCharAvailable_ExpectAndReturn(c);
    TEST_ASSERT_EQUAL(elasticnode_debugReadCharAvailable(), c);
}

void test_elasticnode_debugReadCharProcessed(void) {
    debugReadCharProcessed_Expect();
    elasticnode_debugReadCharProcessed();
}

void test_elasticnode_debugReadCharBlock(void) {
    uint8_t c = 'T';
    debugReadCharBlock_ExpectAndReturn(c);
    TEST_ASSERT_EQUAL(elasticnode_debugReadCharBlock(), c);
}

void test_elasticnode_debugGetChar(void) {
    uint8_t c = 'T';
    debugGetChar_ExpectAndReturn(c);
    TEST_ASSERT_EQUAL(elasticnode_debugGetChar(), c);
}

void test_elasticnode_debugWriteHex8(void) {
    uint8_t num = 1;
    debugWriteHex8_Expect(num);
    elasticnode_debugWriteHex8(num);
}

void test_elasticnode_debugWriteHex16(void) {
    uint16_t num = 1;
    debugWriteHex16_Expect(num);
    elasticnode_debugWriteHex16(num);
}

void test_elasticnode_debugWriteHex32(void) {
    uint32_t num = 1;
    debugWriteHex32_Expect(num);
    elasticnode_debugWriteHex32(num);
}

void test_elasticnode_debugWriteDec8(void) {
    uint8_t num = 1;
    debugWriteDec8_Expect(num);
    elasticnode_debugWriteDec8(num);
}

void test_elasticnode_debugWriteDec16(void) {
    uint16_t num = 1;
    debugWriteDec16_Expect(num);
    elasticnode_debugWriteDec16(num);
}

void test_elasticnode_debugWriteDec32(void) {
    uint32_t num = 1;
    debugWriteDec32_Expect(num);
    elasticnode_debugWriteDec32(num);
}

void test_elasticnode_debugWriteDec32S(void) {
    uint32_t num = 1;
    debugWriteDec32S_Expect(num);
    elasticnode_debugWriteDec32S(num);
}

void test_elasticnode_debugWriteBin4(void) {
    uint8_t num = 1;
    debugWriteBin4_Expect(num);
    elasticnode_debugWriteBin4(num);
}

void test_elasticnode_debugWriteBin8(void) {
    uint8_t num = 1;
    debugWriteBin8_Expect(num);
    elasticnode_debugWriteBin8(num);
}

void test_elasticnode_debugWriteBin32(void) {
    uint32_t num = 1;
    debugWriteBin32_Expect(num);
    elasticnode_debugWriteBin32(num);
}

void test_elasticnode_debugWriteFloat(void) {
    float num = 1.23f;
    debugWriteFloat_Expect(num);
    elasticnode_debugWriteFloat(num);
}

void test_elasticnode_debugWriteFloatFull(void) {
    float num = 1.23f;
    debugWriteFloatFull_Expect(num);
    elasticnode_debugWriteFloatFull(num);
}

void test_elasticnode_debugDone(void) {
    debugDone_Expect();
    elasticnode_debugDone();
}

void test_elasticnode_debugReady(void) {
    debugReady_Expect();
    elasticnode_debugReady();
}

void test_elasticnode_debugWaitUntilDone(void) {
    debugWaitUntilDone_Expect();
    elasticnode_debugWaitUntilDone();
}

void test_elasticnode_debugSending(void) {
    debugSending_ExpectAndReturn(0);
    elasticnode_debugSending();
}

void test_elasticnode_debugAck(void) {
    uint8_t c = 'T';
    debugAck_Expect(c);
    elasticnode_debugAck(c);
}

// TODO: check old tests
/*
//block until it has written all of the data to the file
void test_elasticnode_writeByteToUserlogic(void) {
    TEST_FAIL_MESSAGE("This test cannot be correct. Should be reconsidered");

    initialise_mockRegister();
    uint8_t address = 0;
    uint8_t data = 34;
    elasticnode_writeByteToUserlogic(address, data);
//    elasticnode_writeOneByteBlockingFromFpga(address, data);

    TEST_ASSERT_EQUAL_UINT8((*ptr_xmem_offset), data);
}

void test_elasticnode_writeDataBlockingFromFpga(void) {
    TEST_FAIL_MESSAGE("This test cannot be correct. Should be reconsidered");
    initialise_mockRegister();

    uint8_t address = 0;
    uint8_t destination;
    uint8_t *ptr_data = &destination;
    uint8_t size = 3;

    for (uint8_t i = 0; i < 3; i++) {
        *ptr_data = i;
    }
    elasticnode_writeBufferToUserlogic(address, size, ptr_data);
//    elasticnode_writeDataBlockingFromFpga(address, size, ptr_data);

    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset), *ptr_data);
    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset + 1), *(ptr_data + 1));
    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset + 2), *(ptr_data + 2));
}

void test_elasticnode_readOneByteBlockingFromFpga(void) {
    TEST_FAIL_MESSAGE("This test cannot be correct. Should be reconsidered");
    initialise_mockRegister();

    uint8_t address = 0;

    uint8_t byte = elasticnode_readByteFromUserlogic(address);
    (address);
//            elasticnode_readOneByteBlockingFromFpga(address);

    TEST_ASSERT_EQUAL_UINT8(byte, (*ptr_xmem_offset));
}

// waits until at least one byte is available to return to the application
void test_elasticnode_readDataBlockingFromFpga(void) {
    TEST_FAIL_MESSAGE("This test cannot be correct. Should be reconsidered");
    initialise_mockRegister();
    uint8_t address = 0;
    uint8_t size = 3;
    uint8_t destination;
    uint8_t *ptr_return = &destination;

    elasticnode_readBufferFromUserlogic(address, size, ptr_return);
//    elasticnode_readDataBlockingFromFpga(address, size, ptr_return);

    TEST_ASSERT_EQUAL_UINT8((*ptr_return), (*ptr_xmem_offset));
    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 1), (*ptr_xmem_offset + 1));
    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 2), (*ptr_xmem_offset + 2));
}

*/
