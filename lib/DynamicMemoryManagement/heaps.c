#include <stdint.h>

#include "lib/DynamicMemoryManagement/heaps.h"
#include "malloc_mw.h"
// ##### CHANGED #####
//#include "xmem.h"
#include "lib/xmem/xmem.h"

struct heaplist heaps[HEAP_COUNT];

/*
 * Initialises the heaps into the heaps[] array. Each
 * heap requires its own index, defined in heaps.h.
 */
void heaps_init(uint16_t IMA_Size) {
    struct __freelist *flp_temp = 0;
    struct heaplist empty_heaplist = {0,0,0,0};

    for (int i = 0; i <= HEAP_COUNT - 1; i++) {
        heaps[i] = empty_heaplist;
    }

    struct heaplist hp1 = {flp_temp, &__heap_start, 0, 0};
    heaps[INTERNAL_HEAP_INDEX] = hp1;

    struct heaplist hp2 = {flp_temp, (char *)XMEM_OFFSET + IMA_Size, (char *)0xFFFF, (char *)0};
    heaps[EXTERNAL_HEAP_INDEX] = hp2;

    switch_heap(INTERNAL_HEAP_INDEX); // Choose Internal Heap by default
}

void switch_heap(uint8_t heap_index) {
    static uint8_t previous_heap_index;
    static int initialised_flag = 0;

    if (initialised_flag == 1) { // __current_heap will not be initialised on first run
        heaps[previous_heap_index].brkval = __brkval_mw;
        heaps[previous_heap_index].flp 	= __flp_mw;
    }

    initialised_flag = 1;

    // Update all tuneables to new values
    __flp_mw               = heaps[heap_index].flp;
    __malloc_heap_start_mw = heaps[heap_index].malloc_heap_start;
    __malloc_heap_end_mw   = heaps[heap_index].malloc_heap_end;
    __brkval_mw            = heaps[heap_index].brkval;

    previous_heap_index = heap_index;
}
