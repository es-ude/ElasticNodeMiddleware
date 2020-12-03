//#include "unity.h"
//#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
//#include "src/pinDefinition/fpgaPins.h"
//#include "src/elasticNodeMiddleware/MockelasticNodeMiddleware_internal.h"
//#include "src/xmem/Mockxmem.h"
//#include "test/header_replacements/EmbeddedUtilities/MockBitManipulation.h"
//

// TODO: The middleware API has been renamed and especially the read/write functions are different.
// TODO: Tests likely need to be rewritten. Pls fix.

//uint8_t port_fpga_program_b;
//uint8_t ddr_fpga_program_b;
//
//uint8_t port_fpga_power_sram;
//uint8_t ddr_fpga_power_sram;
//uint8_t port_fpga_power_aux;
//uint8_t ddr_fpga_power_aux;
//uint8_t port_fpga_power_io;
//uint8_t ddr_fpga_power_io;
//uint8_t port_fpga_power_int;
//uint8_t ddr_fpga_power_int;
//
//uint8_t ddr_fpga_cclk;
//
//uint8_t port_fpga_init_b;
//uint8_t ddr_fpga_init_b;
//
//uint8_t port_fpga_done;
//uint8_t ddr_fpga_done;
//
//uint8_t* DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
//uint8_t* PORT_FPGA_PROGRAM_B = &port_fpga_program_b;
//
//uint8_t* DDR_FPGA_POWER_SRAM =&ddr_fpga_power_sram;
//uint8_t* PORT_FPGA_POWER_SRAM =&port_fpga_power_sram;
//uint8_t* DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
//uint8_t* PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
//uint8_t* DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
//uint8_t* PORT_FPGA_POWER_IO = &port_fpga_power_io;
//uint8_t* DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
//uint8_t* PORT_FPGA_POWER_INT = &port_fpga_power_int;
//
//uint8_t* DDR_FPGA_CCLK = &ddr_fpga_cclk;
//
//uint8_t* DDR_FPGA_INIT_B = &ddr_fpga_init_b;
//uint8_t* PORT_FPGA_INIT_B=&port_fpga_init_b;
//
//uint8_t* DDR_FPGA_DONE = &ddr_fpga_done;
//uint8_t* PORT_FPGA_DONE = &port_fpga_done;
//
//uint8_t memoryarea[2000];
//const uint8_t* externalMockMemory = &memoryarea;
//
//volatile uint8_t* ptr_xmem_offset;
//
//void initialise_mockRegister(void) {
//    DDR_FPGA_PROGRAM_B = &ddr_fpga_program_b;
//    PORT_FPGA_PROGRAM_B = &port_fpga_program_b;
//
//    DDR_FPGA_POWER_SRAM =&ddr_fpga_power_sram;
//    PORT_FPGA_POWER_SRAM =&port_fpga_power_sram;
//    DDR_FPGA_POWER_AUX = &ddr_fpga_power_aux;
//    PORT_FPGA_POWER_AUX = &port_fpga_power_aux;
//    DDR_FPGA_POWER_IO = &ddr_fpga_power_io;
//    PORT_FPGA_POWER_IO = &port_fpga_power_io;
//    DDR_FPGA_POWER_INT = &ddr_fpga_power_int;
//    PORT_FPGA_POWER_INT = &port_fpga_power_int;
//
//    DDR_FPGA_CCLK = &ddr_fpga_cclk;
//
//    PORT_FPGA_INIT_B = &port_fpga_init_b;
//    DDR_FPGA_INIT_B= &ddr_fpga_init_b;
//
//    DDR_FPGA_DONE = &ddr_fpga_done;
//    PORT_FPGA_DONE = &port_fpga_done;
//}
//
//void test_elasticnode_initialise(void) {
//    initialise_mockRegister();
//
//
//    //fpgaPowerOn
//    elasticnode_fpgaPowerOn_internal_Expect();
//
//    //single control --> register do not exist
//
//    //enable interface
//    BitManipulation_setBit_Expect(&ddr_fpga_cclk, P_FPGA_CCLK);
//
//    // inputs that only get setup once
//    BitManipulation_setBit_Expect(&port_fpga_init_b, P_FPGA_INIT_B);
//    BitManipulation_clearBit_Expect(&ddr_fpga_init_b, P_FPGA_INIT_B);
//    BitManipulation_setBit_Expect(&port_fpga_init_b, P_FPGA_INIT_B);
//
//    BitManipulation_clearBit_Expect(&ddr_fpga_done, P_FPGA_DONE);
//    BitManipulation_setBit_Expect(&port_fpga_done, P_FPGA_DONE);
//
//    BitManipulation_setBit_Expect(&port_fpga_program_b, P_FPGA_PROGRAM_B);
//    BitManipulation_clearBit_Expect(&ddr_fpga_program_b, P_FPGA_PROGRAM_B);
//    BitManipulation_setBit_Expect(&port_fpga_program_b, P_FPGA_PROGRAM_B);
//
//    elasticnode_initialise();
//}
//
//void test_elasticnodemiddlewareticnode_FPGAPowerOn(void){
//    elasticnode_fpgaPowerOn_internal_Expect();
//    elasticnode_fpgaPowerOn();
//}
//
//void test_elasticnode_FPGAPowerOff(void){
//    elasticnode_fpgaPowerOff_internal_Expect();
//    elasticnode_fpgaPowerOff();
//}
//
////block until it has written all of the data to the file
//void test_elasticnode_writeOneByteBlockingFromFpga(void){
//    initialise_mockRegister();
//    uint8_t address = 0;
//    uint8_t data = 34;
//
//    elasticnode_writeOneByteBlockingFromFpga(address, data);
//
//    TEST_ASSERT_EQUAL_UINT8((*ptr_xmem_offset), data);
//}
//
//void test_elasticnode_writeDataBlockingFromFpga(void) {
//    initialise_mockRegister();
//
//    uint8_t address = 0;
//    uint8_t destination;
//    uint8_t* ptr_data = &destination;
//    uint8_t size = 3;
//
//    for(uint8_t i =0; i<3; i++) {
//        *ptr_data = i;
//    }
//
//    elasticnode_writeDataBlockingFromFpga(address, size, ptr_data);
//
//    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset), *ptr_data);
//    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset +1), *(ptr_data +1));
//    TEST_ASSERT_EQUAL_UINT8(*(ptr_xmem_offset +2), *(ptr_data + 2));
//}
//
//void test_elasticnode_readOneByteBlockingFromFpga(void) {
//    initialise_mockRegister();
//
//    uint8_t address = 0;
//
//    uint8_t byte = elasticnode_readOneByteBlockingFromFpga(address);
//
//    TEST_ASSERT_EQUAL_UINT8(byte,(*ptr_xmem_offset));
//}
//
//// waits until at least one byte is available to return to the application
//void test_elasticnode_readDataBlockingFromFpga(void) {
//    initialise_mockRegister();
//    uint8_t address = 0;
//    uint8_t size= 3;
//    uint8_t destination;
//    uint8_t* ptr_return = &destination;
//
//    elasticnode_readDataBlockingFromFpga(address, size, ptr_return);
//
//    TEST_ASSERT_EQUAL_UINT8((*ptr_return), (*ptr_xmem_offset));
//    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 1), (*ptr_xmem_offset + 1));
//    TEST_ASSERT_EQUAL_UINT8((*ptr_return + 2), (*ptr_xmem_offset  + 2));
//}
//
//void test_fpgaSoftReset(void) {
//    initialise_mockRegister();
//    elasticnode_setFpgaSoftReset_internal_Expect();
//
//    //in internal for testing
//    _delay_ms_Expect(RESET_DELAY);
//
//    elasticnode_clearFpgaSoftReset_internal_Expect();
//
//    elasticnode_userlogicReset();
//}
//
//void test_fpgaHardReset(void) {
//    initialise_mockRegister();
//
//    //xmem_disableXmem_Expect();
//
//    elasticnode_setFpgaHardReset_internal_Expect();
//
//    //_delay_ms_Expect(RESET_DELAY);
//
//    elasticnode_clearFpgaHardReset_internal_Expect();
//
//    elasticnode_fpgaHardReset();
//}