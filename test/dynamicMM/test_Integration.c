#include "unity.h"
// ##### CHANGED #####
//#include "Task.h"
//#include "InputManagement.h"
//#include "MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagementDefintions.h"
#include "lib/DynamicMemoryManagement/TaskDefinitions.h"
#include "lib/DynamicMemoryManagement/Task_Freed.h"

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
    if (ptr != NULL)
    {
        --allocationCount;
        free(ptr);
    }
}

void *IMA;

//    A
//   / \
//  B   C
//   \ /
//    D
// A calculates a random number
// B calculates the biggest prime smaller than that number
// C calculates the smalles prime bigger than that number
// D calculates the product

#define A 0
#define B 1
#define C 2
#define D 3
#define AMOUNT_TASKS 4

uint8_t inputArray[AMOUNT_TASKS] = {0, 1, 1, 2};
uint8_t outputArray[AMOUNT_TASKS] = {2, 1, 1, 0};
#define AMOUNT_DEPENDENCIES 4
uint8_t dependencies[AMOUNT_DEPENDENCIES * 2] =
        {A, B,
         A, C,
         B, D,
         C, D};

void (*tasks[4])();



uint16_t (*resultSizeFunctions[4])();
uint16_t resultSizes[AMOUNT_TASKS] = {1, 1, 1, 2};
uint16_t resultSizeA()
{
    return sizeof(uint8_t);
}
uint16_t resultSizeB()
{
    return sizeof(uint8_t);
}
uint16_t resultSizeC()
{
    return sizeof(uint8_t);
}
uint16_t resultSizeD()
{
    return sizeof(uint16_t);
}

void *getInput(uint8_t index)
{
    void **heapPtr = (void **)IMA;
    return heapPtr[index];
}

void taskA()
{
    uint8_t *iptr = getInput(0);
    uint8_t random = 20; //yes this is a random numer.
    *iptr = random;
}

uint8_t isPrime(uint8_t number)
{
    for (uint8_t j = 2; j < number; j++)
    {
        if (number % j == 0)
        {
            return 0;
        }
    }
    return 1;
}

void taskB()
{
    uint8_t prime = *(uint8_t *)getInput(0);
    while (!isPrime(--prime))
        ;
    uint8_t *iptr = (uint8_t *)getInput(1);
    *iptr = prime;
}

void taskC()
{
    uint8_t prime = *(uint8_t *)getInput(0);
    while (!isPrime(++prime))
        ;
    uint8_t *iptr = (uint8_t *)getInput(1);
    *iptr = prime;
}

void taskD()
{
    uint16_t x = *(uint8_t *)getInput(0);
    uint16_t y = *(uint8_t *)getInput(1);
    uint16_t res = x * y;
    uint16_t *resultPtr = (uint16_t *)getInput(2);
    *resultPtr = res;
}

#if GARBAGE_COLLECTION_TYPE == HARDCODED
uint8_t indices[AMOUNT_TASKS];
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
    tasks[A] = taskA;
    tasks[B] = taskB;
    tasks[C] = taskC;
    tasks[D] = taskD;

    resultSizeFunctions[A] = resultSizeA;
    resultSizeFunctions[B] = resultSizeB;
    resultSizeFunctions[C] = resultSizeC;
    resultSizeFunctions[D] = resultSizeD;

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

void test_taskA()
{
    for (int task = A; task <= A; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }

    uint8_t *iptr = (uint8_t *) TaskDefinition_getTask(graph, A)->result;
    TEST_ASSERT_EQUAL_UINT8(20, *iptr);
    for (int task = B; task <= D; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }
    garbageCollection();
}

void test_taskB()
{
    for (int task = A; task <= B; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }

    uint8_t *iptr = (uint8_t *) TaskDefinition_getTask(graph, B)->result;
    TEST_ASSERT_EQUAL_UINT8(19, *iptr);
    for (int task = C; task <= D; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }
    garbageCollection();
}

void test_taskC()
{
    for (int task = A; task <= C; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }

    uint8_t *iptr = (uint8_t *) TaskDefinition_getTask(graph, C)->result;
    TEST_ASSERT_EQUAL_UINT8(23, *iptr);
    for (int task = D; task <= D; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }
    garbageCollection();
}

void test_taskD()
{
    for (int task = A; task <= D; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }

    uint16_t *iptr = (uint16_t *) TaskDefinition_getTask(graph, D)->result;
    TEST_ASSERT_EQUAL_UINT16(437, *iptr);
    garbageCollection();
}