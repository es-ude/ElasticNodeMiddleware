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
#define FPGA_DONE_MULTIBOOT 2

volatile uint8_t* ptr_xmem_offset;
#define RESET_DELAY 10

void elasticnode_initialise();

void elasticnode_fpgaPowerOff();

/*
 * TODO: implement
void elasticnode_fpgaSleep(uint8_t sleepmode);
*/

void elasticnode_configureFrom(uint32_t address);
uint8_t elasticnode_getLoadedConfiguration();

void elasticnode_writeOneByteBlocking(uint8_t address, uint8_t data);
void elasticnode_writeDataBlocking(uint8_t address, uint8_t size, uint8_t* ptr_data);
uint8_t elasticnode_readOneByteBlocking(uint8_t address);
void elasticnode_readDataBlocking(uint8_t address, uint8_t size, uint8_t* ptr_return);

//TODO: read + write non-blocking might be implemented

void elasticnode_initReset_FPGA();
void elasticnode_fpgaSoftReset();

#ifdef TEST

void cli();
void sei();

#else

#include <avr/interrupt.h>
#include <util/delay.h>

#endif

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
