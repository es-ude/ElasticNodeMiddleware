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
//void reconfigure_fpgaMultiboot(uint32_t address);
uint8_t reconfigure_fpgaMultibootComplete(void);
void reconfigure_interruptSR();

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_H
