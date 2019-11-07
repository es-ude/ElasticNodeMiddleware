//
// Created by annika on 10.10.19.
//

#include "elasticnodemiddleware/reconfigure_multiboot_internal.h"
#include "elasticnodemiddleware/reconfigure_multiboot.h"
#include "elasticnodemiddleware/elasticNodeMiddleware.h"
#include "elasticnodemiddleware/elasticNodeMiddleware_internal.h"
#include "elasticnodemiddleware/xmem.h"


void reconfigure_fpgaMultibootClearComplete_internal() {
    fpgaMultibootCompleteFlag = 0;
}

void reconfigure_fpgaSetDoneReponse_internal(uint8_t response) {
    fpgaMultibootCompleteFlag = response;
}

//raus
void reconfigure_initPtrMultiboot_internal() {
    //multiboot = (uint8_t *) (XMEM_OFFSET + 0x05);
}
/*
void reconfigure_fpgaMultiboot_internal(uint32_t address) {

    elasticnode_fpgaPowerOn_internal();

    enableXmem();

    reconfigure_fpgaSetDoneReponse_internal(FPGA_DONE_PRINT);
    reconfigure_fpgaMultibootClearComplete_internal();

     for (uint8_t i = 0; i < 3; i++)
      {
          *(multiboot + i) = (uint8_t) (0xff & (address >> (i * 8)));
      }
    disableXmem();
}*/