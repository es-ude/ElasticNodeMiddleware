#include "unity.h"
// ##### CHANGED #####
//#include "InputManagement.h"
#include "lib/DynamicMemoryManagement/InputManagement.h"
#include <stdlib.h>

static uint16_t allocationCount = 0;

void* safe_allocation(size_t size){
    void* ptr = malloc(size);
    allocationCount++;
    return ptr;
}

/**
 * We need to test whether the pointer is not NULL, because free(0) is a noop
 * We should decrease allocationcount only if something actually was freed
*/
void safe_free(void *ptr)
{
    if(ptr != NULL){
        --allocationCount;
        free(ptr);
    }
}

void* IMA;

void setUp(void)
{
    IMA = safe_allocation(256);
}

void tearDown(void)
{
    safe_free(IMA);
    TEST_ASSERT_EQUAL(0, allocationCount);
}

void test_writeInt(){
    int x = 5;
    uint16_t offset = 0;
    IM_writeValue(&x,&offset);
    int *ptr = (int *)IMA;
    TEST_ASSERT_EQUAL_INT(5, *ptr);
}

void test_writeByte(){
    uint8_t x = 5;
    uint16_t offset = 0;
    IM_writeValue(&x, &offset);
    uint8_t *ptr = (uint8_t *)IMA;
    TEST_ASSERT_EQUAL_INT(5, *ptr);
}

void test_writeStructSingleValue(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;
    T x = {2,2.5F,4};
    T *xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    T *ptr = *(T **)IMA;
    TEST_ASSERT_EQUAL_PTR(&x, ptr);
    TEST_ASSERT_EQUAL_INT(2, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(2.5F, ptr->y);
    TEST_ASSERT_EQUAL_INT(4, ptr->z);
}

void test_overwriteSingleValue(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;
    T x = {2,2.5,4};
    T *xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    T y = {3,3.5,5};
    T *yPtr = &y;
    offset = 0;
    IM_writeValue(&yPtr, &offset);
    T *ptr = *(T **)IMA;
    TEST_ASSERT_EQUAL_INT(3, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(3.5, ptr->y);
    TEST_ASSERT_EQUAL_INT(5, ptr->z);
}


void test_writeOneMultiInt(){
    int x = 5;
    uint16_t offset = 0;
    IM_writeValue(&x, &offset);
    int *ptr = (int *)IMA;
    TEST_ASSERT_EQUAL_INT(5, *ptr);
}

void test_writeTwoMultiInt(){
    int x = 5;
    int y = 10;
    int *xPtr = &x;
    int *yPtr = &y;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    TEST_ASSERT_EQUAL_UINT16(8, offset);
    IM_writeValue(&yPtr, &offset);
    TEST_ASSERT_EQUAL_UINT16(16, offset);
    int **ptr = (int **)IMA;
    TEST_ASSERT_EQUAL_INT(5, **ptr);
    ptr++;
    TEST_ASSERT_EQUAL_INT(10, **ptr);
}

void test_writeOneMultiIntReturnValue(){
    int x = 5;
    uint16_t offset = 0;
    IM_writeValue(&x, &offset);
    TEST_ASSERT_EQUAL_INT(sizeof(&x), offset);
}

void test_intoffset(){
    uint8_t *ptr = (uint8_t *)IMA;
    *ptr = 5;
    ptr++;
    *ptr = 4;
    uint8_t *ptrtest = (uint8_t *)IMA+1;
    TEST_ASSERT_EQUAL_UINT8(4, *ptrtest);
}

void test_writeTwoMultiIntReturnValue(){
    uint32_t x = 5;
    uint32_t *xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    IM_writeValue(&xPtr, &offset);
    uint32_t **ptr = (uint32_t **)IMA;
    TEST_ASSERT_EQUAL_UINT32(5, **ptr);
    ptr++;
    x = 3;
    TEST_ASSERT_EQUAL_UINT32(3, **ptr);
}

void test_writeStructMultiValue(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;

    T x = {2,3.5,3};
    T y = {5,5.5,6};
    T *xPtr = &x;
    T *yPtr = &y;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    IM_writeValue(&yPtr, &offset);
    T **ptr = (T **)IMA;
    xPtr = *ptr;
    TEST_ASSERT_EQUAL_INT(2, xPtr->x);
    TEST_ASSERT_EQUAL_FLOAT(3.5, xPtr->y);
    TEST_ASSERT_EQUAL_INT(3, xPtr->z);
    ptr++;
    yPtr = *ptr;
    TEST_ASSERT_EQUAL_INT(5, yPtr->x);
    TEST_ASSERT_EQUAL_FLOAT(5.5, yPtr->y);
    TEST_ASSERT_EQUAL_INT(6, yPtr->z);
}

void test_writeIntReference(){
    int x = 5;
    int* xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    int **ptr = (int **)IMA;
    TEST_ASSERT_EQUAL_INT(5, **ptr);
}

void test_writeByteReference(){
    uint8_t x = 5;
    uint8_t* xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    uint8_t **ptr = (uint8_t **)IMA;
    TEST_ASSERT_EQUAL_UINT8(5, **ptr);
}

void test_writeStructSingleReference(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;
    T x = {2,2.5,4};
    T* xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    T **ptrptr = (T **)IMA;
    T *ptr = *ptrptr;
    TEST_ASSERT_EQUAL_INT(2, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(2.5, ptr->y);
    TEST_ASSERT_EQUAL_INT(4, ptr->z);
}

void test_overwriteSingleReference(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;
    T x = {2,2.5,4};
    T *xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    T y = {3,3.5,5};
    T *yPtr = &y;
    offset = 0;
    IM_writeValue(&yPtr, &offset);
    T **ptrptr = (T **)IMA;
    T *ptr = *ptrptr;
    TEST_ASSERT_EQUAL_INT(3, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(3.5, ptr->y);
    TEST_ASSERT_EQUAL_INT(5, ptr->z);
}

void test_writeOneMultiIntReference(){
    int x = 5;
    int *xPtr = &x;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    int **ptr = (int **)IMA;
    TEST_ASSERT_EQUAL_INT(5, **ptr);
}

void test_writeTwoMultiIntReturnReference(){
    int x = 5;
    uint16_t offset = 0;
    IM_writeValue(&x, &offset);
    IM_writeValue(&x, &offset);
    TEST_ASSERT_EQUAL_INT(16, offset);
}

void test_writeStructMultiReference(){
    typedef struct T{
        int x;
        float y;
        int z;
    } T;

    T x = {2,3.5,3};
    T y = {5,5.5,6};
    T *xPtr = &x;
    T *yPtr = &y;
    uint16_t offset = 0;
    IM_writeValue(&xPtr, &offset);
    IM_writeValue(&yPtr, &offset);
    T **ptrptr = (T **)IMA;
    T *ptr = *ptrptr;
    TEST_ASSERT_EQUAL_INT(2, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(3.5, ptr->y);
    TEST_ASSERT_EQUAL_INT(3, ptr->z);
    ptrptr++;
    ptr = *ptrptr;
    TEST_ASSERT_EQUAL_INT(5, ptr->x);
    TEST_ASSERT_EQUAL_FLOAT(5.5, ptr->y);
    TEST_ASSERT_EQUAL_INT(6, ptr->z);
}