
#include "Sere_math.h"
#include "platform/Sere_platform.h"

#include <math.h>
#include <stdlib.h>

static b8 rand_seeded = SERE_FALSE;

f32 Sere_sin(f32 x)
{
    return sinf(x);
}

f32 Sere_cos(f32 x)
{
    return cosf(x);
}

f32 Sere_tan(f32 x)
{
    return tanf(x);
}

f32 Sere_acos(f32 x)
{
    return acosf(x);
}

f32 Sere_sqrt(f32 x)
{
    return sqrtf(x);
}

f32 Sere_abs(f32 x)
{
    return fabsf(x);
}

i32 Sere_Random()
{
    if (!rand_seeded)
    {
        srand((u32)Sere_PlatformGetAbsoluteTime());
        rand_seeded = SERE_TRUE;
    }

    return rand();
}

i32 Sere_RandomRange(i32 min, i32 max)
{
    if (!rand_seeded)
    {
        srand((u32)Sere_PlatformGetAbsoluteTime());
        rand_seeded = SERE_TRUE;
    }

    return (rand() % (max - min + 1)) + min;
}

SERE f32 Sere_RandomF(void)
{
    return (float)Sere_Random() / (f32)RAND_MAX;
}
SERE f32 Sere_RandomFRange(f32 min, f32 max)
{
    return min + ((float)Sere_Random() / ((f32)RAND_MAX / (max - min)));
}
