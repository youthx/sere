#pragma once

#include "Sere_defines.h"
#include "Sere_math_types.h"

#include "core/Sere_memory.h"

// -------------------------------------------------------------------------------------------------
// Constants
// -------------------------------------------------------------------------------------------------

#define SERE_PI 3.14159265358979323846f
#define SERE_TWO_PI (2.0f * SERE_PI)
#define SERE_HALF_PI (0.5f * SERE_PI)
#define SERE_QUARTER_PI (0.25f * SERE_PI)
#define SERE_ONE_OVER_PI (1.0f / SERE_PI)
#define SERE_ONE_OVER_TWO_PI (1.0f / SERE_TWO_PI)

#define SERE_SQRT_TWO 1.41421356237309504880f
#define SERE_SQRT_THREE 1.73205080756887729352f
#define SERE_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define SERE_SQRT_ONE_OVER_THREE 0.57735026918962576450f

#define SERE_DEG2RAD_MULTIPLIER (SERE_PI / 180.0f)
#define SERE_RAD2DEG_MULTIPLIER (180.0f / SERE_PI)

#define SERE_SEC_TO_MS_MULTIPLIER 1000.0f
#define SERE_MS_TO_SEC_MULTIPLIER 0.001f

#define SERE_INFINITY 1e30f
#define SERE_FLOAT_EPSILON 1.192092896e-07f

// -------------------------------------------------------------------------------------------------
// General math
// -------------------------------------------------------------------------------------------------

SERE f32 Sere_sin(f32 x);
SERE f32 Sere_cos(f32 x);
SERE f32 Sere_tan(f32 x);
SERE f32 Sere_acos(f32 x);
SERE f32 Sere_sqrt(f32 x);
SERE f32 Sere_abs(f32 x);

SERE_INLINE b8 Sere_IsPowerOf2(u64 value)
{
    return (value != 0) && ((value & (value - 1)) == 0);
}

SERE i32 Sere_Random(void);
SERE i32 Sere_RandomRange(i32 min, i32 max);

SERE f32 Sere_RandomF(void);
SERE f32 Sere_RandomFRange(f32 min, f32 max);

