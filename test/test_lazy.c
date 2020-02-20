#if GARBAGE_COLLECTION_TYPE == HARDCODED
#include "unity.h"
// ##### CHANGED #####
//#include "MemoryManagement.h"
//#include "Task.h"
//#include "InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"

#include <stdlib.h>
// ##### CHANGED #####
//#include "TaskGraphConfig.h"
#include "lib/DynamicMemoryManagement/TaskGraphConfig.h"


void *IMA;
uint8_t indices[AMOUNT_TASKS];

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

void test_MemoryConsumptionBusy()
{
    TaskConfig config = {
            malloc,
            free,
            realloc,
            AMOUNT_TASKS,
            inputArray,
            outputArray,
            dependencies,
            AMOUNT_DEPENDENCIES}; //7.18 us

    IMA = malloc(256);

    findGoodCollectionIndices(indices);

    graph = Task_createTaskGraph(&config);
    uint32_t cleanedMemory = 0;
    uint32_t uncleanedMemory = 0;
    printf("Index,Cleaned,Uncleaned\n");
    for (uint8_t i = 0; i < AMOUNT_TASKS; i++)
    {
        for(uint8_t j = 0; j < AMOUNT_TASKS; j++){
            if(indices[j] == i){
                cleanedMemory -= resultSize();
            }
        }
        garbageCollection();
        Task_prepareTask(graph, &resultSize);
        cleanedMemory += resultSize();
        uncleanedMemory += resultSize();
        printf("%i,%u,%u\n", i, cleanedMemory, uncleanedMemory);
    }
    garbageCollection();
    Task_freeTaskGraph(graph);
}
#endif