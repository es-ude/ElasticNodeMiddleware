//
// Created by annika on 10.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_H

#include <stdint.h>
#include "elasticnodemiddleware/xmem.h"

//for testing
volatile uint8_t* ptr_fpgaMultibootCompleteFlag;
volatile uint8_t fpgaMultibootCompleteFlag;
volatile uint8_t* multiboot;

void initMultiboot();
void fpgaMultiboot(uint32_t address);
void fpgaMultibootClearComplete();
uint8_t fpgaMultibootComplete(void);
void fpgaSetDoneReponse(uint8_t response);
void interruptSR();

void initPtrMultiboot();

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_H
