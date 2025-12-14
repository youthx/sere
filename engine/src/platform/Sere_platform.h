#pragma once

#include "Sere_defines.h"

typedef struct Sere_PlatformState
{
    void *internal_state;
} Sere_PlatformState;

SERE b8 Sere_PlatformStartup(
    Sere_PlatformState *state,
    const char *title,
    i32 x,
    i32 y,
    i32 width,
    i32 height);

SERE void Sere_PlatformShutdown(Sere_PlatformState* state);

SERE b8 Sere_PlatformPumpMessages(Sere_PlatformState* state);

SERE void* Sere_PlatformAlloc(u64 size, b8 aligned);
SERE void Sere_PlatformFree(void* block, b8 aligned);

SERE void* Sere_PlatformZeroMemory(void* block, u64 size);
SERE void* Sere_PlatformCopyMemory(void* dest, const void* source, u64 size);
SERE void* Sere_PlatformSetMemory(void* dest, i32 value, u64 size);

SERE void Sere_PlatformConsoleWrite(const char* message, u8 color);
SERE void Sere_PlatformConsoleWriteError(const char* message, u8 color);

SERE f64 Sere_PlatformGetAbsoluteTime();

SERE void Sere_PlatformSleep(u64 ms);

