#include "EmbeddedUtilities/BitManipulation.h"

#include "ElasticNodeMiddleware/ElasticNodeMiddleware.h"
#include "src/elasticNodeMiddleware/elasticNodeMiddleware_internal.h"

#include "src/pinDefinition/fpgaPins.h"
#include "src/pinDefinition/fpgaRegisters.h"

#include "src/led/led_mcu.h"
#include "src/reconfigure_multiboot_avr/reconfigure_multiboot_avr.h"

#include "src/delay/delay.h"

#include "src/xmem/xmem.h"

#include "src/interruptManager/interruptManager.h"

#ifdef DEBUG
#include "src/controlmanager/controlmanager.h"
#include "src/configuration/configuration.h"
#include "src/debug/debug.h"
#include "src/flash/flash.h"

#ifdef UART
#include "src/uart/uart.h"
#endif
#endif

//TODO: Put these magic number offset values to somewhere more meaningful
#define USERLOGIC_RESET_OFFSET 0x04
#define USERLOGIC_RESET_VALUE 0x0

// volatile uint8_t* ptr_xmem_offset = (uint8_t* )(XMEM_OFFSET);
//volatile uint8_t *xmemOffset = (uint8_t *) (XMEM_OFFSET);
//volatile uint8_t *userLogicOffsetAddr = (uint8_t *) (XMEM_USERLOGIC_OFFSET);

