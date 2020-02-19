#ifndef _TASK_H
#define _TASK_H
#include "InputManagement.h"
#include <stdlib.h>
#include <stdint.h>
#include "stack_depth.h"
extern void* IMA;

#define DEBUG_REALLOC

/*
    For each Task we need to know all input nodes
    A: {}
    B: {A}
    C: {A}
    D: {B,C}

    For memory Management we need to know all output nodes
    A: {B,C}
    B: {D}
    C: {D}
    D: {}

    Each task has a pointer to its result
    Each task knows how many input nodes it has
    Each task knows how many output nodes it has
*/




typedef struct Task Task;

struct Task{
    uint8_t *inputNodes;
    void* result;
    uint8_t inputCount;
    uint8_t counter;
};

typedef struct TaskConfig{
    void* (*allocate)(size_t size);
    void (*deallocate)(void *ptr);
    void* (*reallocate)(void *ptr, size_t len);
    uint8_t amountTasks;
    uint8_t *inputArray;
    uint8_t *outputArray;
    uint8_t *dependencies;
    uint16_t dependenciesLength;
    uint8_t taskGraphID;
} TaskConfig;

typedef struct TaskGraph TaskGraph;

struct TaskGraph{
    uint8_t id;
    Task* tasks;
    uint8_t amountTasks;
    uint8_t freedTasks;
    uint8_t currentTask;
};

TaskGraph* Task_createTaskGraph(TaskConfig* config);
void Task_freeTaskGraph(TaskGraph* graph);
Task* Task_getTask(TaskGraph* graph, uint8_t index);
void Task_freeTasks(TaskGraph* graph);
void Task_realloc(TaskGraph* graphPtr);

void Task_prepareTask(TaskGraph* graph, uint16_t (*calculateResultSize)(void));


#endif // _TASK_H
