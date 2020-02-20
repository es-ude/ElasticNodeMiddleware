#include "unity.h"
// ##### CHANGED #####
//#include "MemoryManagement.h"
//#include "Task.h"
//#include "InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"

//#define RUNEXPENSIVESHIT

static uint16_t allocationCount = 0;
void *safe_allocation(size_t size)
{
    ++allocationCount;
    return malloc(size);
}

/**
 * We need to test whether the pointer is not NULL, because free(0) is a noop
 * We should decrease allocationcount only if something actually was freed
*/
void safe_free(void *ptr)
{
    if(ptr != NULL){
        --allocationCount;
        free(ptr);
    }
}

void *IMA;




void printInputOutputArray(uint8_t *dependencies, uint8_t amountDependencies, uint8_t amountTasks)
{
    uint8_t input[amountTasks];
    uint8_t output[amountTasks];
    for (int i = 0; i < amountTasks; i++)
    {
        input[i] = 0;
        output[i] = 0;
    }
    for (int i = 0; i < amountDependencies; i++)
    {
        input[dependencies[2 * i + 1]]++;
        output[dependencies[2 * i]]++;
    }
    printf("{");
    for (int i = 0; i < amountTasks; i++)
    {
        printf("%i,", input[i]);
    }
    printf("}\n");

    printf("{");
    for (int i = 0; i < amountTasks; i++)
    {
        printf("%i,", output[i]);
    }
    printf("}\n");
}

uint8_t dependencies[] = {
        0, 1,
        0, 2,
        1, 3,
        1, 4,
        2, 4,
        2, 5,
        2, 6,
        3, 7,
        4, 7,
        5, 8,
        6, 7,
        6, 8,
        7, 9,
        8, 9};
#define AMOUNT_TASKS 10
#define AMOUNT_DEPENDENCIES 14
uint8_t inputArray[AMOUNT_TASKS] = {
        0,
        1,
        1,
        1,
        2,
        1,
        1,
        3,
        2,
        2,
};
uint8_t outputArray[AMOUNT_TASKS] = {
        2,
        2,
        3,
        1,
        1,
        1,
        2,
        1,
        1,
        0,
};
/**
 * Thanks to our dependencies structure we can say that after the last output Node was executed,
 * we can free the task in the next index.
 * */
void findGoodCollectionIndices(uint8_t *indices)
{
    int currentTask = 0;
    for(int i = 0; i < AMOUNT_DEPENDENCIES; i++)
    {
        if(dependencies[2*i] > currentTask){
            indices[currentTask] = dependencies[2*i - 1] + 1;
            currentTask++;
        }
    }
    indices[currentTask] = AMOUNT_TASKS;
}

#if GARBAGE_COLLECTION_TYPE == HARDCODED
uint8_t indices[AMOUNT_TASKS];
#endif
TaskGraph *graph;
TaskConfig config = {
        safe_allocation,
        safe_free,
        realloc,
        AMOUNT_TASKS,
        inputArray,
        outputArray,
        dependencies,
        AMOUNT_DEPENDENCIES,
        0};

void setUp(void)
{
    IMA = safe_allocation(256);
    graph = Task_createTaskGraph(&config);

#if GARBAGE_COLLECTION_TYPE == HARDCODED
    findGoodCollectionIndices(indices);
#endif
}

void tearDown(void)
{
    Task_freeTaskGraph(graph);
    safe_free(IMA);
    TEST_ASSERT_EQUAL_UINT16(0, allocationCount);
}

static void garbageCollection()
{
#if GARBAGE_COLLECTION_TYPE == TRACING
    MM_tracingGarbageCollectorRun(graph, safe_free);
#elif GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
    MM_referenceCountingGarbageCollectorRun(graph, safe_free);
#elif GARBAGE_COLLECTION_TYPE == HARDCODED
    MM_hardcodedGarbageCollectorRun(graph, indices, safe_free);
#endif
}

uint16_t resultSize()
{
    return sizeof(uint8_t);
}

void test_indices()
{
    uint8_t indices[9];
    findGoodCollectionIndices(indices);
    for (int i = 0; i < 9; i++)
    {
        printf("%i\n", indices[i]);
    }
    for (int i = 0; i < AMOUNT_TASKS; i++)
    {
        garbageCollection();
        Task_prepareTask(graph, &resultSize);
    }
    garbageCollection();
}

void test_printInputOutput(void)
{
    printInputOutputArray(dependencies, 14, 10);

    for (int i = 0; i < AMOUNT_TASKS; i++)
    {
        garbageCollection();
        Task_prepareTask(graph, &resultSize);
    }
    garbageCollection();
}

void test_markAndSweep(void)
{
    for (int i = 0; i < AMOUNT_TASKS; i++)
    {
        garbageCollection();
        Task_prepareTask(graph, &resultSize);
    }
    garbageCollection();
}
#ifdef RUNEXPENSIVESHIT
void test_expensiveTest(void)
{
    for (int i = 0; i < 10000000; i++)
    {
        for (int i = 0; i < AMOUNT_TASKS; i++)
        {
            garbageCollection();
            Task_prepareTask(graph, &resultSize, heap);
        }
        garbageCollection();
        Task_freeTaskGraph(graph);
        TEST_ASSERT_EQUAL_UINT16(1, allocationCount);
        graph = Task_createTaskGraph(&config);
    }
    for (int i = 0; i < AMOUNT_TASKS; i++)
    {
        garbageCollection();
        Task_prepareTask(graph, &resultSize, heap);
    }
    garbageCollection();
}
#endif
