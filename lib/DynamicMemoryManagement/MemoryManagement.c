#include "MemoryManagement.h"

// ##### CHANGED ##### from .h in .c
#include "lib/DynamicMemoryManagement/stack_depth.h"

static uint8_t isMarked(Task *task)
{
    return task->counter & (1 << MM_MARK);
}

static uint8_t isFreed(Task *task)
{
#if GARBAGE_COLLECTION_TYPE == TRACING
    return task->counter & ~(1 << MM_MARK);
#elif GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
    return task->counter == 0;
#elif GARBAGE_COLLECTION_TYPE == HARDCODED
    return task->counter & (1 << MM_MARK);
#endif
}

static void setMarked(Task *task)
{
    task->counter |= (1 << MM_MARK);
}

static void removeMark(Task *task)
{
    task->counter &= ~(1 << MM_MARK);
}

/**
 * We clean up the task by deallocating all dynamically allocated memory allocated by the task.
 * */
static void cleanUpTask(Task *task, void (*deallocation)(void *ptr))
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    if (task->result != NULL)
    {
        deallocation(task->result);
        task->result = NULL;
    }
    if (task->inputNodes != NULL)
    {
        deallocation(task->inputNodes);
        task->inputNodes = NULL;
    }
}

#if GARBAGE_COLLECTION_TYPE == TRACING
/**
 * We check whether our task is marked and if it is not yet marked, we mark it to indicate
 * that it was already visited to save computing power.
 * If the task is still not yet executed (it's index is bigger or equal than the currently executed task in the graph),
 * we call mark on all predecessors of the task.
 * 
 * A dead task is found when all of its successors have already been executed and didn't call mark on the dead task.
 * */
static void mark(TaskGraph *graph)
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    for (uint8_t i = graph->currentTask; i < graph->amountTasks; i++)
    {
        Task *task = Task_getTask(graph, i);
        setMarked(task);
        for (uint8_t j = 0; j < task->inputCount; j++)
        {
            uint8_t predecessorIndex = task->inputNodes[j];
            Task *predecessor = Task_getTask(graph, predecessorIndex);
            setMarked(predecessor);
        }
    }
}

/**
 * For the sweeping phase we iterate over all tasks and check whether they have been marked.
 * If it has been marked, the mark is removed.
 * 
 * Otherwise we free the task by deallocating its result, the inputnodes and the outputnodes.
 * 
 * If some node has been found to be freed, we mark it as such and if any nodes were marked this way,
 * we try to shrink the Task Buffer
 * */
static void sweep(TaskGraph *graph, void (*deallocation)(void *ptr))
{

    uint8_t hasFoundSomethingToFree = 0;
    for (uint8_t i = 0; i < graph->amountTasks; i++)
    {
        Task *task = Task_getTask(graph, i);
        if (isMarked(task))
        {
            removeMark(task);
        }
        else
        {
            cleanUpTask(task, deallocation);
            hasFoundSomethingToFree = 1;
        }
    }
    if (hasFoundSomethingToFree)
    {
        Task_realloc(graph);
    }
    #ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
}

/**
 * For the garbage collection we use a mark and sweep algorithm.
 * We traverse the graph initially and mark all nodes that can be reached from the root node.
 * In the sweeping run we free all nodes that are not marked.
 * 
 * We traverse from the last node to the front, because we want to mark all nodes that are alive.
 * */
void MM_tracingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr))
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    mark(graph);
    sweep(graph, deallocation);
}
#endif

/**
 * Every time we prepare the input for a task, we decrease the Reference counter for all input tasks.
 * The reference counter is initialized with the amount of successor nodes of a task.
 * We check for all tasks, whether their respective counter has reached zero and if so, we
 * first deallocate all of its buffers and then check in Task_realloc whether we can shrink the task array.
 * 
 * It is important to not check the last task, as it doesn't have output nodes and it's reference counter will
 * always be zero.
 * */
#if GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
void MM_referenceCountingGarbageCollectorRun(TaskGraph *graph, void (*deallocation)(void *ptr))
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    uint8_t taskWasCleanedUp = 0;
    for (uint8_t index = graph->freedTasks; index < graph->amountTasks - 1; index++)
    {
        Task *task = TaskDefinition_getTask(graph, index);
        if (task->counter == 0)
        {
            cleanUpTask(task, deallocation);
            taskWasCleanedUp = 1;
        }
    }
    if (taskWasCleanedUp)
    {
        Task_realloc(graph);
    }
}
#endif

/* #####
 * iterate over all task and check if indicies array contains current task for any task
 * indicies array = stores for each task the last task, which results are needed, after that it can be freed
 * if yes --> clean up task, mark it in counter as freed, set taskWasCleanedUp to a non-zero value
 * At the end of loop: if taskWasCleanedUp != 0 --> at least one task was freed --> might shrink array
 */
#if GARBAGE_COLLECTION_TYPE == HARDCODED
void MM_hardcodedGarbageCollectorRun(TaskGraph *graph, uint8_t *indices, void (*deallocation)(void *ptr))
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    uint8_t taskWasCleanedUp = 0;
    for (uint8_t i = graph->freedTasks; i < graph->amountTasks - 1; i++)
    {
        if (graph->currentTask == indices[i])
        {
            Task *task = Task_getTask(graph, i);
            cleanUpTask(task, deallocation);
            setMarked(task);
            taskWasCleanedUp = 1;
        }
    }
    if (taskWasCleanedUp)
    {
        Task_realloc(graph);
    }
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
}
#endif