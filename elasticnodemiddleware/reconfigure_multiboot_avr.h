//
// Created by annika on 10.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H

#include <stdint.h>

void reconfigure_initMultiboot();
void reconfigure_fpgaMultiboot(uint32_t address);
void reconfigure_interruptSR();
uint32_t reconfigure_getMultibootAddress();

#ifdef TEST
volatile uint8_t fpgaDoneResponse;
volatile uint8_t *multiboot;
#endif

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H
