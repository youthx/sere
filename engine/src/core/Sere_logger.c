
#include "Sere_logger.h"
#include "Sere_asserts.h"
#include "platform/Sere_platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static b8 is_initialized = SERE_FALSE;

b8 Sere_InitLogging()
{
    // TODO: Create a log file
    is_initialized = SERE_TRUE;
    return SERE_TRUE;
}

SERE b8 Sere_LoggingInitialized()
{
    return is_initialized;
}

void Sere_ShutdownLogging()
{
    // TODO: Cleanup logging/write queued entries
}

void Sere_LogOutput(Sere_LogLevel level, const char *message, ...)
{
    const char *level_string[6] = {
        "[FATAL]: ",
        "[ERROR]: ",
        "[WARNING]: ",
        "[INFO]: ",
        "[DEBUG]: ",
        "[TRACE]: "};

    b8 is_error = level < SERE_LOG_LEVEL_WARN;

    const i32 msg_length = 32000;

    char out_msg[msg_length];
    memset(out_msg, 0, sizeof(out_msg));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_msg, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_msg2[msg_length];
    snprintf(out_msg2, sizeof(out_msg2), "%s%s\n", level_string[level], out_msg);

    if (is_error)
    {
        Sere_PlatformConsoleWriteError(out_msg2, level);
    }
    else
    {
        Sere_PlatformConsoleWrite(out_msg2, level);
    }
}

void Sere_ReportAssertionFailure(const char *expr, const char *message, const char *file, i32 line)
{
    Sere_LogOutput(SERE_LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file %s, line: %d\n", expr, message, file, line);
}