// --------- <INTERNAL ---------
void elasticnode_initialise(void) {
    // disable r2f chip for start up
    BitManipulation_setBit(DDR_FPGA_POWER_SRAM, 6); // wireless_cs low
    BitManipulation_setBit(PORT_FPGA_POWER_SRAM, 6);
    BitManipulation_setBit(DDR_FPGA_POWER_INT, 5); // wireless_reset lows
    BitManipulation_clearBit(PORT_FPGA_DONE, 5);
    //DDRE |= 0x40; // wireless_cs low
    //PORTE |= 0x40;
    //DDRB |= 0x20; // wireless_reset low
    //PORTB &= ~0x20;

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

void elasticnode_userlogicReset(void) {
    //this resets the userlogic
    elasticnode_setFpgaSoftReset_internal();
    _delay_ms(RESET_DELAY);
    elasticnode_clearFpgaSoftReset_internal();
}

void elasticnode_fpgaHardReset(void) {
    // This resets the FPGA using gpio pins
    elasticnode_setFpgaHardReset_internal();
    elasticnode_clearFpgaHardReset_internal();
}
// --------- INTERNAL> ---------

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
void elasticnode_initFpgaInterface(void) {
    xmem_initXmem();
    xmem_enableXmem();
    xmem_disableXmem();
    _delay_ms(10);
}

uint16_t elasticnode_xmem_offset(void) {
    return xmem_offset();
}

void elasticnode_enableFpgaInterface(void) {
    xmem_enableXmem();
}

void elasticnode_disableFpgaInterface(void) {
    xmem_disableXmem();
}
// --------- XMEM> ---------

// --------- <RECONFIGURE_MULTIBOOT_AVR ---------
void elasticnode_configureFPGA(uint32_t address) {
    reconfigure_fpgaMultiboot(address);
}

uint8_t elasticnode_configureFPGA_wait_for_finish(uint32_t address) {
    elasticnode_fpgaPowerOn_internal();
    uint16_t count = 0;
    while (!reconfigure_fpgaMultibootComplete()) {
        if (count > 300) {
            break;
        }
        count++;
        _delay_ms(10);
    }
    reconfigure_fpgaMultiboot(address);
    count = 0;
    while (!reconfigure_fpgaMultibootComplete()) {
        if (count > 300) {
#ifdef DEBUG
            debugWriteString("Something went wrong with configurating the FGPA to ");
            debugWriteHex32(address);
            debugWriteLine("!");
#endif
            return 0;
        }
        count++;
        _delay_ms(10);
    }

    _delay_ms(10);
    return 1;
}

void elasticnode_reconfigure_interruptSR(void) {
    reconfigure_interruptSR();
}

uint32_t elasticnode_getLoadedConfigurationAddress(void) {
    return reconfigure_getMultibootAddress();
}

uint8_t elasticnode_reconfigure_fpgaMultibootComplete(void) {
    uint8_t rec = reconfigure_fpgaMultibootComplete();
    _delay_ms(10);
    return rec;
}
// --------- RECONFIGURE_MULTIBOOT_AVR> ---------

// --------- <INTERRUPT ---------
void elasticnode_interruptManager_clearInterrupt(void) {
    interruptManager_clearInterrupt();
}

void elasticnode_interruptManager_setInterrupt(void) {
    interruptManager_setInterrupt();
}
// --------- INTERRUPT> ---------

#ifdef DEBUG
// --------- <CONTROLMANAGER ---------
void elasticnode_control_setUserHandle(void (*userHandler)(uint8_t)){
    control_setUserHandle(userHandler);
}

void elasticnode_control_handleChar(uint8_t currentData){
    control_handleChar(currentData);
}
// --------- CONTROLMANAGER> ---------

// --------- <CONFIG ---------
void elasticnode_configurationFlash(void (*readData)(uint8_t *, uint16_t)) {
    initFlash(); // SPI interface init and ..? Todo
    unlockFlash(0);
    configurationFlash(readData);
}
// --------- CONFIG> ---------

// --------- <FLASH ---------
void elasticnode_eraseSectorFlash(uint32_t address, uint8_t mcuFlash) {
    eraseSectorFlash(address, mcuFlash);
}

void elasticnode_flashEnableInterface(void) {
    flashEnableInterface();
}

void elasticnode_writeDataFlash(uint32_t address, uint8_t *data, uint16_t length, uint8_t mcuFlash) {
    writeDataFlash(address, data, length, mcuFlash);
}
// --------- FLASH> ---------

// --------- <DEBUG ---------
#ifdef LUFA
void  elasticnode_debugTask(void){
    debugTask();
}

uint16_t  elasticnode_debugNumInputAvailable(void){
    return debugNumInputAvailable();
}
#endif

void elasticnode_debugInit(void (*receiveHandler)(uint8_t)){
    debugInit(receiveHandler);
}

#ifdef UART
void elasticnode_setDebugReceiveHandler(void (*receiveHandler)(uint8_t)){
    setDebugReceiveHandler(receiveHandler);
}
#endif

void elasticnode_debugNewLine(void){
    debugNewLine();
}

void elasticnode_debugWriteBool(uint8_t input){
    debugWriteBool(input);
}

void elasticnode_debugWriteLine(char *s){
    debugWriteLine(s);
}

void elasticnode_debugWriteString(char *s){
    debugWriteString(s);
}

void elasticnode_debugWriteStringLength(char *s, uint16_t length){
    debugWriteStringLength(s, length);
}

void elasticnode_debugWriteChar(uint8_t c){
    debugWriteChar(c);
}

void elasticnode_debugWriteCharBlock(uint8_t c){
    debugWriteCharBlock(c);
};

uint8_t elasticnode_debugReadCharAvailable(void){
    return debugReadCharAvailable();
}

#ifdef UART
void elasticnode_debugReadCharProcessed(void){
    return debugReadCharProcessed();
}
#endif

uint8_t elasticnode_debugReadCharBlock(void){
    return debugReadCharBlock();
}

uint8_t elasticnode_debugGetChar(void){
    return debugGetChar();
}

void elasticnode_debugWriteHex8(uint8_t num){
    debugWriteHex8(num);
}

void elasticnode_debugWriteHex16(uint16_t num){
    debugWriteHex16(num);
}

void elasticnode_debugWriteHex32(uint32_t num){
    debugWriteHex32(num);
}

void elasticnode_debugWriteDec8(uint8_t num){
    debugWriteDec8(num);
}

void elasticnode_debugWriteDec16(uint16_t num){
    debugWriteDec16(num);
}

void elasticnode_debugWriteDec32(uint32_t num){
    debugWriteDec32(num);
}

void elasticnode_debugWriteDec32S(int32_t num){
    debugWriteDec32S(num);
}

void elasticnode_debugWriteBin4(uint8_t num){
    debugWriteBin4(num);
}

void elasticnode_debugWriteBin8(uint8_t num){
    debugWriteBin8(num);
}

void elasticnode_debugWriteBin32(uint32_t num){
    debugWriteBin32(num);
}

void elasticnode_debugWriteFloat(float num){
    debugWriteFloat(num);
}

void elasticnode_debugWriteFloatFull(float num){
    debugWriteFloatFull(num);
}

void elasticnode_debugDone(void){
    debugDone();
}

void elasticnode_debugReady(void){
    debugReady();
}

void elasticnode_debugWaitUntilDone(void){
    debugWaitUntilDone();
}

#ifdef UART
uint8_t elasticnode_debugSending(void){
    return debugSending();
}
#endif

void elasticnode_debugAck(uint8_t c){
    debugAck(c);
}

#ifdef UART
void elasticnode_uart_ISR_Receive(){
    uart_ISR_Receive();
}

void elasticnode_uart_ISR_Transmit(){
    uart_ISR_Transmit();
}
#endif
// --------- DEBUG> ---------
#endif


// TODO: check old code
/*
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

void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data) {
    *(ptr_xmem_offset + address) = data;
}

void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data) {
    uint8_t *targetAddress = ptr_xmem_offset + address;
    for (uint8_t k = 0; k < size; k++) {
        *(targetAddress + k) = *(ptr_data + k);
    }
}

uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address) {
    return *(ptr_xmem_offset + address);
}

void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return) {
    ptr_xmem_offset = ptr_xmem_offset + address;
    for (uint8_t i = 0; i < size; i++) {
        *(ptr_return + i) = *(ptr_xmem_offset + i);
    }
}

*/
