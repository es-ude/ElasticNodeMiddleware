#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H

#include <stdint.h>

// TODO: Was only in defined elasticNodeMiddleware_internal.h originally
#define RESET_DELAY 10

void reconfigure_initMultiboot(void);

void reconfigure_fpgaMultiboot(uint32_t address);

void reconfigure_interruptSR(void);

uint32_t reconfigure_getMultibootAddress(void);

uint8_t reconfigure_fpgaMultibootComplete(void);

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_AVR_H
