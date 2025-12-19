#include "Sere_string.h"
#include "Sere_memory.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// -------------------------------------------------------------------------------------------------
// Basic string utilities
// -------------------------------------------------------------------------------------------------

SERE u64 Sere_StrLen(const char *str)
{
    return strlen(str);
}

SERE char *Sere_StrDup(const char *str)
{
    u64 length = Sere_StrLen(str);
    char *copy = Sere_Alloc(length + 1, SERE_MEMORY_TAG_STRING);
    Sere_CopyMemory(copy, str, length + 1);
    return copy;
}

SERE b8 Sere_StrCompare(const char *s0, const char *s1)
{
    return strcmp(s0, s1) == 0;
}

// -------------------------------------------------------------------------------------------------
// Extended helpers
// -------------------------------------------------------------------------------------------------

// Case-insensitive compare
SERE b8 Sere_StrCompareIgnoreCase(const char *s0, const char *s1)
{
#if defined(_WIN32)
    return _stricmp(s0, s1) == 0;
#else
    return strcasecmp(s0, s1) == 0;
#endif
}

// Concatenate two strings into a new allocation
SERE char *Sere_StrConcat(const char *s0, const char *s1)
{
    u64 len0 = Sere_StrLen(s0);
    u64 len1 = Sere_StrLen(s1);
    char *result = Sere_Alloc(len0 + len1 + 1, SERE_MEMORY_TAG_STRING);
    Sere_CopyMemory(result, s0, len0);
    Sere_CopyMemory(result + len0, s1, len1 + 1); // include null terminator
    return result;
}

// Copy up to max characters safely
SERE void Sere_StrCopy(char *dest, const char *src, u64 max)
{
    if (!dest || !src || max == 0)
        return;
    strncpy(dest, src, max - 1);
    dest[max - 1] = '\0';
}

// Find substring
SERE const char *Sere_StrFind(const char *haystack, const char *needle)
{
    return strstr(haystack, needle);
}

// Check if string starts with prefix
SERE b8 Sere_StrStartsWith(const char *str, const char *prefix)
{
    u64 lenStr = Sere_StrLen(str);
    u64 lenPre = Sere_StrLen(prefix);
    if (lenPre > lenStr)
        return FALSE;
    return strncmp(str, prefix, lenPre) == 0;
}

// Check if string ends with suffix
SERE b8 Sere_StrEndsWith(const char *str, const char *suffix)
{
    u64 lenStr = Sere_StrLen(str);
    u64 lenSuf = Sere_StrLen(suffix);
    if (lenSuf > lenStr)
        return FALSE;
    return strncmp(str + (lenStr - lenSuf), suffix, lenSuf) == 0;
}

// Trim whitespace (in place)
SERE char *Sere_StrTrim(char *str)
{
    if (!str)
        return NULL;
    // leading
    while (*str && isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return str;
    // trailing
    char *end = str + Sere_StrLen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    *(end + 1) = '\0';
    return str;
}

SERE void Sere_StrFormat(char *outStr, u64 maxLen, const char *fmt, ...)
{
    if (!outStr || !fmt || maxLen == 0) return;

    va_list args;
    va_start(args, fmt);
    vsnprintf(outStr, (size_t)maxLen, fmt, args);
    va_end(args);

    // Ensure null termination
    outStr[maxLen - 1] = '\0';
}
