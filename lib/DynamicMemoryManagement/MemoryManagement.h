#ifndef _MEMORYMANAGEMENT_H
#define _MEMORYMANAGEMENT_H
#include "Task.h"
#include "stack_depth.h"

#define TRACING 0
#define REFERENCE_COUNTING 1
#define HARDCODED 2
#define GARBAGE_COLLECTION_TYPE REFERENCE_COUNTING

#define MM_MARK 0
#define MM_FREED 1


void MM_tracingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr));
void MM_referenceCountingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr));
void MM_hardcodedGarbageCollectorRun(TaskGraph *graph, uint8_t *indices, void (*deallocation)(void *ptr));
uint8_t MM_taskIsFreed(TaskGraph *graph, uint8_t index);

#endif // _MEMORYMANAGEMENT_H
