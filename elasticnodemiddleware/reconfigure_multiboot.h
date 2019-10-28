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
volatile uint8_t* ptr_fpgaDoneResponse;
volatile uint8_t fpgaDoneResponse;
volatile uint8_t* multiboot;

void reconfigure_initMultiboot();
void reconfigure_fpgaMultiboot(uint32_t address);
void reconfigure_fpgaMultibootClearComplete();
uint8_t reconfigure_fpgaMultibootComplete(void);
void reconfigure_fpgaSetDoneReponse(uint8_t response);
void reconfigure_interruptSR();

void reconfigure_initPtrMultiboot();

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_H
