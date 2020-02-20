#include "unity.h"
// ##### CHANGED #####
//#include "Task.h"
//#include "InputManagement.h"
//#include "MemoryManagement.h"
#include "lib/DynamicMemoryManagement/Task.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"
#include "lib/DynamicMemoryManagement/MemoryManagement.h"

#include <stdlib.h>

static uint16_t allocationCount = 0;

void* safe_allocation(size_t size){
    void* ptr = malloc(size);
    allocationCount++;
    return ptr;
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

#define A 0
#define B 1
#define C 2
#define D 3
#define AMOUNT_TASKS 4

uint8_t inputArray[4] = {0,1,1,2};
uint8_t outputArray[4] = {2,1,1,0};
#define AMOUNT_DEPENDENCIES 4
uint8_t dependencies[AMOUNT_DEPENDENCIES * 2] =
        {A,B,
         A,C,
         B,D,
         C,D};
uint16_t resultSizes[AMOUNT_TASKS] = {1,1,1,2};

TaskGraph* graph;
void *IMA;

void setUp(void)
{
    TaskConfig config = {
            safe_allocation,
            safe_free,
            realloc,
            AMOUNT_TASKS,
            inputArray,
            outputArray,
            dependencies,
            AMOUNT_DEPENDENCIES,
            0
    };
    graph = Task_createTaskGraph(&config);
}

void tearDown(void)
{
    Task_freeTasks(graph);
    Task_freeTaskGraph(graph);
    TEST_ASSERT_EQUAL_UINT16(0, allocationCount);
}

void test_currentTaskZero(void){
    TEST_ASSERT_EQUAL(0, graph->currentTask);
}

void test_taskInput(){
    TEST_ASSERT_EQUAL_UINT8(0, Task_getTask(graph,0)->inputCount);
    TEST_ASSERT_EQUAL_UINT8(1, Task_getTask(graph,1)->inputCount);
    TEST_ASSERT_EQUAL_UINT8(1, Task_getTask(graph,2)->inputCount);
    TEST_ASSERT_EQUAL_UINT8(2, Task_getTask(graph,3)->inputCount);
}

void test_dependencies(){

    TEST_ASSERT_EQUAL_PTR(Task_getTask(graph,0), Task_getTask(graph,Task_getTask(graph,1)->inputNodes[0]));
    TEST_ASSERT_EQUAL_PTR(Task_getTask(graph,0), Task_getTask(graph,Task_getTask(graph,2)->inputNodes[0]));
    TEST_ASSERT_EQUAL_PTR(Task_getTask(graph,1), Task_getTask(graph,Task_getTask(graph,3)->inputNodes[0]));
    TEST_ASSERT_EQUAL_PTR(Task_getTask(graph,2), Task_getTask(graph,Task_getTask(graph,3)->inputNodes[1]));
}


int sizeOfGraph(TaskGraph* graph){
    int size = 0;
    for(int i = 0; i < 4; i++){
        size += sizeof(graph->tasks[i].inputCount);
        size += sizeof(graph->tasks[i].result);
        size += sizeof(graph->tasks[i].inputNodes);
        size += sizeof(graph->tasks[i].counter);
        size += graph->tasks[i].inputCount * sizeof(uint8_t);
    }
    size += sizeof(graph->id);
    size += sizeof(graph->tasks);
    size += sizeof(graph->amountTasks);
    size += sizeof(graph->currentTask);
    size += sizeof(graph->freedTasks);
    return size;
}

int sizeOfGraphOnArduino(TaskGraph* graph){
    int size = 0;
    for(int i = 0; i < 4; i++){
        size += sizeof(graph->tasks[i].inputCount);
        size += sizeof(graph->tasks[i].result)/4;
        size += sizeof(graph->tasks[i].inputNodes)/4;
        size += sizeof(graph->tasks[i].counter);
        size += graph->tasks[i].inputCount * sizeof(uint8_t);
    }
    size += sizeof(graph->id);
    size += sizeof(graph->tasks)/4;
    size += sizeof(graph->amountTasks);
    size += sizeof(graph->currentTask);
    size += sizeof(graph->freedTasks);
    return size;
}

/*
typedef struct Task{
    uint8_t *inputNodes;        8 + inputCount*1
    void* result;               8
    uint8_t inputCount;         1
    uint8_t counter;            1
} Task;

struct TaskGraph{
    uint8_t id;                 1
    Task* tasks;                8
    uint8_t amountTasks;        1
    uint8_t freedTasks;         1
    uint8_t currentTask;        1
};  Total 12 Byte
size(Task) = 18 + i
size(A) = 18 = 18
size(B) = 18 + 1 = 19
size(C) = 18 + 1 = 19
size(D) = 18 + 2 = 20
size(Tasks) = 76 Byte

size(TaskGraph) = 12

*/
//    A
//   / \
//  B   C
//   \ /
//    D
// A calculates a random number
// B calculates the biggest prime smaller than that number
// C calculates the smalles prime bigger than that number
// D calculates the product

void test_sizeOfGraph(){
    TEST_ASSERT_EQUAL_INT(88, sizeOfGraph(graph));
}

void test_sizeOfGraphOnArduino(){
    TEST_ASSERT_EQUAL_INT(34, sizeOfGraphOnArduino(graph));
}