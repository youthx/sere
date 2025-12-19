
#pragma once

#include "Sere_defines.h"

typedef struct Sere_Vec2
{
    f32 x, y;
} Sere_Vec2;

typedef struct Sere_Vec3
{
    f32 x, y, z;
} Sere_Vec3;

typedef struct Sere_Vec4
{
    f32 x, y, z, w;
} Sere_Vec4;

typedef Sere_Vec4 Sere_Quat;

typedef union Sere_Mat4
{
    f32 data[16];
#if defined(SEREUSE_SIMD)
    Sere_Vec4 rows[4];
#endif
} Sere_Mat4;
