#ifndef HEAPS_H
#define HEAPS_H

#include <stdint.h>

/*
 * struct heaplist maintains information about a single
 * heap. It has been given no typedef in order to maintain
 * consistency with the original malloc implementation,
 * which never gave its structs typedefs.
 */

/*
 * ##### modify malloc_heap_start on shared memory to start at bigger value --> free space for IMA
 */
struct heaplist {
    struct __freelist *flp;
    char *malloc_heap_start;
    char *malloc_heap_end;
    char *brkval;
};

#define INTERNAL_HEAP_INDEX 0
#define EXTERNAL_HEAP_INDEX 1
#define HEAP_COUNT 2

extern struct heaplist heaps[HEAP_COUNT];

void heaps_init(uint16_t IMA_Size);

void switch_heap(uint8_t heap_index);

#endif 