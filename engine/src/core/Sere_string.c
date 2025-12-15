
#include "Sere_string.h"
#include "Sere_memory.h"

#include <string.h>

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
