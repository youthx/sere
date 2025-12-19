#include "Sere_logger.h"
#include "Sere_asserts.h"
#include "Sere_string.h"

#include "platform/Sere_filesystem.h"
#include "platform/Sere_platform.h"


#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


static b8 is_initialized = SERE_FALSE;
static Sere_FileHandle *log_file_handle = 0;

// -------------------------------------------------------------------------------------------------
// Initialization & shutdown
// -------------------------------------------------------------------------------------------------

b8 Sere_InitLogging(void) {
  if (is_initialized) {
    return SERE_TRUE; // already initialized
  }

  log_file_handle =
      Sere_OpenFile("console.log", SERE_FILEMODE_WRITE, SERE_FALSE);
  if (!log_file_handle || !log_file_handle->is_valid) {
    Sere_PlatformConsoleWriteError("Failed to open console.log for writing.",
                                   SERE_LOG_LEVEL_ERROR);
    return SERE_FALSE;
  }

  is_initialized = SERE_TRUE;
  return SERE_TRUE;
}

SERE b8 Sere_LoggingInitialized(void) { return is_initialized; }

void Sere_ShutdownLogging(void) {
  if (!is_initialized)
    return;

  if (log_file_handle) {
    Sere_CloseFile(log_file_handle);
    log_file_handle = 0;
  }

  is_initialized = SERE_FALSE;
}
#include <time.h>

// Helper to get current date/time string
static void Sere_GetDate(char *buffer, u64 maxLen) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, maxLen, "[%Y-%m-%d %H:%M:%S]", t);
}
// -------------------------------------------------------------------------------------------------
// File append helper
// -------------------------------------------------------------------------------------------------

static void Sere_AppendToLogFile(const char *message) {
  if (!log_file_handle || !log_file_handle->is_valid)
    return;

  u64 length = Sere_StrLen(message);
  u64 written = 0;
  if (!Sere_FileWrite(log_file_handle, length, message, &written) ||
      written != length) {
    Sere_PlatformConsoleWriteError("ERROR writing to console.log",
                                   SERE_LOG_LEVEL_ERROR);
  }
}

// -------------------------------------------------------------------------------------------------
// Log output
// -------------------------------------------------------------------------------------------------

void Sere_LogOutput(Sere_LogLevel level, const char *message, ...) {
  static const char *level_string[] = {"[FATAL]: ", "[ERROR]: ", "[WARNING]: ",
                                       "[INFO]: ",  "[DEBUG]: ", "[TRACE]: "};

  const b8 is_error = (level <= SERE_LOG_LEVEL_ERROR);
  const i32 msg_length = 32000;

  char formatted[msg_length];
  memset(formatted, 0, sizeof(formatted));

  va_list args;
  va_start(args, message);
  vsnprintf(formatted, msg_length, message, args);
  va_end(args);

  // Console message (no date)
  char console_msg[msg_length];
  snprintf(console_msg, sizeof(console_msg), "%s%s\n", level_string[level],
           formatted);

  if (is_error) {
    Sere_PlatformConsoleWriteError(console_msg, level);
  } else {
    Sere_PlatformConsoleWrite(console_msg, level);
  }

  // File message (with date)
  char date[64];
  Sere_GetDate(date, sizeof(date));

  char file_msg[msg_length];
  snprintf(file_msg, sizeof(file_msg), "%s %s%s\n", date, level_string[level],
           formatted);

  Sere_AppendToLogFile(file_msg);
}

// -------------------------------------------------------------------------------------------------
// Assertion failure reporting
// -------------------------------------------------------------------------------------------------

void Sere_ReportAssertionFailure(const char *expr, const char *message,
                                 const char *file, i32 line) {
  Sere_LogOutput(SERE_LOG_LEVEL_FATAL,
                 "Assertion Failure: %s, message: '%s', in file %s, line: %d",
                 expr ? expr : "(null)", message ? message : "(null)",
                 file ? file : "(unknown)", line);
}
