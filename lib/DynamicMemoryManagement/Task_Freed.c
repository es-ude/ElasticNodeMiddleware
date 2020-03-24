//
// Created by annika on 28.02.20.
//

#include "lib/DynamicMemoryManagement/Task_Freed.h"
#include "lib/DynamicMemoryManagement/MemoryManagementDefintions.h"

/*
 * ##### how to decide if a task has been freed in the last garbage collection
 */
uint8_t Task_Freed_taskIsFreed(TaskGraph *graph, uint8_t index)
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    return isFreed(TaskDefinition_getTask(graph, index));
}

static uint8_t isFreed(Task *task)
{
#if GARBAGE_COLLECTION_TYPE == TRACING
    return task->counter & ~(1 << MM_MARK);
#elif GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
    return task->counter == 0;
#elif GARBAGE_COLLECTION_TYPE == HARDCODED
    return task->counter & (1 << MM_MARK);
#endif
}