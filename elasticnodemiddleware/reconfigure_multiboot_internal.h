//
// Created by annika on 10.10.19.
//

#ifndef ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H


void fpgaMultibootClearComplete_internal();
void fpgaSetDoneReponse_internal(uint8_t response);
void initPtrMultiboot_internal();
void fpgaMultiboot_internal(uint32_t address);

#endif //ELASTICNODEMIDDLEWARE_RECONFIGURE_MULTIBOOT_INTERNAL_H