// -------------------------------------------------------------------------------------------------
// Vec2
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_CreateVec2(f32 x, f32 y)
{
    return (Sere_Vec2){x, y};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Zero(void)
{
    return (Sere_Vec2){0.0f, 0.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2One(void)
{
    return (Sere_Vec2){1.0f, 1.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Up(void)
{
    return (Sere_Vec2){0.0f, 1.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Down(void)
{
    return (Sere_Vec2){0.0f, -1.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Left(void)
{
    return (Sere_Vec2){-1.0f, 0.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Right(void)
{
    return (Sere_Vec2){1.0f, 0.0f};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Add(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){a.x + b.x, a.y + b.y};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Sub(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){a.x - b.x, a.y - b.y};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Mul(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){a.x * b.x, a.y * b.y};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Div(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){a.x / b.x, a.y / b.y};
}

SERE_INLINE f32 Sere_Vec2LengthSquared(Sere_Vec2 v)
{
    return v.x * v.x + v.y * v.y;
}

SERE_INLINE f32 Sere_Vec2Length(Sere_Vec2 v)
{
    return Sere_sqrt(Sere_Vec2LengthSquared(v));
}

SERE_INLINE void Sere_Vec2Normalize(Sere_Vec2 *v)
{
    f32 len = Sere_Vec2Length(*v);
    v->x /= len;
    v->y /= len;
}

SERE_INLINE Sere_Vec2 Sere_Vec2Normalized(Sere_Vec2 v)
{
    Sere_Vec2Normalize(&v);
    return v;
}

SERE_INLINE b8 Sere_Vec2Compare(Sere_Vec2 a, Sere_Vec2 b, f32 tolerance)
{
    if (Sere_abs(a.x - b.x) > tolerance)
        return FALSE;
    if (Sere_abs(a.y - b.y) > tolerance)
        return FALSE;
    return TRUE;
}

SERE_INLINE f32 Sere_Vec2Distance(Sere_Vec2 a, Sere_Vec2 b)
{
    return Sere_Vec2Length(Sere_Vec2Sub(a, b));
}

// -------------------------------------------------------------------------------------------------
// Vec3
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec3 Sere_CreateVec3(f32 x, f32 y, f32 z)
{
    return (Sere_Vec3){x, y, z};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Zero(void)
{
    return (Sere_Vec3){0.0f, 0.0f, 0.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3One(void)
{
    return (Sere_Vec3){1.0f, 1.0f, 1.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Up(void)
{
    return (Sere_Vec3){0.0f, 1.0f, 0.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Down(void)
{
    return (Sere_Vec3){0.0f, -1.0f, 0.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Left(void)
{
    return (Sere_Vec3){-1.0f, 0.0f, 0.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Right(void)
{
    return (Sere_Vec3){1.0f, 0.0f, 0.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Forward(void)
{
    return (Sere_Vec3){0.0f, 0.0f, -1.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Back(void)
{
    return (Sere_Vec3){0.0f, 0.0f, 1.0f};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Add(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Sub(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Mul(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

SERE_INLINE Sere_Vec3 Sere_Vec3MulScalar(Sere_Vec3 v, f32 s)
{
    return (Sere_Vec3){v.x * s, v.y * s, v.z * s};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Div(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){a.x / b.x, a.y / b.y, a.z / b.z};
}

SERE_INLINE f32 Sere_Vec3LengthSquared(Sere_Vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

SERE_INLINE f32 Sere_Vec3Length(Sere_Vec3 v)
{
    return Sere_sqrt(Sere_Vec3LengthSquared(v));
}

SERE_INLINE void Sere_Vec3Normalize(Sere_Vec3 *v)
{
    f32 len = Sere_Vec3Length(*v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}

SERE_INLINE Sere_Vec3 Sere_Vec3Normalized(Sere_Vec3 v)
{
    Sere_Vec3Normalize(&v);
    return v;
}

SERE_INLINE Sere_Vec3 Sere_Vec3Cross(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

SERE_INLINE b8 Sere_Vec3Compare(Sere_Vec3 a, Sere_Vec3 b, f32 tolerance)
{
    if (Sere_abs(a.x - b.x) > tolerance)
        return FALSE;
    if (Sere_abs(a.y - b.y) > tolerance)
        return FALSE;
    if (Sere_abs(a.z - b.z) > tolerance)
        return FALSE;
    return TRUE;
}

SERE_INLINE f32 Sere_Vec3Distance(Sere_Vec3 a, Sere_Vec3 b)
{
    return Sere_Vec3Length(Sere_Vec3Sub(a, b));
}

// -------------------------------------------------------------------------------------------------
// Vec4 (scalar path preserved)
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec4 Sere_CreateVec4(f32 x, f32 y, f32 z, f32 w)
{
    return (Sere_Vec4){x, y, z, w};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Zero(void)
{
    return (Sere_Vec4){0.0f, 0.0f, 0.0f, 0.0f};
}

SERE_INLINE Sere_Vec4 Sere_Vec4One(void)
{
    return (Sere_Vec4){1.0f, 1.0f, 1.0f, 1.0f};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Add(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Sub(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Mul(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Div(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

SERE_INLINE f32 Sere_Vec4LengthSquared(Sere_Vec4 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

SERE_INLINE f32 Sere_Vec4Length(Sere_Vec4 v)
{
    return Sere_sqrt(Sere_Vec4LengthSquared(v));
}

SERE_INLINE void Sere_Vec4Normalize(Sere_Vec4 *v)
{
    f32 len = Sere_Vec4Length(*v);
    v->x /= len;
    v->y /= len;
    v->z /= len;
    v->w /= len;
}

SERE_INLINE Sere_Vec4 Sere_Vec4Normalized(Sere_Vec4 v)
{
    Sere_Vec4Normalize(&v);
    return v;
}

SERE_INLINE f32 Sere_Vec4DotF32(
    f32 a0, f32 a1, f32 a2, f32 a3,
    f32 b0, f32 b1, f32 b2, f32 b3)
{
    return a0 * b0 +
           a1 * b1 +
           a2 * b2 +
           a3 * b3;
}

// -------------------------------------------------------------------------------------------------
// Vec4 comparisons & distance
// -------------------------------------------------------------------------------------------------

SERE_INLINE b8 Sere_Vec4Compare(Sere_Vec4 a, Sere_Vec4 b, f32 tolerance)
{
    if (Sere_abs(a.x - b.x) > tolerance)
        return FALSE;
    if (Sere_abs(a.y - b.y) > tolerance)
        return FALSE;
    if (Sere_abs(a.z - b.z) > tolerance)
        return FALSE;
    if (Sere_abs(a.w - b.w) > tolerance)
        return FALSE;
    return TRUE;
}

SERE_INLINE f32 Sere_Vec4Distance(Sere_Vec4 a, Sere_Vec4 b)
{
    return Sere_Vec4Length(Sere_Vec4Sub(a, b));
}

// -------------------------------------------------------------------------------------------------
// Dot helpers
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_Vec2Dot(Sere_Vec2 a, Sere_Vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

SERE_INLINE f32 Sere_Vec3Dot(Sere_Vec3 a, Sere_Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

SERE_INLINE f32 Sere_Vec4Dot(Sere_Vec4 a, Sere_Vec4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// -------------------------------------------------------------------------------------------------
// Distance squared helpers
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_Vec2DistanceSquared(Sere_Vec2 a, Sere_Vec2 b)
{
    Sere_Vec2 d = Sere_Vec2Sub(a, b);
    return Sere_Vec2Dot(d, d);
}

SERE_INLINE f32 Sere_Vec3DistanceSquared(Sere_Vec3 a, Sere_Vec3 b)
{
    Sere_Vec3 d = Sere_Vec3Sub(a, b);
    return Sere_Vec3Dot(d, d);
}

SERE_INLINE f32 Sere_Vec4DistanceSquared(Sere_Vec4 a, Sere_Vec4 b)
{
    Sere_Vec4 d = Sere_Vec4Sub(a, b);
    return Sere_Vec4Dot(d, d);
}

// -------------------------------------------------------------------------------------------------
// Lerp
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Lerp(Sere_Vec2 a, Sere_Vec2 b, f32 t)
{
    return (Sere_Vec2){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Lerp(Sere_Vec3 a, Sere_Vec3 b, f32 t)
{
    return (Sere_Vec3){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Lerp(Sere_Vec4 a, Sere_Vec4 b, f32 t)
{
    return (Sere_Vec4){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t};
}

// -------------------------------------------------------------------------------------------------
// Component-wise min / max
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Min(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Max(Sere_Vec2 a, Sere_Vec2 b)
{
    return (Sere_Vec2){
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Min(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Max(Sere_Vec3 a, Sere_Vec3 b)
{
    return (Sere_Vec3){
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Min(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){
        a.x < b.x ? a.x : b.x,
        a.y < b.y ? a.y : b.y,
        a.z < b.z ? a.z : b.z,
        a.w < b.w ? a.w : b.w};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Max(Sere_Vec4 a, Sere_Vec4 b)
{
    return (Sere_Vec4){
        a.x > b.x ? a.x : b.x,
        a.y > b.y ? a.y : b.y,
        a.z > b.z ? a.z : b.z,
        a.w > b.w ? a.w : b.w};
}

// -------------------------------------------------------------------------------------------------
// Projection
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Project(Sere_Vec2 a, Sere_Vec2 b)
{
    f32 denom = Sere_Vec2Dot(b, b);
    if (denom <= SERE_FLOAT_EPSILON)
        return Sere_Vec2Zero();

    f32 scale = Sere_Vec2Dot(a, b) / denom;
    return Sere_Vec2Mul(b, (Sere_Vec2){scale, scale});
}

SERE_INLINE Sere_Vec3 Sere_Vec3Project(Sere_Vec3 a, Sere_Vec3 b)
{
    f32 denom = Sere_Vec3Dot(b, b);
    if (denom <= SERE_FLOAT_EPSILON)
        return Sere_Vec3Zero();

    f32 scale = Sere_Vec3Dot(a, b) / denom;
    return Sere_Vec3MulScalar(b, scale);
}

SERE_INLINE Sere_Vec4 Sere_Vec4Project(Sere_Vec4 a, Sere_Vec4 b)
{
    f32 denom = Sere_Vec4Dot(b, b);
    if (denom <= SERE_FLOAT_EPSILON)
        return Sere_Vec4Zero();

    f32 scale = Sere_Vec4Dot(a, b) / denom;
    return Sere_Vec4Mul(b, (Sere_Vec4){scale, scale, scale, scale});
}

// -------------------------------------------------------------------------------------------------
// Reflection
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Reflect(Sere_Vec2 v, Sere_Vec2 n)
{
    return Sere_Vec2Sub(
        v,
        Sere_Vec2Mul(
            n,
            (Sere_Vec2){
                2.0f * Sere_Vec2Dot(v, n),
                2.0f * Sere_Vec2Dot(v, n)}));
}

SERE_INLINE Sere_Vec3 Sere_Vec3Reflect(Sere_Vec3 v, Sere_Vec3 n)
{
    return Sere_Vec3Sub(
        v,
        Sere_Vec3MulScalar(n, 2.0f * Sere_Vec3Dot(v, n)));
}

SERE_INLINE Sere_Vec4 Sere_Vec4Reflect(Sere_Vec4 v, Sere_Vec4 n)
{
    f32 d = 2.0f * Sere_Vec4Dot(v, n);
    return Sere_Vec4Sub(
        v,
        (Sere_Vec4){n.x * d, n.y * d, n.z * d, n.w * d});
}

// -------------------------------------------------------------------------------------------------
// Refraction
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// Angle helpers
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_Vec2Angle(Sere_Vec2 a, Sere_Vec2 b)
{
    f32 denom = Sere_Vec2Length(a) * Sere_Vec2Length(b);
    if (denom <= SERE_FLOAT_EPSILON)
        return 0.0f;

    f32 d = Sere_Vec2Dot(a, b) / denom;
    d = SERE_CLAMP(d, -1.0f, 1.0f);
    return Sere_acos(d);
}

SERE_INLINE f32 Sere_Vec3Angle(Sere_Vec3 a, Sere_Vec3 b)
{
    f32 denom = Sere_Vec3Length(a) * Sere_Vec3Length(b);
    if (denom <= SERE_FLOAT_EPSILON)
        return 0.0f;

    f32 d = Sere_Vec3Dot(a, b) / denom;
    d = SERE_CLAMP(d, -1.0f, 1.0f);
    return Sere_acos(d);
}

SERE_INLINE f32 Sere_Vec2SignedAngle(Sere_Vec2 a, Sere_Vec2 b)
{
    f32 angle = Sere_Vec2Angle(a, b);
    f32 cross = a.x * b.y - a.y * b.x;
    return cross < 0.0f ? -angle : angle;
}

// -------------------------------------------------------------------------------------------------
// Clamp & saturate
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Clamp(Sere_Vec2 v, Sere_Vec2 min, Sere_Vec2 max)
{
    return (Sere_Vec2){
        SERE_CLAMP(v.x, min.x, max.x),
        SERE_CLAMP(v.y, min.y, max.y)};
}

SERE_INLINE Sere_Vec3 Sere_Vec3Clamp(Sere_Vec3 v, Sere_Vec3 min, Sere_Vec3 max)
{
    return (Sere_Vec3){
        SERE_CLAMP(v.x, min.x, max.x),
        SERE_CLAMP(v.y, min.y, max.y),
        SERE_CLAMP(v.z, min.z, max.z)};
}

SERE_INLINE Sere_Vec4 Sere_Vec4Clamp(Sere_Vec4 v, Sere_Vec4 min, Sere_Vec4 max)
{
    return (Sere_Vec4){
        SERE_CLAMP(v.x, min.x, max.x),
        SERE_CLAMP(v.y, min.y, max.y),
        SERE_CLAMP(v.z, min.z, max.z),
        SERE_CLAMP(v.w, min.w, max.w)};
}

SERE_INLINE Sere_Vec2 Sere_Vec2Saturate(Sere_Vec2 v)
{
    return Sere_Vec2Clamp(
        v,
        (Sere_Vec2){0.0f, 0.0f},
        (Sere_Vec2){1.0f, 1.0f});
}

SERE_INLINE Sere_Vec3 Sere_Vec3Saturate(Sere_Vec3 v)
{
    return Sere_Vec3Clamp(
        v,
        (Sere_Vec3){0.0f, 0.0f, 0.0f},
        (Sere_Vec3){1.0f, 1.0f, 1.0f});
}

SERE_INLINE Sere_Vec4 Sere_Vec4Saturate(Sere_Vec4 v)
{
    return Sere_Vec4Clamp(
        v,
        (Sere_Vec4){0.0f, 0.0f, 0.0f, 0.0f},
        (Sere_Vec4){1.0f, 1.0f, 1.0f, 1.0f});
}

// -------------------------------------------------------------------------------------------------
// Utility predicates
// -------------------------------------------------------------------------------------------------

SERE_INLINE b8 Sere_Vec2IsZero(Sere_Vec2 v)
{
    return Sere_abs(v.x) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.y) <= SERE_FLOAT_EPSILON;
}

SERE_INLINE b8 Sere_Vec3IsZero(Sere_Vec3 v)
{
    return Sere_abs(v.x) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.y) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.z) <= SERE_FLOAT_EPSILON;
}

SERE_INLINE b8 Sere_Vec4IsZero(Sere_Vec4 v)
{
    return Sere_abs(v.x) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.y) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.z) <= SERE_FLOAT_EPSILON &&
           Sere_abs(v.w) <= SERE_FLOAT_EPSILON;
}

// -------------------------------------------------------------------------------------------------
// Refraction
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec2 Sere_Vec2Refract(Sere_Vec2 v, Sere_Vec2 n, f32 eta)
{
    f32 cos_i = -Sere_Vec2Dot(n, v);
    f32 sin_t2 = eta * eta * (1.0f - cos_i * cos_i);
    if (sin_t2 > 1.0f)
        return Sere_Vec2Zero(); // total internal reflection
    f32 cos_t = Sere_sqrt(1.0f - sin_t2);
    return Sere_Vec2Add(Sere_Vec2Mul(v, (Sere_Vec2){eta, eta}),
                        Sere_Vec2Mul(n, (Sere_Vec2){eta * cos_i - cos_t, eta * cos_i - cos_t}));
}

SERE_INLINE Sere_Vec3 Sere_Vec3Refract(Sere_Vec3 v, Sere_Vec3 n, f32 eta)
{
    f32 cos_i = -Sere_Vec3Dot(n, v);
    f32 sin_t2 = eta * eta * (1.0f - cos_i * cos_i);
    if (sin_t2 > 1.0f)
        return Sere_Vec3Zero();
    f32 cos_t = Sere_sqrt(1.0f - sin_t2);
    return Sere_Vec3Add(Sere_Vec3MulScalar(v, eta),
                        Sere_Vec3MulScalar(n, eta * cos_i - cos_t));
}

SERE_INLINE Sere_Vec4 Sere_Vec4Refract(Sere_Vec4 v, Sere_Vec4 n, f32 eta)
{
    f32 cos_i = -Sere_Vec4Dot(n, v);
    f32 sin_t2 = eta * eta * (1.0f - cos_i * cos_i);
    if (sin_t2 > 1.0f)
        return Sere_Vec4Zero();
    f32 cos_t = Sere_sqrt(1.0f - sin_t2);
    f32 d = eta * cos_i - cos_t;
    return Sere_Vec4Add(Sere_Vec4Mul(v, (Sere_Vec4){eta, eta, eta, eta}),
                        (Sere_Vec4){n.x * d, n.y * d, n.z * d, n.w * d});
}

// -------------------------------------------------------------------------------------------------
// Angle helpers (extended)
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_Deg2Rad(f32 degrees) { return degrees * SERE_DEG2RAD_MULTIPLIER; }
SERE_INLINE f32 Sere_Rad2Deg(f32 radians) { return radians * SERE_RAD2DEG_MULTIPLIER; }

SERE_INLINE f32 Sere_Vec4Angle(Sere_Vec4 a, Sere_Vec4 b)
{
    f32 denom = Sere_Vec4Length(a) * Sere_Vec4Length(b);
    if (denom <= SERE_FLOAT_EPSILON)
        return 0.0f;
    f32 d = Sere_Vec4Dot(a, b) / denom;
    d = SERE_CLAMP(d, -1.0f, 1.0f);
    return Sere_acos(d);
}

// -------------------------------------------------------------------------------------------------
// Mat4 basic constructors
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Identity()
{
    Sere_Mat4 out_matrix;
    Sere_ZeroMemory(out_matrix.data, sizeof(f32) * 16);
    out_matrix.data[0] = 1.0f;
    out_matrix.data[5] = 1.0f;
    out_matrix.data[10] = 1.0f;
    out_matrix.data[15] = 1.0f;
    return out_matrix;
}

SERE_INLINE Sere_Mat4 Sere_Mat4Zero()
{
    Sere_Mat4 out_matrix;
    Sere_ZeroMemory(out_matrix.data, sizeof(f32) * 16);
    return out_matrix;
}

SERE_INLINE Sere_Mat4 Sere_Mat4Diagonal(f32 d)
{
    Sere_Mat4 out_matrix = Sere_Mat4Zero();
    out_matrix.data[0] = d;
    out_matrix.data[5] = d;
    out_matrix.data[10] = d;
    out_matrix.data[15] = d;
    return out_matrix;
}

// -------------------------------------------------------------------------------------------------
// Mat4 copy / compare
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Copy(Sere_Mat4 m)
{
    Sere_Mat4 out;
    Sere_CopyMemory(out.data, m.data, sizeof(f32) * 16);
    return out;
}

SERE_INLINE b8 Sere_Mat4Compare(Sere_Mat4 a, Sere_Mat4 b, f32 tolerance)
{
    for (u32 i = 0; i < 16; ++i)
    {
        if (Sere_abs(a.data[i] - b.data[i]) > tolerance)
            return FALSE;
    }
    return TRUE;
}

// -------------------------------------------------------------------------------------------------
// Mat4 multiplication
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Mul(Sere_Mat4 a, Sere_Mat4 b)
{
    Sere_Mat4 out = Sere_Mat4Zero();

    for (u32 row = 0; row < 4; ++row)
    {
        for (u32 col = 0; col < 4; ++col)
        {
            out.data[col * 4 + row] =
                a.data[0 * 4 + row] * b.data[col * 4 + 0] +
                a.data[1 * 4 + row] * b.data[col * 4 + 1] +
                a.data[2 * 4 + row] * b.data[col * 4 + 2] +
                a.data[3 * 4 + row] * b.data[col * 4 + 3];
        }
    }

    return out;
}

SERE_INLINE Sere_Vec4 Sere_Mat4MulVec4(Sere_Mat4 m, Sere_Vec4 v)
{
    Sere_Vec4 out;
    out.x = m.data[0] * v.x + m.data[4] * v.y + m.data[8] * v.z + m.data[12] * v.w;
    out.y = m.data[1] * v.x + m.data[5] * v.y + m.data[9] * v.z + m.data[13] * v.w;
    out.z = m.data[2] * v.x + m.data[6] * v.y + m.data[10] * v.z + m.data[14] * v.w;
    out.w = m.data[3] * v.x + m.data[7] * v.y + m.data[11] * v.z + m.data[15] * v.w;
    return out;
}

// -------------------------------------------------------------------------------------------------
// Translation
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Translate(Sere_Vec3 t)
{
    Sere_Mat4 out = Sere_Mat4Identity();
    out.data[12] = t.x;
    out.data[13] = t.y;
    out.data[14] = t.z;
    return out;
}

// -------------------------------------------------------------------------------------------------
// Scaling
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Scale(Sere_Vec3 s)
{
    Sere_Mat4 out = Sere_Mat4Zero();
    out.data[0] = s.x;
    out.data[5] = s.y;
    out.data[10] = s.z;
    out.data[15] = 1.0f;
    return out;
}

// -------------------------------------------------------------------------------------------------
// Rotation (axis-angle)
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Rotate(f32 angle_rad, Sere_Vec3 axis)
{
    axis = Sere_Vec3Normalized(axis);

    f32 c = Sere_cos(angle_rad);
    f32 s = Sere_sin(angle_rad);
    f32 t = 1.0f - c;

    f32 x = axis.x;
    f32 y = axis.y;
    f32 z = axis.z;

    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[0] = t * x * x + c;
    out.data[1] = t * x * y + s * z;
    out.data[2] = t * x * z - s * y;

    out.data[4] = t * x * y - s * z;
    out.data[5] = t * y * y + c;
    out.data[6] = t * y * z + s * x;

    out.data[8] = t * x * z + s * y;
    out.data[9] = t * y * z - s * x;
    out.data[10] = t * z * z + c;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 transpose
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Transpose(Sere_Mat4 m)
{
    Sere_Mat4 out;

    out.data[0] = m.data[0];
    out.data[1] = m.data[4];
    out.data[2] = m.data[8];
    out.data[3] = m.data[12];

    out.data[4] = m.data[1];
    out.data[5] = m.data[5];
    out.data[6] = m.data[9];
    out.data[7] = m.data[13];

    out.data[8] = m.data[2];
    out.data[9] = m.data[6];
    out.data[10] = m.data[10];
    out.data[11] = m.data[14];

    out.data[12] = m.data[3];
    out.data[13] = m.data[7];
    out.data[14] = m.data[11];
    out.data[15] = m.data[15];

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 determinant
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_Mat4Determinant(Sere_Mat4 m)
{
    const f32 *a = m.data;

    return a[12] * a[9] * a[6] * a[3] -
           a[8] * a[13] * a[6] * a[3] -
           a[12] * a[5] * a[10] * a[3] +
           a[4] * a[13] * a[10] * a[3] +
           a[8] * a[5] * a[14] * a[3] -
           a[4] * a[9] * a[14] * a[3] -
           a[12] * a[9] * a[2] * a[7] +
           a[8] * a[13] * a[2] * a[7] +
           a[12] * a[1] * a[10] * a[7] -
           a[0] * a[13] * a[10] * a[7] -
           a[8] * a[1] * a[14] * a[7] +
           a[0] * a[9] * a[14] * a[7] +
           a[12] * a[5] * a[2] * a[11] -
           a[4] * a[13] * a[2] * a[11] -
           a[12] * a[1] * a[6] * a[11] +
           a[0] * a[13] * a[6] * a[11] +
           a[4] * a[1] * a[14] * a[11] -
           a[0] * a[5] * a[14] * a[11] -
           a[8] * a[5] * a[2] * a[15] +
           a[4] * a[9] * a[2] * a[15] +
           a[8] * a[1] * a[6] * a[15] -
           a[0] * a[9] * a[6] * a[15] -
           a[4] * a[1] * a[10] * a[15] +
           a[0] * a[5] * a[10] * a[15];
}

// -------------------------------------------------------------------------------------------------
// Mat4 inverse
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Inverse(Sere_Mat4 m)
{
    Sere_Mat4 out;
    f32 inv[16];
    const f32 *a = m.data;

    inv[0] = a[5] * a[10] * a[15] -
             a[5] * a[11] * a[14] -
             a[9] * a[6] * a[15] +
             a[9] * a[7] * a[14] +
             a[13] * a[6] * a[11] -
             a[13] * a[7] * a[10];

    inv[4] = -a[4] * a[10] * a[15] +
             a[4] * a[11] * a[14] +
             a[8] * a[6] * a[15] -
             a[8] * a[7] * a[14] -
             a[12] * a[6] * a[11] +
             a[12] * a[7] * a[10];

    inv[8] = a[4] * a[9] * a[15] -
             a[4] * a[11] * a[13] -
             a[8] * a[5] * a[15] +
             a[8] * a[7] * a[13] +
             a[12] * a[5] * a[11] -
             a[12] * a[7] * a[9];

    inv[12] = -a[4] * a[9] * a[14] +
              a[4] * a[10] * a[13] +
              a[8] * a[5] * a[14] -
              a[8] * a[6] * a[13] -
              a[12] * a[5] * a[10] +
              a[12] * a[6] * a[9];

    inv[1] = -a[1] * a[10] * a[15] +
             a[1] * a[11] * a[14] +
             a[9] * a[2] * a[15] -
             a[9] * a[3] * a[14] -
             a[13] * a[2] * a[11] +
             a[13] * a[3] * a[10];

    inv[5] = a[0] * a[10] * a[15] -
             a[0] * a[11] * a[14] -
             a[8] * a[2] * a[15] +
             a[8] * a[3] * a[14] +
             a[12] * a[2] * a[11] -
             a[12] * a[3] * a[10];

    inv[9] = -a[0] * a[9] * a[15] +
             a[0] * a[11] * a[13] +
             a[8] * a[1] * a[15] -
             a[8] * a[3] * a[13] -
             a[12] * a[1] * a[11] +
             a[12] * a[3] * a[9];

    inv[13] = a[0] * a[9] * a[14] -
              a[0] * a[10] * a[13] -
              a[8] * a[1] * a[14] +
              a[8] * a[2] * a[13] +
              a[12] * a[1] * a[10] -
              a[12] * a[2] * a[9];

    inv[2] = a[1] * a[6] * a[15] -
             a[1] * a[7] * a[14] -
             a[5] * a[2] * a[15] +
             a[5] * a[3] * a[14] +
             a[13] * a[2] * a[7] -
             a[13] * a[3] * a[6];

    inv[6] = -a[0] * a[6] * a[15] +
             a[0] * a[7] * a[14] +
             a[4] * a[2] * a[15] -
             a[4] * a[3] * a[14] -
             a[12] * a[2] * a[7] +
             a[12] * a[3] * a[6];

    inv[10] = a[0] * a[5] * a[15] -
              a[0] * a[7] * a[13] -
              a[4] * a[1] * a[15] +
              a[4] * a[3] * a[13] +
              a[12] * a[1] * a[7] -
              a[12] * a[3] * a[5];

    inv[14] = -a[0] * a[5] * a[14] +
              a[0] * a[6] * a[13] +
              a[4] * a[1] * a[14] -
              a[4] * a[2] * a[13] -
              a[12] * a[1] * a[6] +
              a[12] * a[2] * a[5];

    inv[3] = -a[1] * a[6] * a[11] +
             a[1] * a[7] * a[10] +
             a[5] * a[2] * a[11] -
             a[5] * a[3] * a[10] -
             a[9] * a[2] * a[7] +
             a[9] * a[3] * a[6];

    inv[7] = a[0] * a[6] * a[11] -
             a[0] * a[7] * a[10] -
             a[4] * a[2] * a[11] +
             a[4] * a[3] * a[10] +
             a[8] * a[2] * a[7] -
             a[8] * a[3] * a[6];

    inv[11] = -a[0] * a[5] * a[11] +
              a[0] * a[7] * a[9] +
              a[4] * a[1] * a[11] -
              a[4] * a[3] * a[9] -
              a[8] * a[1] * a[7] +
              a[8] * a[3] * a[5];

    inv[15] = a[0] * a[5] * a[10] -
              a[0] * a[6] * a[9] -
              a[4] * a[1] * a[10] +
              a[4] * a[2] * a[9] +
              a[8] * a[1] * a[6] -
              a[8] * a[2] * a[5];

    f32 det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];

    if (Sere_abs(det) <= SERE_FLOAT_EPSILON)
        return Sere_Mat4Identity();

    det = 1.0f / det;

    for (u32 i = 0; i < 16; ++i)
        out.data[i] = inv[i] * det;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 perspective projection
// Right-handed, depth range [0, 1] (Vulkan-style)
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Perspective(
    f32 fov_radians,
    f32 aspect_ratio,
    f32 near_clip,
    f32 far_clip)
{
    Sere_Mat4 out;
    Sere_ZeroMemory(out.data, sizeof(f32) * 16);

    const f32 tan_half_fov = Sere_tan(fov_radians * 0.5f);

    out.data[0] = 1.0f / (aspect_ratio * tan_half_fov);
    out.data[5] = 1.0f / tan_half_fov;
    out.data[10] = far_clip / (far_clip - near_clip);
    out.data[11] = 1.0f;
    out.data[14] = (-near_clip * far_clip) / (far_clip - near_clip);

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 orthographic projection
// Right-handed, depth range [0, 1]
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Orthographic(
    f32 left,
    f32 right,
    f32 bottom,
    f32 top,
    f32 near_clip,
    f32 far_clip)
{
    Sere_Mat4 out;
    Sere_ZeroMemory(out.data, sizeof(f32) * 16);

    out.data[0] = 2.0f / (right - left);
    out.data[5] = 2.0f / (top - bottom);
    out.data[10] = 1.0f / (far_clip - near_clip);

    out.data[12] = -(right + left) / (right - left);
    out.data[13] = -(top + bottom) / (top - bottom);
    out.data[14] = -near_clip / (far_clip - near_clip);
    out.data[15] = 1.0f;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 look-at (view matrix)
// Right-handed
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4LookAt(
    Sere_Vec3 eye,
    Sere_Vec3 target,
    Sere_Vec3 up)
{
    const Sere_Vec3 f = Sere_Vec3Normalized(Sere_Vec3Sub(target, eye));
    const Sere_Vec3 s = Sere_Vec3Normalized(Sere_Vec3Cross(f, up));
    const Sere_Vec3 u = Sere_Vec3Cross(s, f);

    Sere_Mat4 out;
    Sere_ZeroMemory(out.data, sizeof(f32) * 16);

    out.data[0] = s.x;
    out.data[1] = u.x;
    out.data[2] = -f.x;

    out.data[4] = s.y;
    out.data[5] = u.y;
    out.data[6] = -f.y;

    out.data[8] = s.z;
    out.data[9] = u.z;
    out.data[10] = -f.z;

    out.data[12] = -Sere_Vec3Dot(s, eye);
    out.data[13] = -Sere_Vec3Dot(u, eye);
    out.data[14] = Sere_Vec3Dot(f, eye);
    out.data[15] = 1.0f;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 translation
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4Translation(Sere_Vec3 translation)
{
    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[12] = translation.x;
    out.data[13] = translation.y;
    out.data[14] = translation.z;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Mat4 Euler rotations (radians)
// Right-handed
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4EulerX(f32 angle)
{
    const f32 c = Sere_cos(angle);
    const f32 s = Sere_sin(angle);

    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[5] = c;
    out.data[6] = s;
    out.data[9] = -s;
    out.data[10] = c;

    return out;
}

SERE_INLINE Sere_Mat4 Sere_Mat4EulerY(f32 angle)
{
    const f32 c = Sere_cos(angle);
    const f32 s = Sere_sin(angle);

    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[0] = c;
    out.data[2] = -s;
    out.data[8] = s;
    out.data[10] = c;

    return out;
}

SERE_INLINE Sere_Mat4 Sere_Mat4EulerZ(f32 angle)
{
    const f32 c = Sere_cos(angle);
    const f32 s = Sere_sin(angle);

    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[0] = c;
    out.data[1] = s;
    out.data[4] = -s;
    out.data[5] = c;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Combined Euler rotations
// Order matters: applied right-to-left
// -------------------------------------------------------------------------------------------------

// XYZ: rotate X, then Y, then Z
SERE_INLINE Sere_Mat4 Sere_Mat4EulerXYZ(f32 x, f32 y, f32 z)
{
    Sere_Mat4 rx = Sere_Mat4EulerX(x);
    Sere_Mat4 ry = Sere_Mat4EulerY(y);
    Sere_Mat4 rz = Sere_Mat4EulerZ(z);

    return Sere_Mat4Mul(Sere_Mat4Mul(rz, ry), rx);
}

// ZYX: rotate Z, then Y, then X (common camera order)
SERE_INLINE Sere_Mat4 Sere_Mat4EulerZYX(f32 x, f32 y, f32 z)
{
    Sere_Mat4 rx = Sere_Mat4EulerX(x);
    Sere_Mat4 ry = Sere_Mat4EulerY(y);
    Sere_Mat4 rz = Sere_Mat4EulerZ(z);

    return Sere_Mat4Mul(Sere_Mat4Mul(rx, ry), rz);
}

// -------------------------------------------------------------------------------------------------
// Euler from Vec3 helper
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_Mat4EulerVec3(Sere_Vec3 euler_radians)
{
    return Sere_Mat4EulerXYZ(
        euler_radians.x,
        euler_radians.y,
        euler_radians.z);
}

// -------------------------------------------------------------------------------------------------
// Quaternion creation & constants
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatIdentity()
{
    return (Sere_Quat){0.0f, 0.0f, 0.0f, 1.0f};
}

SERE_INLINE Sere_Quat Sere_QuatCreate(f32 x, f32 y, f32 z, f32 w)
{
    return (Sere_Quat){x, y, z, w};
}

// -------------------------------------------------------------------------------------------------
// Quaternion normalization
// -------------------------------------------------------------------------------------------------

SERE_INLINE f32 Sere_QuatLengthSquared(Sere_Quat q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

SERE_INLINE f32 Sere_QuatLength(Sere_Quat q)
{
    return Sere_sqrt(Sere_QuatLengthSquared(q));
}

SERE_INLINE Sere_Quat Sere_QuatNormalized(Sere_Quat q)
{
    const f32 len = Sere_QuatLength(q);
    if (len <= SERE_FLOAT_EPSILON)
        return Sere_QuatIdentity();

    const f32 inv = 1.0f / len;
    return (Sere_Quat){
        q.x * inv,
        q.y * inv,
        q.z * inv,
        q.w * inv};
}

// -------------------------------------------------------------------------------------------------
// Quaternion conjugate & inverse
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatConjugate(Sere_Quat q)
{
    return (Sere_Quat){-q.x, -q.y, -q.z, q.w};
}

SERE_INLINE Sere_Quat Sere_QuatInverse(Sere_Quat q)
{
    const f32 len_sq = Sere_QuatLengthSquared(q);
    if (len_sq <= SERE_FLOAT_EPSILON)
        return Sere_QuatIdentity();

    const f32 inv = 1.0f / len_sq;
    return (Sere_Quat){
        -q.x * inv,
        -q.y * inv,
        -q.z * inv,
        q.w * inv};
}

// -------------------------------------------------------------------------------------------------
// Quaternion multiplication (composition)
// Note: order matters: result = b * a applies a then b
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatMul(Sere_Quat a, Sere_Quat b)
{
    Sere_Quat out;

    out.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    out.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    out.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    out.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;

    return out;
}

// -------------------------------------------------------------------------------------------------
// Quaternion from axis-angle
// Axis must be normalized
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatFromAxisAngle(Sere_Vec3 axis, f32 angle_radians)
{
    const f32 half = angle_radians * 0.5f;
    const f32 s = Sere_sin(half);

    return Sere_QuatNormalized((Sere_Quat){
        axis.x * s,
        axis.y * s,
        axis.z * s,
        Sere_cos(half)});
}

// -------------------------------------------------------------------------------------------------
// Quaternion from Euler angles (XYZ order)
// Angles in radians
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatFromEuler(f32 pitch, f32 yaw, f32 roll)
{
    const f32 hx = pitch * 0.5f;
    const f32 hy = yaw * 0.5f;
    const f32 hz = roll * 0.5f;

    const f32 sx = Sere_sin(hx);
    const f32 cx = Sere_cos(hx);
    const f32 sy = Sere_sin(hy);
    const f32 cy = Sere_cos(hy);
    const f32 sz = Sere_sin(hz);
    const f32 cz = Sere_cos(hz);

    Sere_Quat out;
    out.x = sx * cy * cz + cx * sy * sz;
    out.y = cx * sy * cz - sx * cy * sz;
    out.z = cx * cy * sz + sx * sy * cz;
    out.w = cx * cy * cz - sx * sy * sz;

    return Sere_QuatNormalized(out);
}

// -------------------------------------------------------------------------------------------------
// Quaternion rotate vector
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Vec3 Sere_QuatRotateVec3(Sere_Quat q, Sere_Vec3 v)
{
    const Sere_Quat p = {v.x, v.y, v.z, 0.0f};
    const Sere_Quat q_inv = Sere_QuatInverse(q);
    const Sere_Quat r = Sere_QuatMul(Sere_QuatMul(q, p), q_inv);

    return (Sere_Vec3){r.x, r.y, r.z};
}

// -------------------------------------------------------------------------------------------------
// Quaternion to Mat4 (rotation only)
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Mat4 Sere_QuatToMat4(Sere_Quat q)
{
    q = Sere_QuatNormalized(q);

    const f32 xx = q.x * q.x;
    const f32 yy = q.y * q.y;
    const f32 zz = q.z * q.z;
    const f32 xy = q.x * q.y;
    const f32 xz = q.x * q.z;
    const f32 yz = q.y * q.z;
    const f32 wx = q.w * q.x;
    const f32 wy = q.w * q.y;
    const f32 wz = q.w * q.z;

    Sere_Mat4 out = Sere_Mat4Identity();

    out.data[0] = 1.0f - 2.0f * (yy + zz);
    out.data[1] = 2.0f * (xy + wz);
    out.data[2] = 2.0f * (xz - wy);

    out.data[4] = 2.0f * (xy - wz);
    out.data[5] = 1.0f - 2.0f * (xx + zz);
    out.data[6] = 2.0f * (yz + wx);

    out.data[8] = 2.0f * (xz + wy);
    out.data[9] = 2.0f * (yz - wx);
    out.data[10] = 1.0f - 2.0f * (xx + yy);

    return out;
}

// -------------------------------------------------------------------------------------------------
// Quaternion slerp
// -------------------------------------------------------------------------------------------------

SERE_INLINE Sere_Quat Sere_QuatSlerp(Sere_Quat a, Sere_Quat b, f32 t)
{
    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

    if (dot < 0.0f)
    {
        dot = -dot;
        b.x = -b.x;
        b.y = -b.y;
        b.z = -b.z;
        b.w = -b.w;
    }

    if (dot > 0.9995f)
    {
        Sere_Quat result = {
            a.x + t * (b.x - a.x),
            a.y + t * (b.y - a.y),
            a.z + t * (b.z - a.z),
            a.w + t * (b.w - a.w)};
        return Sere_QuatNormalized(result);
    }

    const f32 theta_0 = Sere_acos(dot);
    const f32 theta = theta_0 * t;

    const f32 sin_theta = Sere_sin(theta);
    const f32 sin_theta_0 = Sere_sin(theta_0);

    const f32 s0 = Sere_cos(theta) - dot * sin_theta / sin_theta_0;
    const f32 s1 = sin_theta / sin_theta_0;

    return (Sere_Quat){
        (a.x * s0) + (b.x * s1),
        (a.y * s0) + (b.y * s1),
        (a.z * s0) + (b.z * s1),
        (a.w * s0) + (b.w * s1)};
}
