// ##### CHANGED #####
/*
#include "InputManagement.h"
#include "MemoryManagement.h"
#include "Task.h"
#include <avr/io.h>
#include "heaps.h"
#include "malloc_mw.h"
#include "xmem.h"
#include "TaskGraphConfig.h"
#include <stdio.h>
#include "stack_depth.h"
#include "Uart.h"
*/

#include "lib/DynamicMemoryManagement/InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include <avr/io.h>
#include "lib/DynamicMemoryManagement/heaps.h"
#include "lib/DynamicMemoryManagement/malloc_mw.h"
#include "lib/xmem/xmem.h"
#include "lib/DynamicMemoryManagement/TaskGraphConfig.h"
#include <stdio.h>
#include "lib/DynamicMemoryManagement/stack_depth.h"
#include "lib/uart/uart.h"

void *IMA;


#if GARBAGE_COLLECTION_TYPE == HARDCODED
uint8_t indices[AMOUNT_TASKS];

 // Thanks to our dependencies structure we can say that after the last output Node was executed,
 // we can free the task in the next index.

void findGoodCollectionIndices(uint8_t *indices)
{
    int currentTask = 0;
    for (int i = 0; i < AMOUNT_DEPENDENCIES; i++)
    {
        if (dependencies[2 * i] > currentTask)
        {
            indices[currentTask] = dependencies[2 * i - 1] + 1;
            currentTask++;
        }
    }
    indices[currentTask] = AMOUNT_TASKS;
}
#endif
TaskGraph *graph;

static inline void garbageCollection()
{
#if GARBAGE_COLLECTION_TYPE == TRACING
    MM_tracingGarbageCollectorRun(graph, free);
#elif GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
    MM_referenceCountingGarbageCollectorRun(graph, free);
#elif GARBAGE_COLLECTION_TYPE == HARDCODED
    MM_hardcodedGarbageCollectorRun(graph, indices, free);
#endif
}

int main()
{
    DDRB |= (1 << PB7) | (1 << PB6) | (1 << PB5) | (1 << PB4);
    DDRH |= (1 << PH6);
    DDRE |= (1 << PE1);

    uint16_t IMA_size = 256;
    heaps_init(IMA_size);

    // ##### CHANGED #####
    //xmem_init();
    //xmem_enable();
    xmem_initXmem();
    xmem_enableXmem();

#ifdef UART_DEBUG
    uart_init();
#endif

    switch_heap(EXTERNAL_HEAP_INDEX);

    TaskConfig config = {
            malloc_mw,
            free_mw,
            realloc_mw,
            AMOUNT_TASKS,
            inputArray,
            outputArray,
            dependencies,
            AMOUNT_DEPENDENCIES};

    IMA = XMEM_OFFSET;
    //heaps[1].malloc_heap_start += 256;
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif

#if GARBAGE_COLLECTION_TYPE == HARDCODED
    findGoodCollectionIndices(indices);
#endif

    PORTB |= (1 << PB7);
    PORTB |= (1 << PB6);

    PORTB |= (1 << PB5);
    graph = Task_createTaskGraph(&config);
    PORTB &= ~(1 << PB5);
    for (uint8_t i = 0; i < AMOUNT_TASKS; i++)
    {
#ifdef UART_DEBUG
        printStackPointer(__FUNCTION_NAME__, SP);
#endif
        PORTB |= (1 << PB4);
        garbageCollection();
        PORTB &= ~(1 << PB4);
        PORTH |= (1 << PH6);
        Task_prepareTask(graph, &resultSize);
        PORTH &= ~(1 << PH6);

    }
    PORTB |= (1 << PB4);
    garbageCollection();
    PORTB &= ~(1 << PB4);

    Task_freeTaskGraph(graph);
    PORTB &= ~(1 << PB6);
    PORTB &= ~(1 << PB7);
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    return 0;
}