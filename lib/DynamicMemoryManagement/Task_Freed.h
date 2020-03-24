//
// Created by annika on 28.02.20.
//

#ifndef ELASTICNODEMIDDLEWARE_TASK_FREED_H
#define ELASTICNODEMIDDLEWARE_TASK_FREED_H

#include <stdio.h>
#include "lib/DynamicMemoryManagement/TaskDefinitions.h"

uint8_t Task_Freed_taskIsFreed(TaskGraph *graph, uint8_t index);
static uint8_t isFreed(Task *task);

#endif //ELASTICNODEMIDDLEWARE_TASK_FREED_H
