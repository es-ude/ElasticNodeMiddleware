#ifndef TASK_GRAPH_CONFIG
#define TASK_GRAPH_CONFIG

#define AMOUNT_TASKS 22
#define AMOUNT_DEPENDENCIES 28

/* ##### TaskGraph Definitions
    dependency array: ordering important
    even index = origin, odd index = destination */
// ##### for testing the variable must be static #####
uint8_t dependencies[2 * AMOUNT_DEPENDENCIES] = {
        0, 1,
        0, 2,
        1, 3,
        1, 4,
        2, 5,
        2, 6,
        3, 7,
        3, 8,
        4, 9,
        4, 10,
        5, 11,
        5, 12,
        6, 13,
        6, 14,
        7, 15,
        8, 15,
        9, 16,
        10, 16,
        11, 17,
        12, 17,
        13, 18,
        14, 18,
        15, 19,
        16, 19,
        17, 20,
        18, 20,
        19, 21,
        20, 21,
};
/* ##### number = how many input task this task has
    --> allocate for each task an array that is big enough to store indicies of its input tasks */
// ##### for testing the variable must be static #####
uint8_t inputArray[AMOUNT_TASKS] = {
        0,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
};

/* ##### number = how many output task this task has
    --> initalize reference counter for garbage collection (some algorithms) */
// ##### for testing the variable must be static #####
uint8_t outputArray[AMOUNT_TASKS] = {
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        1,
        0,
};
// ##### for testing the variable must be static #####
uint16_t resultSize()
{
    return 100*sizeof(uint8_t);
}
#endif
// Total Size: 166 Bytes
