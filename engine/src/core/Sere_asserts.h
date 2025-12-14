
#pragma once

#include "Sere_defines.h"

#define SERE_ASSERTIONS_ENABLED

#ifdef SERE_ASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

SERE void Sere_ReportAssertionFailure(const char *expr, const char *message, const char *file, i32 line);

#define SERE_ASSERT(expr)                                               \
    {                                                                   \
        if (expr)                                                       \
        {                                                               \
        }                                                               \
        else                                                            \
        {                                                               \
            Sere_ReportAssertionFailure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                               \
        }                                                               \
    }

#define SERE_ASSERT_MSG(expr, message)                                       \
    {                                                                        \
        if (expr)                                                            \
        {                                                                    \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Sere_ReportAssertionFailure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                    \
        }                                                                    \
    }

#ifdef _DEBUG
#define SERE_ASSERT_DEBUG(expr)                                              \
    {                                                                        \
        if (expr)                                                            \
        {                                                                    \
        }                                                                    \
        else                                                                 \
        {                                                                    \
            Sere_ReportAssertionFailure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                    \
        }                                                                    \
    }
#else
#define SERE_ASSERT_DEBUG(expr)
#endif 

#else
#define SERE_ASSERT(expr)
#define SERE_ASSERT_MSG(expr, message)
#define SERE_ASSERT_DEBUG(expr)
#endif 

