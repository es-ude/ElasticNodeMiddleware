#include "unity.h"
// ##### CHANGED #####
//#include "Task.h"
//#include "InputManagement.h"
//#include "MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"

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

void *IMA = NULL;

//    A
//   / \
//  B   C
//   \ /
//    D
// A calculates three numbers
// B fills a matrix with dimensions of first two numbers
// C fills a matrix with dimensions of second two numbers
// D calculates matrix multiplication

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

uint16_t (*resultSizeFunctions[4])();

typedef struct Matrix{
    uint8_t rows;
    uint8_t columns;
    uint8_t data[];
} Matrix;

void *getInput(uint8_t index)
{
    void **heapPtr = (void **)IMA;
    return heapPtr[index];
}

uint16_t resultSizeA()
{
    return 3 * sizeof(uint8_t);
}
uint16_t resultSizeB()
{
    uint8_t *input = (uint8_t *)getInput(0);
    uint8_t rows = input[0];
    uint8_t columns = input[1];
    return rows * columns * sizeof(uint8_t) + sizeof(rows)* sizeof(uint8_t) + sizeof(columns)* sizeof(uint8_t);
}
uint16_t resultSizeC()
{
    uint8_t *input = (uint8_t *)getInput(0);
    uint8_t rows = input[1];
    uint8_t columns = input[2];
    return rows * columns * sizeof(uint8_t) + sizeof(rows)* sizeof(uint8_t) + sizeof(columns)* sizeof(uint8_t);
}
uint16_t resultSizeD()
{
    Matrix *mB = (Matrix *)getInput(0);
    Matrix *mC = (Matrix *)getInput(1);
    uint16_t result =  mB->rows * mC->columns * sizeof(uint8_t) + sizeof(mB->rows) * sizeof(uint8_t) + sizeof(mC->columns) * sizeof(uint8_t);
    return result;
}

uint8_t M_get(Matrix* matrix, uint8_t row, uint8_t column){
    return matrix->data[matrix->columns * row + column];
}

void resetMatrix(Matrix* m){
    for(int i = 0; i < m->rows * m->columns; i++){
        m->data[i] = 0;
    }
}

void taskA()
{
    uint8_t *iptr = getInput(0);
    iptr[0] = 2;
    iptr[1] = 3;
    iptr[2] = 2;
}

void taskB()
{
    uint8_t *input = (uint8_t *)getInput(0);
    uint8_t rows = input[0];
    uint8_t columns = input[1];
    Matrix *output = (Matrix *)getInput(1);
    output->rows = rows;
    output->columns = columns;
    resetMatrix(output);
    for (int i = 0; i < rows * columns; i++)
    {
        output->data[i] = i + 1;
    }
}

void taskC()
{
    uint8_t *input = (uint8_t *)getInput(0);
    uint8_t rows = input[1];
    uint8_t columns = input[2];
    Matrix *output = (Matrix *)getInput(1);
    output->rows = rows;
    output->columns = columns;
    resetMatrix(output);

    for (int i = 0; i < rows * columns; i++)
    {
        output->data[i] = i + 7;
    }
}


void taskD()
{
    Matrix *mB = (Matrix *)getInput(0);
    Matrix *mC = (Matrix *)getInput(1);
    Matrix *mD = (Matrix *)getInput(2);
    mD->rows = mB->rows;
    mD->columns = mC->columns;
    resetMatrix(mD);
    uint8_t dim = mB->columns;
    for (int row = 0; row < mD->rows; row++) {
        for (int column = 0; column < mD->columns; column++) {
            for (int i = 0; i < dim; i++) {
                mD->data[row*mD->columns + column] += M_get(mB, row, i) * M_get(mC, i, column);
            }
        }
    }
}

TaskGraph *graph = NULL;
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

void test_TaskA(void)
{
    for (int task = A; task <= A; task++)
    {
        garbageCollection();
        Task_prepareTask(graph, resultSizeFunctions[task]);
        tasks[task]();
    }
    uint8_t *iptr = (uint8_t *)Task_getTask(graph, A)->result;
    TEST_ASSERT_EQUAL_UINT8(2, iptr[0]);
    TEST_ASSERT_EQUAL_UINT8(3, iptr[1]);
    TEST_ASSERT_EQUAL_UINT8(2, iptr[2]);
    for(int task = B; task <= D; task++){
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
    uint8_t *iptr = (uint8_t *)Task_getTask(graph, B)->result;
    TEST_ASSERT_EQUAL_UINT8(2, iptr[0]);
    TEST_ASSERT_EQUAL_UINT8(3, iptr[1]);
    for (int i = 0; i < 6; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i + 1, iptr[i + 2]);
    }
    for(int task = C; task <= D; task++){
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
        Task_prepareTask(graph,  resultSizeFunctions[task]);
        tasks[task]();
    }
    uint8_t *iptr = (uint8_t *)Task_getTask(graph, C)->result;
    TEST_ASSERT_EQUAL_UINT8(3, iptr[0]);
    TEST_ASSERT_EQUAL_UINT8(2, iptr[1]);
    for (int i = 0; i < 6; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i + 7, iptr[i + 2]);
    }
    for(int task = D; task <= D; task++){
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
    uint8_t *iptr = (uint8_t *)Task_getTask(graph, D)->result;
    TEST_ASSERT_EQUAL_UINT8(2, iptr[0]);
    TEST_ASSERT_EQUAL_UINT8(2, iptr[1]);
    TEST_ASSERT_EQUAL_UINT8(58, iptr[2]);
    TEST_ASSERT_EQUAL_UINT8(64, iptr[3]);
    TEST_ASSERT_EQUAL_UINT8(139, iptr[4]);
    TEST_ASSERT_EQUAL_UINT8(154, iptr[5]);
    garbageCollection();
}