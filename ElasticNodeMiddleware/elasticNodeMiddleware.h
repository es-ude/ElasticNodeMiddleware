#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H

#include "src/xmem/xmem.h"

// --------- <INTERNAL ---------
void elasticnode_initialise(void);

void elasticnode_fpgaPowerOn(void);

void elasticnode_fpgaPowerOff(void);

void elasticnode_setFpgaSoftReset(void);

void elasticnode_clearFpgaSoftReset(void);

void elasticnode_setFpgaHardReset(void);

void elasticnode_clearFpgaHardReset(void);
// --------- INTERNAL> ---------

// --------- <CONTROLMANAGER ---------
void elasticnode_control_setUserHandle(void (*userHandler)(uint8_t));

void elasticnode_control_handleChar(uint8_t currentData);
// --------- CONTROLMANAGER> ---------

// --------- <LED ---------
void elasticnode_led_mcu_init(void);

void elasticnode_led_mcu_turnOn(uint8_t lednumber);

void elasticnode_led_mcu_turnOff(uint8_t lednumber);

void elasticnode_led_mcu_turnOnAll(void);

void elasticnode_led_mcu_turnOffAll(void);
// --------- LED> ---------

// --------- <XMEM ---------
void elasticnode_xmem_initXmem(void);

void elasticnode_enableFpgaInterface(void);

void elasticnode_disableFpgaInterface(void);

// --------- XMEM> ---------

// --------- <RECONFIGURE_MULTIBOOT_AVR ---------
void elasticnode_reconfigure_initMultiboot(void);

void elasticnode_configureFPGA(uint32_t address);

void elasticnode_reconfigure_interruptSR(void);

uint32_t elasticnode_reconfigure_getMultibootAddress(void);

uint8_t elasticnode_reconfigure_fpgaMultibootComplete(void);
// --------- RECONFIGURE_MULTIBOOT_AVR> ---------

#endif // ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
