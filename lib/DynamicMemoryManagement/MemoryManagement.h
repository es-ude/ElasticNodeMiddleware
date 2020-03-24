#ifndef _MEMORYMANAGEMENT_H
#define _MEMORYMANAGEMENT_H
#include "Task.h"
#include "stack_depth.h"

// ##### CHANGED #####
#include "lib/DynamicMemoryManagement/TaskDefinitions.h"
#include "lib/DynamicMemoryManagement/MemoryManagementDefintions.h"


void MM_tracingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr));
void MM_referenceCountingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr));
void MM_hardcodedGarbageCollectorRun(TaskGraph *graph, uint8_t *indices, void (*deallocation)(void *ptr));

#endif // _MEMORYMANAGEMENT_H
