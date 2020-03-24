#include "Task.h"
#include "InputManagement.h"

// ##### CHANGED #####
#include "lib/DynamicMemoryManagement/Task_Freed.h"

void (*deallocator)(void *ptr);
void *(*allocator)(size_t size);
void *(*reallocate)(void *ptr, size_t len);

TaskGraph *Task_createTaskGraph(TaskConfig *config)
{
    deallocator = config->deallocate;
    allocator = config->allocate;
    reallocate = config->reallocate;
    Task *graph = config->allocate(sizeof(Task) * config->amountTasks);

    TaskGraph *taskgraph = config->allocate(sizeof(TaskGraph));
    taskgraph->tasks = graph;
    taskgraph->id = config->taskGraphID;
    taskgraph->amountTasks = config->amountTasks;
    taskgraph->freedTasks = 0;
    taskgraph->currentTask = 0;

    for (uint8_t i = 0; i < config->amountTasks; ++i)
    {
        uint8_t index = config->amountTasks - i - 1; //reverse array
        graph[i].inputCount = config->inputArray[index];
        //graph[i].outputCount = config->outputArray[index];
        graph[i].result = NULL;

/**
         * We use the counter in all garbage collection strategies with a different purpose.
         * In reference counting we store the amount of outputs.
         * In tracing garbage collection we store status bits, e.g. MARK and FREED
         * */
#if GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
        graph[i].counter = config->outputArray[index];
#elif GARBAGE_COLLECTION_TYPE == TRACING
        graph[i].counter = 0;
#elif GARBAGE_COLLECTION_TYPE == HARDCODED
        graph[i].counter = 0;
#endif

        /**
         * If the count is bigger than 0, we allocate a buffer to store the index.
         * Unfortunately malloc(0) is undefined behavior, thus we can't
         * assume that the pointer will be NULL and have to set it manually.
         * 
         * Instead of allocating two buffers we allocate one buffer that we use for the input and output nodes.
         * */
        if (graph[i].inputCount > 0)
        {
            graph[i].inputNodes = config->allocate(graph[i].inputCount * sizeof(uint8_t));
            //graph[i].outputNodes = &graph[i].inputNodes[graph[i].inputCount];
        }
        else
        {
            graph[i].inputNodes = NULL;
            //graph[i].outputNodes = NULL;
        }
    } // we need to allocate all buffers first

    // ##### filling inputNodes array (array with indicies of all input nodes of a task)
    for (uint8_t task = 0; task < config->amountTasks; ++task)
    {
        uint8_t index = config->amountTasks - task - 1; //reverse array
        uint8_t inputIndex = 0;
        // ##### loop over all dependencies and check if destination (odd index) == task --> origin (index before this destination) is input Node
        for (uint16_t i = 0; i < config->dependenciesLength; ++i)
        {

            //Dependencies in form of {0,1, 0,2, 1,3, 2,3} if 0->1 0->2 1->3 2->3
            /*
                if the first of a tuple is the current task, 
                (i.e. for task 2 a tuple is found where 2 is at the front)
                the output node located at the even index after the first is added to the array
            */
            if (config->dependencies[2 * i] == task)
            {
                //graph[index].outputNodes[outputIndex] = config->dependencies[2 * i + 1];
                //outputIndex++;
            }
            if (config->dependencies[2 * i + 1] == task)
            {
                graph[index].inputNodes[inputIndex] = config->dependencies[2 * i];
                inputIndex++;
            }
        }
    }

    return taskgraph;
}

//Don't call this method unless you are in a unit test that doesn't perform a full graph
//In nen test damit
void Task_freeTasks(TaskGraph *graph)
{
    for (uint8_t i = 0; i < graph->amountTasks; ++i)
    {
        if (graph->tasks[i].inputCount > 0)
        {
            deallocator(graph->tasks[i].inputNodes);
            graph->tasks[i].inputNodes = NULL;
        }
    }
}

void Task_freeTaskGraph(TaskGraph *graph)
{
    //We need to free the last node too
    //It only has inputNodes
    if (graph->tasks[0].result != NULL)
    {
        deallocator(graph->tasks[0].result);
        graph->tasks[0].result = NULL;
    }
    if (graph->tasks[0].inputNodes != NULL)
    {
        deallocator(graph->tasks[0].inputNodes);
        graph->tasks[0].inputNodes = NULL;
    }
    deallocator(graph->tasks);
    graph->tasks = NULL;
    deallocator(graph);
    graph = NULL;
}

/**
 * We start with Task 0 and look how many consecutive tasks starting with Task 0 can be freed
 * We realloc to a smaller buffer and count how many consecutive tasks we found, to not visit them 
 * again in a later pass.
 * */
void Task_realloc(TaskGraph *graph)
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
    // ##### CHANGED #####
    //while (MM_taskIsFreed(graph, graph->freedTasks))
    while (Task_Freed_taskIsFreed(graph, graph->freedTasks))
    {
        if (graph->freedTasks == graph->amountTasks)
        {
            break;
        }
        graph->freedTasks++;
    }
    void *new_tasks = reallocate(graph->tasks, sizeof(Task) * (graph->amountTasks - graph->freedTasks + 1));
    if (new_tasks != NULL)
    {
        graph->tasks = new_tasks;
    }
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif
}

/**
 * Write all addresses of the results of input tasks successively into the IMA
 * If our garbage collection is done through reference counting, we need to decrease
 * the counter in this step.
 * */
static void Task_prepareInput(Task *newTask, TaskGraph *graph, uint16_t *offset)
{
    uint8_t inputCount = newTask->inputCount;
    for (uint8_t i = 0; i < inputCount; ++i)
    {
        Task *inputNode = TaskDefinition_getTask(graph, newTask->inputNodes[i]);
#if GARBAGE_COLLECTION_TYPE == REFERENCE_COUNTING
        inputNode->counter--;
#endif
        IM_writeValue(&inputNode->result, offset);
    }
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif

}

/* ##### allocate buffer of calculated result size on shared memory & save returned address in task
 *  write address of result into IMA*/
static void Task_prepareOutput(Task *newTask, uint16_t offset, uint16_t (*calculateResultSize)(void))
{
    uint16_t resultSize = calculateResultSize();
    newTask->result = allocator(resultSize);
    IM_writeValue(&newTask->result, &offset);
#ifdef UART_DEBUG
    char buffer[20];
    snprintf(buffer, 20, "Buffer: %p\r\n", newTask->result);
    // ##### CHANGED #####
    // uart_printstring(buffer);
    uart_WriteString(buffer)
    printStackPointer(__FUNCTION_NAME__, SP);
#endif

}

// ##### offset = where to write several inputs, after that, following free space for calculate result size
// ##### result size possible dependent on input
void Task_prepareTask(TaskGraph *graph, uint16_t (*calculateResultSize)(void))
{
    Task *newTask = TaskDefinition_getTask(graph, graph->currentTask);
    uint16_t offset = 0; //Where to store following result

    Task_prepareInput(newTask, graph, &offset);
    Task_prepareOutput(newTask, offset, calculateResultSize);
    graph->currentTask++;
}