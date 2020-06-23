#include "lib/uart/circularBuffer/circularBuffer.h"
#include "lib/interruptManager/interruptManager.h"
#include "lib/pinDefinition/fpgaRegisters.h"

void circularBuffer_Init(circularBuffer *c, uint16_t size){

    // allocate buffer
    c->buffer = (uint8_t *) malloc(size);
    if (!c->buffer)
    {
        // CAUTION: no error message for inadequate memory
    }
    else
    {
        // init head and tail pointers
        c->head = c->buffer;
        c->tail = c->buffer;
        c->last = c->buffer + size - 1;
        c->currentLen = 0;
        c->maxLen = size;
    }
}

uint16_t circularBuffer_Space(circularBuffer *c){
    return c->maxLen - c->currentLen;
}

uint16_t circularBuffer_Count(circularBuffer *c){
    return c->currentLen;
}

uint16_t circularBuffer_CountObjects(circularBuffer *c, uint16_t size){
    return circularBuffer_Count(c) / size;
}

uint8_t circularBuffer_Push(circularBuffer *c, uint8_t data){

    //uint8_t gi = interruptStatus();
    uint8_t gi = SREG & (1<<7);

   //if (gi) = if(SREG & (1<<7)) =
   if(gi) {
       interruptManager_clearInterrupt();
   }

   // check if buffer is full
   if (c->currentLen == c->maxLen)
   {
       if (gi) {
           interruptManager_setInterrupt();
       }
       return 0;
   }

   // store new data
   *c->head = data;

   // find pointer for next data
   c->head++;
   // circle around when head passes too far
   if (c->head > c->last)
       c->head = c->buffer;

   // counter for convenience
   c->currentLen++;

   if (gi) {
       interruptManager_setInterrupt();
   }
   return 1;

    // return success to indicate successful push.
}

uint8_t circularBuffer_Pop(circularBuffer *c, uint8_t *data){

     // check if available
    if (!c->currentLen)
        return 0;

    //uint8_t gi = interruptStatus();
    uint8_t gi = SREG & (1<<7);

    if (gi) {
        interruptManager_clearInterrupt();
    }
    // data exists
    c->currentLen--;

    // fetch data
    *data = *c->tail;

    // next data
    c->tail++;
    if (c->tail > c->last)
        c->tail = c->buffer;

    if (gi) {
        interruptManager_setInterrupt();
    }

    return 1;  // return success to indicate successful pop.

}