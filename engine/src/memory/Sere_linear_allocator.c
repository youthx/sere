
#include "Sere_linear_allocator.h"

#include "core/Sere_memory.h"
#include "core/Sere_logger.h"

Sere_LinearAllocator *Sere_CreateLinearAllocator(u64 total_size, void *memory)
{
    Sere_LinearAllocator *out_allocator = (Sere_LinearAllocator *)Sere_Alloc(sizeof(Sere_LinearAllocator), SERE_MEMORY_TAG_ALLOCATOR);

    out_allocator->total_size = total_size;
    out_allocator->allocated = 0;
    out_allocator->owns_memory = memory == 0;
    if (memory)
    {
        out_allocator->memory = memory;
    }
    else
    {
        out_allocator->memory = Sere_Alloc(total_size, SERE_MEMORY_TAG_ALLOCATOR);
    }

    return out_allocator;
}

void Sere_DestroyLinearAllocator(Sere_LinearAllocator *allocator)
{
    allocator->allocated = 0;
    if (allocator->owns_memory && allocator->memory)
    {
        Sere_Free(allocator->memory, allocator->total_size, SERE_MEMORY_TAG_ALLOCATOR);
    }

    allocator->total_size = 0;
    allocator->owns_memory = SERE_FALSE;
}

void *Sere_LinearAllocatorAllocate(Sere_LinearAllocator *allocator, u64 size)
{
    if (allocator && allocator->memory)
    {
        if (allocator->allocated + size > allocator->total_size)
        {
            u64 remaining = allocator->total_size - allocator->allocated;
            SERE_ERROR("Sere_LinearAllocatorAllocate - Tried to allocate %lluB, only %lluB remaining.", size, remaining);
            return 0;
        }

        void *block = allocator->memory + allocator->allocated;
        allocator->allocated += size;
        return block;
    }

    SERE_ERROR("Sere_LinearAllocatorAllocate - Unable to access Allocator / Memory.");
    return 0;
}

void Sere_LinearAllocatorFreeAll(Sere_LinearAllocator *allocator)
{
    if (allocator && allocator->memory)
    {
        allocator->allocated = 0;
        Sere_ZeroMemory(allocator->memory, allocator->total_size);
    }
}
