
#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H

#include <stdint.h>

#define RESET_DELAY 10

void elasticnode_fpgaPowerOn_internal(void);

void elasticnode_fpgaPowerOff_internal(void);

void elasticnode_setFpgaSoftReset_internal(void);

void elasticnode_clearFpgaSoftReset_internal(void);

void elasticnode_setFpgaHardReset_internal(void);

void elasticnode_clearFpgaHardReset_internal(void);

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
