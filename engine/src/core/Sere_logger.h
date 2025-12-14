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

b8 Sere_InitLogging();
void Sere_ShutdownLogging();

SERE void Sere_LogOutput(Sere_LogLevel level, const char *message, ...);

#define SERE_FATAL(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)

#ifndef SERE_ERROR
// Logs an error-level message
#define SERE_ERROR(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#endif

#if SERE_LOG_WARN_ENABLED == 1
#define SERE_WARN(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
// Does nothing
#define SERE_WARN(message, ...)
#endif

#if SERE_LOG_INFO_ENABLED == 1
#define SERE_INFO(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
// Does nothing
#define SERE_INFO(message, ...)
#endif

#if SERE_LOG_DEBUG_ENABLED == 1
#define SERE_DEBUG(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_DEGUG, message, ##__VA_ARGS__)
#else
// Does nothing
#define SERE_DEBUG(message, ...)
#endif

#if SERE_LOG_TRACE_ENABLED == 1
#define SERE_TRACE(message, ...) Sere_LogOutput(SERE_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
// Does nothing
#define SERE_TRACE(message, ...)
#endif
