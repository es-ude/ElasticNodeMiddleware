//
// Created by annika on 08.01.20.
//

#ifndef ELASTICNODEMIDDLEWARE_CIRCULARBUFFER_H
#define ELASTICNODEMIDDLEWARE_CIRCULARBUFFER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    uint8_t *head;
    uint8_t *tail;
    uint8_t *last;
    uint16_t currentLen;
    uint16_t maxLen;
} circularBuffer;

void circularBuffer_Init(circularBuffer *c, uint16_t size);

uint16_t circularBuffer_Space(circularBuffer *c);
uint16_t circularBuffer_Count(circularBuffer *c);
uint16_t circularBuffer_CountObjects(circularBuffer *c, uint16_t size);

uint8_t circularBuffer_Push(circularBuffer *c, uint8_t data);
uint8_t circularBuffer_Pop(circularBuffer *c, uint8_t *data);
#endif //ELASTICNODEMIDDLEWARE_CIRCULARBUFFER_H
