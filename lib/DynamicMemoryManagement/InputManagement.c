#include "lib/DynamicMemoryManagement/InputManagement.h"

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