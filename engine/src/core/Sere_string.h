#pragma once

#include "Sere_defines.h"

// -------------------------------------------------------------------------------------------------
// String utilities
// -------------------------------------------------------------------------------------------------

// Length of a C-string
SERE u64 Sere_StrLen(const char *str);

// Duplicate a string (allocates new memory)
SERE char *Sere_StrDup(const char *str);

// Compare two strings (case-sensitive)
SERE b8 Sere_StrCompare(const char *s0, const char *s1);

// Compare two strings (case-insensitive)
SERE b8 Sere_StrCompareIgnoreCase(const char *s0, const char *s1);

// Concatenate two strings into a new allocation
SERE char *Sere_StrConcat(const char *s0, const char *s1);

// Copy up to max characters safely
SERE void Sere_StrCopy(char *dest, const char *src, u64 max);

// Find substring
SERE const char *Sere_StrFind(const char *haystack, const char *needle);

// Check if string starts with prefix
SERE b8 Sere_StrStartsWith(const char *str, const char *prefix);

// Check if string ends with suffix
SERE b8 Sere_StrEndsWith(const char *str, const char *suffix);

// Trim whitespace (in place)
SERE char *Sere_StrTrim(char *str);

// Format a string into outStr using printf-style formatting.
// outStr: destination buffer
// maxLen: maximum number of characters (including null terminator)
// fmt:    format string
// ...:    variable arguments
SERE void Sere_StrFormat(char *outStr, u64 maxLen, const char *fmt, ...);
