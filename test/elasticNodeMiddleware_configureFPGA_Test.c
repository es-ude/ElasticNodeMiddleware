#include "unity.h"
#include "src/elasticNodeMiddleware/configure/elasticNodeMiddleware_configureFPGA.h"
#include "src/reconfigure_multiboot_avr/Mockreconfigure_multiboot_avr.h"

void test_elasticnode_configureFrom(void) {
    uint32_t address = 0x23409;

    reconfigure_fpgaMultiboot_Expect(address);
    reconfigure_fpgaMultibootComplete_ExpectAndReturn(1);
    elasticnode_configureFPGA_configureFrom(address);
}

void test_elasticnode_getConfiguration(void) {

    //beliebige Zahl?
    reconfigure_getMultibootAddress_ExpectAndReturn(0);
    elasticnode_configureFPGA_getLoadedConfiguration();
}