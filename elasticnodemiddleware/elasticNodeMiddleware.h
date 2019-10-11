//
// Created by annika on 17.09.19.
//

#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H

/* *
 * initialise the middleware
 *  - with whatever is necessary
 *
 * FPGA direct control:
 * turn on FPGA
 * turn off FPGA
 * put FPGA to "some" sleep mode
 * reconfigure FPGA to specific state
 *  - This can potentially need different options for how a configuration is loaded.
 * write configuration onto FPGA flash (optional for now)
 *
 * write data to FPGA (blocking)
 * read data from FPGA (blocking)
 * write data to FPGA (non-blocking)
 * read data from FPGA (non-blocking)
 * */

#include <stdint.h>
#define FPGA_DONE_NOTHING 0
#define FPGA_DONE_PRINT 1

void elasticnode_initialise();

void elasticnode_fpgaPowerOn();
void elasticnode_fpgaPowerOff();

/*
 * TODO: implement
void elasticnode_fpgaSleep(uint8_t sleepmode);
*/

//'c'
void elasticnode_configure();
uint8_t elasticnode_getLoadedConfiguration();

void elasticnode_writeDataBlocking(uint8_t* address, uint8_t data);
uint8_t elasticnode_readDataBlocking();

void elasticnode_writeDataNonBlocking(uint8_t* address, uint8_t data);
uint8_t elasticnode_readDataNonBlocking(uint8_t* address);

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
