//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/reconfigure_multiboot.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/xmem.h"


void fpgaMultibootClearComplete_internal() {
    fpgaMultibootCompleteFlag = 0;
}

void fpgaSetDoneReponse_internal(uint8_t response) {
    fpgaMultibootCompleteFlag = response;
}

void initPtrMultiboot_internal() {
    multiboot = (uint8_t *) (XMEM_OFFSET + 0x05);
}

void fpgaMultiboot_internal(uint32_t address) {

    elasticnode_fpgaPowerOn();

    enableXmem();

    fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    fpgaMultibootClearComplete_internal();

     for (uint8_t i = 0; i < 3; i++)
      {
          *(multiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
      }
    disableXmem();
}