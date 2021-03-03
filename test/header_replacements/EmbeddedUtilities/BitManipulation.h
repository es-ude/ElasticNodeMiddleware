#ifndef ELASTICNODEMIDDLEWARE_BITMANIPULATION_TEST_H
#define ELASTICNODEMIDDLEWARE_BITMANIPULATION_TEST_H

#include <stdint.h>
#include <stdbool.h>

void BitManipulation_clearBit(volatile uint8_t *byte_ptr, uint8_t offset);

void BitManipulation_setBit(volatile uint8_t *byte_ptr, uint8_t offset);

void BitManipulation_setByte(volatile uint8_t *byte_ptr, uint8_t bitmask, uint8_t value);

bool BitManipulation_bitIsSetOnArray(volatile const uint8_t *field, uint8_t offset);

#endif //ELASTICNODEMIDDLEWARE_BITMANIPULATION_TEST_H
