#include "EmbeddedUtilities/BitManipulation.h"

#include "ElasticNodeMiddleware/elasticNodeMiddleware.h"
#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/led/led_mcu.h"
#include "src/xmem/xmem.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"
#include "src/controlmanager/controlmanager.h"

//TODO: Put these magic number offset values to somewhere more meaningful
#define USERLOGIC_RESET_OFFSET 0x04
#define USERLOGIC_RESET_VALUE 0x0

// volatile uint8_t* ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET);
volatile uint8_t *xmemOffset = (uint8_t * )(XMEM_OFFSET);
volatile uint8_t *userLogicOffsetAddr = (uint8_t * )(XMEM_USERLOGIC_OFFSET);

// --------- <INTERNAL ---------
void elasticnode_initialise(void) {
    // disable r2f chip for start up
    DDRE |= 0x40; // wireless_cs low
    PORTE |= 0x40;
    DDRB |= 0x20; // wireless_reset low
    PORTB &= ~0x20;

    // initalise fpga
    elasticnode_fpgaPowerOn_internal();

    // enable interface
    BitManipulation_setBit(DDR_FPGA_CCLK, P_FPGA_CCLK);

    // inputs that only get setup once
    BitManipulation_setBit(PORT_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_clearBit(DDR_FPGA_INIT_B, P_FPGA_INIT_B);
    BitManipulation_setBit(PORT_FPGA_INIT_B, P_FPGA_INIT_B);

    BitManipulation_clearBit(DDR_FPGA_DONE, P_FPGA_DONE);
    BitManipulation_setBit(PORT_FPGA_DONE, P_FPGA_DONE);

    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_clearBit(DDR_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
    BitManipulation_setBit(PORT_FPGA_PROGRAM_B, P_FPGA_PROGRAM_B);
}

void elasticnode_fpgaPowerOn(void) {
    elasticnode_fpgaPowerOn_internal();
}

void elasticnode_fpgaPowerOff(void) {
    elasticnode_fpgaPowerOff_internal();
}

void elasticnode_setFpgaSoftReset(void) {
    elasticnode_setFpgaSoftReset_internal();
}

void elasticnode_clearFpgaSoftReset(void) {
    elasticnode_clearFpgaSoftReset_internal();
}

void elasticnode_setFpgaHardReset(void) {
    elasticnode_setFpgaHardReset_internal();
}

void elasticnode_clearFpgaHardReset(void) {
    elasticnode_clearFpgaHardReset_internal();
}
// --------- INTERNAL> ---------


// --------- <CONTROLMANAGER ---------
void elasticnode_control_setUserHandle(void (*userHandler)(uint8_t)){
    control_setUserHandle(userHandler);
}

void elasticnode_control_handleChar(uint8_t currentData){
    control_handleChar(currentData);
}
// --------- CONTROLMANAGER> ---------


// --------- <LED ---------
void elasticnode_led_mcu_init(void) {
    led_mcu_init();
}

void elasticnode_led_mcu_turnOn(uint8_t lednumber) {
    led_mcu_turnOn(lednumber);
}

void elasticnode_led_mcu_turnOff(uint8_t lednumber) {
    led_mcu_turnOff(lednumber);
}

void elasticnode_led_mcu_turnOnAll(void) {
    led_mcu_turnOnAll();
}

void elasticnode_led_mcu_turnOffAll(void) {
    led_mcu_turnOffAll();
}
// --------- LED> ---------


// --------- <XMEM ---------
void elasticnode_xmem_initXmem(void) {
    xmem_initXmem();
}

void elasticnode_enableFpgaInterface(void) {
    xmem_enableXmem();
}

void elasticnode_disableFpgaInterface(void) {
    xmem_disableXmem();
}
// --------- XMEM> ---------


// --------- <RECONFIGURE_MULTIBOOT_AVR ---------
// TODO: Why not used?
void elasticnode_reconfigure_initMultiboot(void) {
    reconfigure_initMultiboot();
}

void elasticnode_configureFPGA(uint32_t address) {
    reconfigure_fpgaMultiboot(address);
    // TODO: Does not finish
    //while(!reconfigure_fpgaMultibootComplete());
}

void elasticnode_reconfigure_interruptSR(void) {
    reconfigure_interruptSR();
}

uint32_t elasticnode_reconfigure_getMultibootAddress(void) {
    reconfigure_getMultibootAddress();
}

uint8_t elasticnode_reconfigure_fpgaMultibootComplete(void) {
    reconfigure_fpgaMultibootComplete();
}
// --------- RECONFIGURE_MULTIBOOT_AVR> ---------

/*


//TODO: Does not finishes
void elasticnode_configureFPGA(uint32_t address){
    reconfigure_fpgaMultiboot(address);
    while(!reconfigure_fpgaMultibootComplete());
}

uint32_t elasticnode_getLoadedConfigurationAddress(void){
    return reconfigure_getMultibootAddress();
}
void elasticnode_writeByteToUserlogic(uint8_t userlogicAddr, uint8_t data) {
    *(userLogicOffsetAddr + userlogicAddr) = data;
}

void elasticnode_writeBufferToUserlogic(uint8_t userlogicAddr, uint16_t size, const uint8_t *buffer) {
    for (uint16_t i = 0; i < size; i++) {
        *(userLogicOffsetAddr + userlogicAddr + i) = *(buffer + i);
    }
}

uint8_t elasticnode_readByteFromUserlogic(uint8_t userlogicAddr) {
    return *(userLogicOffsetAddr + userlogicAddr);
}

void elasticnode_readBufferFromUserlogic(uint8_t userlogicAddr, uint16_t size, uint8_t *buffer) {
    for (uint16_t i = 0; i < size; i++) {
        *(buffer + i) = *(userLogicOffsetAddr + userlogicAddr + i);
    }
}


//void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data) {
//    *(ptr_xmem_offset + address) = data;
//}
//void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data) {
//    uint8_t *targetAddress = ptr_xmem_offset + address;
//    for (uint8_t k = 0; k < size; k++) {
//        *(targetAddress + k) = *(ptr_data + k);
//    }
//}
//
//uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address) {
//    return *(ptr_xmem_offset + address);
//}
//
//void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return) {
//    ptr_xmem_offset = ptr_xmem_offset + address;
//    for (uint8_t i = 0; i < size; i++) {
//        *(ptr_return + i) = *(ptr_xmem_offset + i);
//    }
//}

void elasticnode_userlogicReset(void) {
    //this resets the userlogic
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}
*/
