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


void elasticnode_initialise(void);
void elasticnode_fpgaPowerOn(void);
void elasticnode_fpgaPowerOff(void);

// TODO: implement void elasticnode_fpgaSleep(uint8_t sleepmode);

void elasticnode_enableFpgaInterface(void);
void elasticnode_disableFpgaInterface(void);

void elasticnode_writeByteToUserlogic(uint8_t userlogicAddr, uint8_t data);
void elasticnode_writeBufferToUserlogic(uint8_t userlogicAddr, uint16_t size, const uint8_t *buffer);
uint8_t elasticnode_readByteFromUserlogic(uint8_t userlogicAddr);
void elasticnode_readBufferFromUserlogic(uint8_t userlogicAddr, uint16_t size, uint8_t *buffer);

void elasticnode_configureFPGA(uint32_t bitfileAddress);
uint32_t elasticnode_getLoadedConfigurationAddress(void);

//void elasticnode_writeOneByteBlockingFromFpga(uint8_t address, uint8_t data);
//void elasticnode_writeDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_data);
//uint8_t elasticnode_readOneByteBlockingFromFpga(uint8_t address);
//void elasticnode_readDataBlockingFromFpga(uint8_t address, uint8_t size, uint8_t *ptr_return);

//TODO: read + write non-blocking might be implemented

void elasticnode_userlogicReset(void);
void elasticnode_fpgaHardReset(void);

//for integration testing
#define FPGA_DONE_MULTIBOOT 2


#ifdef TEST

#else

#include <util/delay.h>

#endif

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_H
