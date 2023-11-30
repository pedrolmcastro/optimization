#pragma once

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    Vec3 dir;
    Vec3 origin;
} Ray;

static inline Vec3 Vec3_Sub(Vec3 lhs, Vec3 rhs);
static inline Vec3 Vec3_Add(Vec3 lhs, Vec3 rhs);
static inline Vec3 Vec3_ScalarDiv(Vec3 lhs, f32 rhs);
static inline Vec3 Vec3_ScalarMul(Vec3 lhs, f32 rhs);
static inline Vec3 Vec3_Normalize(Vec3 v);
static inline f32 Vec3_Dot(Vec3 lhs, Vec3 rhs);
static inline f32 Vec3_LengthSq(Vec3 v);
static inline f32 Vec3_Length(Vec3 v);

static inline Vec3 Ray_At(Ray ray, f32 t);

/// IMPLEMENTATION ///

static inline Vec3 Vec3_Sub(Vec3 lhs, Vec3 rhs) {
    Vec3 result;
    result.x = lhs.x - rhs.x;
    result.y = lhs.y - rhs.y;
    result.z = lhs.z - rhs.z;
    return result;
}

static inline Vec3 Vec3_Add(Vec3 lhs, Vec3 rhs) {
    Vec3 result;
    result.x = lhs.x + rhs.x;
    result.y = lhs.y + rhs.y;
    result.z = lhs.z + rhs.z;
    return result;
}

static inline Vec3 Vec3_ScalarDiv(Vec3 lhs, f32 rhs) {
    Vec3 result;
    result.x = lhs.x / rhs;
    result.y = lhs.y / rhs;
    result.z = lhs.z / rhs;
    return result;
}

static inline Vec3 Vec3_ScalarMul(Vec3 lhs, f32 rhs) {
    Vec3 result;
    result.x = lhs.x * rhs;
    result.y = lhs.y * rhs;
    result.z = lhs.z * rhs;
    return result;
}

static inline Vec3 Vec3_Normalize(Vec3 v) {
    return Vec3_ScalarDiv(v, Vec3_Length(v));
}

static inline f32 Vec3_Dot(Vec3 lhs, Vec3 rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

static inline f32 Vec3_LengthSq(Vec3 v) {
    return Vec3_Dot(v, v);
}

static inline f32 Vec3_Length(Vec3 v) {
    return sqrtf(Vec3_LengthSq(v));
}

static inline Vec3 Ray_At(Ray ray, f32 t) {
    return Vec3_Add(ray.origin, Vec3_ScalarMul(ray.dir, t));
}
