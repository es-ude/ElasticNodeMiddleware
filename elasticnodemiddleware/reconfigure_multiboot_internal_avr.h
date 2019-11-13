//
// Created by annika on 10.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H

#include <stdint.h>

#define FPGA_DONE_NOTHING 0
#define FPGA_DONE_PRINT 1
#define FPGA_DONE_MULTIBOOT 2

void reconfigure_fpgaMultibootClearComplete_internal();
void reconfigure_fpgaSetDoneReponse_internal(uint8_t response);
uint8_t reconfigure_fpgaMultibootComplete_internal();


#ifdef TEST
volatile uint8_t fpgaMultibootCompleteState;
#endif

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H
