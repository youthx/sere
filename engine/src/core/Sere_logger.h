#pragma once

#include "Sere_defines.h"

#define SERE_LOG_WARN_ENABLED 1
#define SERE_LOG_INFO_ENABLED 1
#define SERE_LOG_DEBUG_ENABLED 1
#define SERE_LOG_TRACE_ENABLED 1

#if KRELEASE == 1
#define SERE_LOG_DEBUG_ENABLED 0
#define SERE_LOG_TRACE_ENABLED 0
#endif

typedef enum Sere_LogLevel
{
    SERE_LOG_LEVEL_FATAL = 0,
    SERE_LOG_LEVEL_ERROR = 1,
    SERE_LOG_LEVEL_WARN = 2,
    SERE_LOG_LEVEL_INFO = 3,
    SERE_LOG_LEVEL_DEBUG = 4,
    SERE_LOG_LEVEL_TRACE = 5,
} Sere_LogLevel;

SERE b8 Sere_InitLogging();
SERE b8 Sere_LoggingInitialized();
SERE void Sere_ShutdownLogging();
SERE void Sere_LogOutput(Sere_LogLevel level, const char *message, ...);

#ifdef SERE_SILENT

// ─────────────────────────────────────────────
// Silent mode: all macros expand to nothing
// ─────────────────────────────────────────────
#define SERE_FATAL(message, ...)
#define SERE_ERROR(message, ...)
#define SERE_WARN(message, ...)
#define SERE_INFO(message, ...)
#define SERE_DEBUG(message, ...)
#define SERE_TRACE(message, ...)

#else

// ─────────────────────────────────────────────
// Normal logging macros
// ─────────────────────────────────────────────
#define SERE_FATAL(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#ifndef SERE_ERROR
#define SERE_ERROR(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if SERE_LOG_WARN_ENABLED == 1
#define SERE_WARN(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
#define SERE_WARN(message, ...)
#endif

#if SERE_LOG_INFO_ENABLED == 1
#define SERE_INFO(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
#define SERE_INFO(message, ...)
#endif

#if defined(_DEBUG) && (SERE_LOG_DEBUG_ENABLED == 1)
// In debug builds, and if debug logging is enabled
#define SERE_DEBUG(message, ...) \
    Sere_LogOutput(SERE_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
// Otherwise, macro expands to nothing
#define SERE_DEBUG(message, ...)
#endif

#if SERE_LOG_TRACE_ENABLED == 1
#define SERE_TRACE(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
#define SERE_TRACE(message, ...)
#endif

#endif // SERE_SILENT
