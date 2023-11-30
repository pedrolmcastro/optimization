#pragma once

typedef union {
    __m128 v;
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 _w;
    };
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
    result.v = _mm_sub_ps(lhs.v, rhs.v);
    return result;
}

static inline Vec3 Vec3_Add(Vec3 lhs, Vec3 rhs) {
    Vec3 result;
    result.v = _mm_add_ps(lhs.v, rhs.v);
    return result;
}

static inline Vec3 Vec3_ScalarDiv(Vec3 lhs, f32 rhs) {
    Vec3 result;
    result.v = _mm_div_ps(lhs.v, _mm_set1_ps(rhs));
    return result;
}

static inline Vec3 Vec3_ScalarMul(Vec3 lhs, f32 rhs) {
    Vec3 result;
    result.v = _mm_mul_ps(lhs.v, _mm_set1_ps(rhs));
    return result;
}

static inline Vec3 Vec3_Normalize(Vec3 v) {
    return Vec3_ScalarDiv(v, Vec3_Length(v));
}

static inline f32 Vec3_Dot(Vec3 lhs, Vec3 rhs) {
    __m128 result = _mm_dp_ps(lhs.v, rhs.v, 0b11110001);
    return result.m128_f32[0];
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
