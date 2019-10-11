//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/reconfigure_multiboot.h"

void fpgaMultibootClearComplete_internal() {
    fpgaMultibootCompleteFlag = 0;
}

void fpgaSetDoneReponse_internal(uint8_t response) {
    fpgaMultibootCompleteFlag = response;
}
