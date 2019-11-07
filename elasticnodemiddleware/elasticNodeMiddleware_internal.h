//
// Created by annika on 26.09.19.
//

#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H

#include <stdint.h>

void elasticnode_fpgaPowerOn_internal();
void elasticnode_setFpgaSoftReset_internal();
void elasticnode_clearFpgaSoftReset_internal();

volatile uint8_t *reset_fpga;

#ifdef TEST
void _delay_ms(uint8_t delay);

#endif

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
