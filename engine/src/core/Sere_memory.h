
#pragma once

#include "Sere_defines.h"

typedef enum Sere_MemoryTag
{
    SERE_MEMORY_TAG_UNKNOWN,
    SERE_MEMORY_TAG_STATIC_ARRAY,
    SERE_MEMORY_TAG_DYNAMIC_ARRAY,
    SERE_MEMORY_TAG_DICT,
    SERE_MEMORY_TAG_RING_QUEUE,
    SERE_MEMORY_TAG_BST,
    SERE_MEMORY_TAG_STRING,
    SERE_MEMORY_TAG_APPLICATION,
    SERE_MEMORY_TAG_PLATFORM,
    SERE_MEMORY_TAG_JOB,
    SERE_MEMORY_TAG_TEXTURE,
    SERE_MEMORY_TAG_MATERIAL_INSTANCE,
    SERE_MEMORY_TAG_RENDERER,
    SERE_MEMORY_TAG_SYSTEM,
    SERE_MEMORY_TAG_TRANSFORM,
    SERE_MEMORY_TAG_ENTITY,
    SERE_MEMORY_TAG_ENTITY_NODE,
    SERE_MEMORY_TAG_SCENE,

    SERE_MEMORY_TAG_MAX_TAGS
} Sere_MemoryTag;

SERE void Sere_InitMemory(); 
SERE void Sere_ShutdownMemory();

SERE void *Sere_Alloc(u64 size, Sere_MemoryTag tag);
SERE void Sere_Free(void *block, u64 size, Sere_MemoryTag tag);

SERE void *Sere_ZeroMemory(void *block, u64 size);
SERE void *Sere_CopyMemory(void *dest, const void *source, u64 size);
SERE void *Sere_SetMemory(void *dest, i32 value, u64 size);

SERE char* Sere_GetMemoryUsageString();
