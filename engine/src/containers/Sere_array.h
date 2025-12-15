
#pragma once

#include "Sere_defines.h"

enum
{
    SERE_ARRAY_CAPACITY,
    SERE_ARRAY_LENGTH,
    SERE_ARRAY_STRIDE,
    SERE_ARRAY_FIELD_LENGTH,
};

SERE void *_Sere_CreateArray(u64 length, u64 stride);
SERE void _Sere_DestroyArray(void *array);

SERE u64 _Sere_ArrayFieldGet(void *array, u64 field);
SERE void _Sere_ArrayFieldSet(void *array, u64 field, u64 value);

SERE void *_Sere_ArrayResize(void *array);

SERE void *_Sere_ArrayPush(void *array, const void *value_ptr);
SERE void _Sere_ArrayPop(void *array, void *dest);

SERE void *_Sere_ArrayPopAt(void *array, u64 index, void *dest);
SERE void *_Sere_ArrayInsertAt(void *array, u64 index, void *value_ptr);

#define SERE_DARRAY_DEFAULT_CAPACITY 1
#define SERE_DARRAY_RESIZE_FACTOR 2

#define Sere_CreateArray(type) \
    _Sere_CreateArray(SERE_DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define Sere_ReserveArray(type, capacity) \
    _Sere_CreateArray(capacity, sizeof(type))

#define Sere_DestroyArray(array) _Sere_DestroyArray(array)

#define Sere_ArrayPush(array, value)           \
    {                                          \
        typeof(value) temp = value;            \
        array = _Sere_ArrayPush(array, &temp); \
    }

#define Sere_ArrayPop(array, value_ptr) \
    _Sere_ArrayPop(array, value_ptr)

#define Sere_ArrayInsertAt(array, index, value)           \
    {                                                     \
        typeof(value) temp = value;                       \
        array = _Sere_ArrayInsertAt(array, index, &temp); \
    }

#define Sere_ArrayPopAt(array, index, value_ptr) \
    _Sere_ArrayPopAt(array, index, value_ptr)

#define Sere_ArrayClear(array) \
    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, 0)

#define Sere_ArrayCapacity(array) \ 
    _Sere_ArrayFieldGet(array, SERE_ARRAY_CAPACITY)

#define Sere_ArrayLength(array) \
    _Sere_ArrayFieldGet(array, SERE_ARRAY_LENGTH)

#define Sere_ArrayStride(array) \
    _Sere_ArrayFieldGet(array, SERE_ARRAY_STRIDE)

#define Sere_ArrayLengthSet(array, value) \
    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, value)
    