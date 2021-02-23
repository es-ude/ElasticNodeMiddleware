#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H

#include <stddef.h>

#include "src/xmem/xmem.h"

// --------- <INTERNAL ---------
void elasticnode_initialise(void);

void elasticnode_fpgaPowerOn(void);

void elasticnode_fpgaPowerOff(void);

void elasticnode_setFpgaSoftReset(void);

void elasticnode_clearFpgaSoftReset(void);

void elasticnode_userlogicReset(void);

void elasticnode_fpgaHardReset(void);
// --------- INTERNAL> ---------

// --------- <LED ---------
void elasticnode_led_mcu_init(void);

void elasticnode_led_mcu_turnOn(uint8_t lednumber);

void elasticnode_led_mcu_turnOff(uint8_t lednumber);

void elasticnode_led_mcu_turnOnAll(void);

void elasticnode_led_mcu_turnOffAll(void);
// --------- LED> ---------

// --------- <XMEM ---------
void elasticnode_enableFpgaInterface(void);

void elasticnode_disableFpgaInterface(void);

// --------- XMEM> ---------

// --------- <RECONFIGURE_MULTIBOOT_AVR ---------
void elasticnode_configureFPGA(uint32_t address);

void elasticnode_reconfigure_interruptSR(void);

uint32_t elasticnode_getLoadedConfigurationAddress(void);

uint8_t elasticnode_reconfigure_fpgaMultibootComplete(void);
// --------- RECONFIGURE_MULTIBOOT_AVR> ---------

#ifdef DEBUG
// --------- <CONTROLMANAGER ---------
void elasticnode_control_setUserHandle(void (*userHandler)(uint8_t));

void elasticnode_control_handleChar(uint8_t currentData);
// --------- CONTROLMANAGER> ---------

// --------- <DEBUG ---------
void  elasticnode_debugTask(void);

uint16_t  elasticnode_debugNumInputAvailable(void);

void elasticnode_debugInit(void (*receiveHandler)(uint8_t));

void elasticnode_setDebugReceiveHandler(void (*receiveHandler)(uint8_t));

void elasticnode_debugNewLine(void);

void elasticnode_debugWriteBool(uint8_t input);

void elasticnode_debugWriteLine(char *s);

void elasticnode_debugWriteString(char *s);

void elasticnode_debugWriteStringLength(char *s, uint16_t length);

void elasticnode_debugWriteChar(uint8_t c);

void elasticnode_debugWriteCharBlock(uint8_t c);

uint8_t elasticnode_debugReadCharAvailable(void);

void elasticnode_debugReadCharProcessed(void);

uint8_t elasticnode_debugReadCharBlock(void);

uint8_t elasticnode_debugGetChar(void);

void elasticnode_debugWriteHex8(uint8_t num);

void elasticnode_debugWriteHex16(uint16_t num);

void elasticnode_debugWriteHex32(uint32_t num);

void elasticnode_debugWriteDec8(uint8_t num);

void elasticnode_debugWriteDec16(uint16_t num);

void elasticnode_debugWriteDec32(uint32_t num);

void elasticnode_debugWriteDec32S(int32_t num);

void elasticnode_debugWriteBin4(uint8_t num);

void elasticnode_debugWriteBin8(uint8_t num);

void elasticnode_debugWriteBin32(uint32_t num);

void elasticnode_debugWriteFloat(float num);

void elasticnode_debugWriteFloatFull(float num);

void elasticnode_debugDone(void);

void elasticnode_debugReady(void);

void elasticnode_debugWaitUntilDone(void);

uint8_t elasticnode_debugSending(void);

void elasticnode_debugAck(uint8_t c);
// --------- DEBUG> ---------
#endif

/*
// TODO: implement void elasticnode_fpgaSleep(uint8_t sleepmode);

void elasticnode_writeByteToUserlogic(uint8_t userlogicAddr, uint8_t data);
void elasticnode_writeBufferToUserlogic(uint8_t userlogicAddr, uint16_t size, const uint8_t *buffer);
uint8_t elasticnode_readByteFromUserlogic(uint8_t userlogicAddr);
void elasticnode_readBufferFromUserlogic(uint8_t userlogicAddr, uint16_t size, uint8_t *buffer);

//void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data);
//void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data);
//uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address);
//void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return);

//TODO: read + write non-blocking might be implemented

//for integration testing
#define FPGA_DONE_MULTIBOOT 2

#ifdef TEST

#else

#include <util/delay.h>

#endif
*/

#endif // ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
