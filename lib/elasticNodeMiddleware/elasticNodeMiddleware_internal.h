//
// Created by annika on 26.09.19.
//

#ifndef ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
#define ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H

#include <stdint.h>

void elasticnode_fpgaPowerOn_internal(void);
void elasticnode_fpgaPowerOff_internal(void);
void elasticnode_setFpgaSoftReset_internal(void);
void elasticnode_clearFpgaSoftReset_internal(void);
void elasticnode_setFpgaHardReset_internal(void);
void elasticnode_clearFpgaHardReset_internal(void);

#ifdef TEST

void _delay_ms(uint8_t delay);

#else

#include <avr/interrupt.h>

#endif

#endif //ELASTICNODEMIDDLEWARE_ELASTICNODEMIDDLEWARE_INTERNAL_H
