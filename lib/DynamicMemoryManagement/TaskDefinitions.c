//
// Created by annika on 28.02.20.
//

#include "lib/DynamicMemoryManagement/TaskDefinitions.h"

//reverse array for letting it shrink easily in Task_realloc
Task *TaskDefinition_getTask(TaskGraph *graph, uint8_t index)
{
    return &graph->tasks[graph->amountTasks - index - 1];
}