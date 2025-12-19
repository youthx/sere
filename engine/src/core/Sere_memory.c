

#include "Sere_memory.h"

#include "core/Sere_logger.h"
#include "platform/Sere_platform.h"

#include <string.h>
#include <stdio.h>

typedef struct Sere_MemoryStats
{
    u64 total_allocated;
    u64 tagged_allocations[SERE_MEMORY_TAG_MAX_TAGS];
} Sere_MemoryStats;

static const char *Sere_MemoryTagStrings[SERE_MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN      ",
    "STATIC_ARRAY ",
    "DYNAMIC_ARRAY",
    "ALLOCATOR    ",
    "DICT         ",
    "RING_QUEUE   ",
    "BST          ",
    "STRING       ",
    "APPLICATION  ",
    "PLATFORM     ",
    "JOB          ",
    "TEXTURE      ",
    "MAT_INST     ",
    "RENDERER     ",
    "SYSTEM       ",
    "TRANSFORM    ",
    "ENTITY       ",
    "ENTITY_NODE  ",
    "SCENE        "};

static Sere_MemoryStats stats;
static b8 is_initialized = SERE_FALSE;

SERE void Sere_InitMemory()
{
    Sere_PlatformZeroMemory(&stats, sizeof(stats));
    is_initialized = SERE_TRUE;
}

SERE b8 Sere_MemoryInitialized() {
    return is_initialized;
}


SERE void Sere_ShutdownMemory()
{
}

SERE void *Sere_Alloc(u64 size, Sere_MemoryTag tag)
{
    if (tag == SERE_MEMORY_TAG_UNKNOWN)
    {
        SERE_WARN("Sere_Alloc called using SERE_MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    }

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO: Memory alignment
    void *block = Sere_PlatformAlloc(size, SERE_FALSE);
    Sere_PlatformZeroMemory(block, sizeof(block));

    return block;
}

SERE void Sere_Free(void *block, u64 size, Sere_MemoryTag tag)
{
    if (tag == SERE_MEMORY_TAG_UNKNOWN)
    {
        SERE_WARN("Sere_Free called using SERE_MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    }

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    Sere_PlatformFree(block, SERE_FALSE);
}

SERE void *Sere_ZeroMemory(void *block, u64 size)
{
    return Sere_PlatformZeroMemory(block, size);
}

SERE void *Sere_CopyMemory(void *dest, const void *source, u64 size)
{
    return Sere_PlatformCopyMemory(dest, source, size);
}

SERE void *Sere_SetMemory(void *dest, i32 value, u64 size)
{
    return Sere_PlatformSetMemory(dest, value, size);
}

SERE char *Sere_GetMemoryUsageString()
{
    const u64 gib = 1024ull * 1024ull * 1024ull;
    const u64 mib = 1024ull * 1024ull;
    const u64 kib = 1024ull;

    char buffer[16384]; // bigger buffer
    u64 offset = 0;

    // Header with total
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                       "System memory usage:\n Total allocated: %llu bytes (%.2f MiB)\n\n",
                       (unsigned long long)stats.total_allocated,
                       stats.total_allocated / (float)mib);

    // Per-tag breakdown
    for (u32 i = 0; i < SERE_MEMORY_TAG_MAX_TAGS; ++i)
    {
        u64 bytes = stats.tagged_allocations[i];
        if (bytes == 0)
            continue; // skip unused tags

        const char *unit = "B";
        float amount = (float)bytes;

        if (bytes >= gib)
        {
            unit = "GiB";
            amount = bytes / (float)gib;
        }
        else if (bytes >= mib)
        {
            unit = "MiB";
            amount = bytes / (float)mib;
        }
        else if (bytes >= kib)
        {
            unit = "KiB";
            amount = bytes / (float)kib;
        }

        float percent = stats.total_allocated > 0
                            ? (bytes * 100.0f) / (float)stats.total_allocated
                            : 0.0f;

        offset += snprintf(buffer + offset, sizeof(buffer) - offset,
                           " %-12s : %8.2f %s (%llu bytes, %.1f%%)\n",
                           Sere_MemoryTagStrings[i],
                           amount, unit,
                           (unsigned long long)bytes,
                           percent);
    }

    // Duplicate to heap string
    char *out_string = _strdup(buffer);
    return out_string;
}
