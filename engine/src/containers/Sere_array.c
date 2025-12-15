
#include "containers/Sere_array.h"

#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

void *_Sere_CreateArray(u64 length, u64 stride)
{
    u64 header_size = SERE_ARRAY_FIELD_LENGTH * sizeof(u64);
    u64 array_size = length * stride;
    u64 *new_array = Sere_Alloc(header_size + array_size, SERE_MEMORY_TAG_DYNAMIC_ARRAY);
    Sere_SetMemory(new_array, 0, header_size + array_size);

    new_array[SERE_ARRAY_CAPACITY] = length;
    new_array[SERE_ARRAY_LENGTH] = 0;
    new_array[SERE_ARRAY_STRIDE] = stride;
    return (void *)(new_array + SERE_ARRAY_FIELD_LENGTH);
}

void _Sere_DestroyArray(void *array)
{
    u64 *header = (u64 *)array - SERE_ARRAY_FIELD_LENGTH;
    u64 header_size = SERE_ARRAY_FIELD_LENGTH * sizeof(u64);
    u64 total_size = header_size + header[SERE_ARRAY_CAPACITY] * header[SERE_ARRAY_STRIDE];
    Sere_Free(header, total_size, SERE_MEMORY_TAG_DYNAMIC_ARRAY);
}

u64 _Sere_ArrayFieldGet(void *array, u64 field)
{
    u64 *header = (u64 *)array - SERE_ARRAY_FIELD_LENGTH;
    return header[field];
}

void _Sere_ArrayFieldSet(void *array, u64 field, u64 value)
{
    u64 *header = (u64 *)array - SERE_ARRAY_FIELD_LENGTH;
    header[field] = value;
}

void *_Sere_ArrayResize(void *array)
{
    u64 len = Sere_ArrayLength(array);
    u64 stride = Sere_ArrayStride(array);

    void *temp = _Sere_CreateArray(
        (SERE_DARRAY_RESIZE_FACTOR * Sere_ArrayCapacity(array)),
        stride);
    Sere_CopyMemory(temp, array, len * stride);

    _Sere_ArrayFieldSet(temp, SERE_ARRAY_LENGTH, len);
    _Sere_DestroyArray(array);
    return temp;
}

void *_Sere_ArrayPush(void *array, const void *value_ptr)
{
    u64 len = Sere_ArrayLength(array);
    u64 stride = Sere_ArrayStride(array);
    if (len >= Sere_ArrayCapacity(array))
    {
        array = _Sere_ArrayResize(array);
    }

    u64 addr = (u64)array;
    addr += (len * stride);
    Sere_CopyMemory((void *)addr, value_ptr, stride);
    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, len + 1);
    return array;
}

void _Sere_ArrayPop(void *array, void *dest)
{
    u64 len = Sere_ArrayLength(array);
    u64 stride = Sere_ArrayStride(array);

    u64 addr = (u64)array;
    addr += ((len - 1) * stride);
    Sere_CopyMemory(dest, (void *)addr, stride);
    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, len - 1);
}

void *_Sere_ArrayPopAt(void *array, u64 index, void *dest)
{
    u64 len = Sere_ArrayLength(array);
    u64 stride = Sere_ArrayStride(array);
    if (index >= len)
    {
        SERE_ERROR("Index outside of bounds of this array. Length: %i, index: %i", len, index);
        return array;
    }

    u64 addr = (u64)array;
    Sere_CopyMemory(dest, (void *)(addr + (index * stride)), stride);

    if (index != len - 1)
    {
        Sere_CopyMemory(
            (void *)(addr + (index * stride)),
            (void *)(addr + ((index + 1) * stride)),
            stride * (len - index));
    }

    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, len - 1);
    return array;
}

void *_Sere_ArrayInsertAt(void *array, u64 index, void *value_ptr)
{
    u64 len = Sere_ArrayLength(array);
    u64 stride = Sere_ArrayStride(array);
    if (index >= len)
    {
        SERE_ERROR("Index outside of bounds of this array. Length: %i, index: %i", len, index);
        return array;
    }

    if (len >= Sere_ArrayCapacity(array))
    {
        array = _Sere_ArrayResize(array);
    }

    u64 addr = (u64)array;

    if (index != len - 1)
    {
        Sere_CopyMemory(
            (void *)(addr + ((index + 1) * stride)),
            (void *)(addr + (index * stride)),
            stride * (len - index));
    }

    Sere_CopyMemory((void *)(addr + (index * stride)), value_ptr, stride);

    _Sere_ArrayFieldSet(array, SERE_ARRAY_LENGTH, len + 1);
    return array;
}
