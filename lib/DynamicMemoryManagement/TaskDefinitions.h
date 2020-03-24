//
// Created by annika on 28.02.20.
//

#ifndef ELASTICNODEMIDDLEWARE_TASKDEFINITIONS_H
#define ELASTICNODEMIDDLEWARE_TASKDEFINITIONS_H

#include <stdlib.h>
#include <stdint.h>
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




typedef struct Task  Task;

//##### inputNodes = inputTasks, result = address where to store result, inputCount = number of input tasks, counter = for garbage collection
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
//##### id, tasks = set of tasks, amountTasks = how many tasks, freedTasks = number of freed tasks, currentTask = which task is next
struct TaskGraph{
    uint8_t id;
    Task* tasks;
    uint8_t amountTasks;
    uint8_t freedTasks;
    uint8_t currentTask;
};

// ##### CHANGED (name) #####
Task* TaskDefinition_getTask(TaskGraph *graph, uint8_t index);

#endif //ELASTICNODEMIDDLEWARE_TASKDEFINITIONS_H
