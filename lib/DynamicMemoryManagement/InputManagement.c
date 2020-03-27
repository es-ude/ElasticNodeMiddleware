#include "lib/DynamicMemoryManagement/InputManagement.h"

// ##### CHANGED ##### from .h in .c
#include "lib/DynamicMemoryManagement/stack_depth.h"

/* ##### wirte address into IMA (number of address = number of input tasks)
    deep copy of address from source to destination (destination = IMA) --> deep copy, therefore using void pointer
    If the system is 16-bit/32-bit/64-bit, size of void pointer is 2/4/8 bytes.*/
void IM_writeValue(void *input, uint16_t *offset)
{
#ifdef UART_DEBUG
    printStackPointer(__FUNCTION_NAME__, SP);
#endif

    uint8_t *source = (uint8_t *)input;
    uint8_t *destination = (uint8_t *)(IMA + (*offset));
    uint8_t counter = sizeof(void *);
    while (counter--)
    {
        *destination++ = *source++;
    }
    *offset += sizeof(void *);

}