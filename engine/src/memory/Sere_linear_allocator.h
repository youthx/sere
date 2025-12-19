
#pragma once

#include "Sere_defines.h"

typedef struct Sere_LinearAllocator
{
    u64 total_size;
    u64 allocated;

    void *memory;
    b8 owns_memory;
} Sere_LinearAllocator;

SERE Sere_LinearAllocator *Sere_CreateLinearAllocator(u64 total_size, void *memory);
SERE void Sere_DestroyLinearAllocator(Sere_LinearAllocator *allocator);

SERE void *Sere_LinearAllocatorAllocate(Sere_LinearAllocator *allocator, u64 size);
SERE void Sere_LinearAllocatorFreeAll(Sere_LinearAllocator *allocator);

