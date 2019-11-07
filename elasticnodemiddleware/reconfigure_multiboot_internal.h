//
// Created by annika on 10.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H

#include <stdint.h>

void reconfigure_fpgaMultibootClearComplete_internal();
void reconfigure_fpgaSetDoneReponse_internal(uint8_t response);
void reconfigure_initPtrMultiboot_internal();
void reconfigure_fpgaMultiboot_internal(uint32_t address);

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H
