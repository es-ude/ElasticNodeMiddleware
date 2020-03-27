#ifndef _INPUTMANAGEMENT_H
#define _INPUTMANAGEMENT_H
#include <stdint.h>
//#include "stack_depth.h" --> in .c
extern void* IMA;

void IM_writeValue(void* input, uint16_t* index);

#endif // _INPUTMANAGEMENT_H
