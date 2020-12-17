#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H

#include <stdint.h>

#define FPGA_DONE_NOTHING 0
#define FPGA_DONE_PRINT 1
#define FPGA_DONE_MULTIBOOT 2

void reconfigure_fpgaMultibootClearComplete_internal(void);
void reconfigure_fpgaSetDoneReponse_internal(uint8_t response);
uint8_t reconfigure_fpgaMultibootComplete_internal(void);


#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_AVR_H
