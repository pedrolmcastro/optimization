#pragma once

#include <immintrin.h>

typedef __m128 f32Simd;

typedef struct {
    __m128 x;
    __m128 y;
    __m128 z;
} Vec3Simd;

typedef struct {
    Vec3Simd dir;
    Vec3Simd origin;
} RaySimd;

static inline Vec3Simd Vec3Simd_Sub(Vec3Simd lhs, Vec3Simd rhs);
static inline Vec3Simd Vec3Simd_Add(Vec3Simd lhs, Vec3Simd rhs);
static inline Vec3Simd Vec3Simd_ScalarDiv(Vec3Simd lhs, f32Simd rhs);
static inline Vec3Simd Vec3Simd_ScalarMul(Vec3Simd lhs, f32Simd rhs);
static inline Vec3Simd Vec3Simd_Normalize(Vec3Simd v);
static inline f32Simd Vec3Simd_Dot(Vec3Simd lhs, Vec3Simd rhs);
static inline f32Simd Vec3Simd_LengthSq(Vec3Simd v);
static inline f32Simd Vec3Simd_Length(Vec3Simd v);

static inline Vec3Simd RaySimd_At(RaySimd ray, f32Simd t);

static inline Vec3Simd Vec3Simd_Sub(Vec3Simd lhs, Vec3Simd rhs) {
    Vec3Simd result;
    result.x = _mm_sub_ps(lhs.x, rhs.x);
    result.y = _mm_sub_ps(lhs.y, rhs.y);
    result.z = _mm_sub_ps(lhs.z, rhs.z);
    return result;
}

static inline Vec3Simd Vec3Simd_Add(Vec3Simd lhs, Vec3Simd rhs) {
    Vec3Simd result;
    result.x = _mm_add_ps(lhs.x, rhs.x);
    result.y = _mm_add_ps(lhs.y, rhs.y);
    result.z = _mm_add_ps(lhs.z, rhs.z);
    return result;
}

static inline Vec3Simd Vec3Simd_ScalarDiv(Vec3Simd lhs, f32Simd rhs) {
    Vec3Simd result;
    result.x = _mm_div_ps(lhs.x, rhs);
    result.y = _mm_div_ps(lhs.y, rhs);
    result.z = _mm_div_ps(lhs.z, rhs);
    return result;
}

static inline Vec3Simd Vec3Simd_ScalarMul(Vec3Simd lhs, f32Simd rhs) {
    Vec3Simd result;
    result.x = _mm_mul_ps(lhs.x, rhs);
    result.y = _mm_mul_ps(lhs.y, rhs);
    result.z = _mm_mul_ps(lhs.z, rhs);
    return result;
}

static inline Vec3Simd Vec3Simd_Normalize(Vec3Simd v) {
    return Vec3Simd_ScalarDiv(v, Vec3Simd_Length(v));
}

static inline f32Simd Vec3Simd_Dot(Vec3Simd lhs, Vec3Simd rhs) {
    return _mm_fmadd_ps(lhs.x, rhs.x,
                        _mm_add_ps(_mm_mul_ps(lhs.y, rhs.y),
                                   _mm_mul_ps(lhs.z, rhs.z)));
}

static inline f32Simd Vec3Simd_LengthSq(Vec3Simd v) {
    return Vec3Simd_Dot(v, v);
}

static inline f32Simd Vec3Simd_Length(Vec3Simd v) {
    return _mm_sqrt_ps(Vec3Simd_LengthSq(v));
}

static inline Vec3Simd RaySimd_At(RaySimd ray, f32Simd t) {
    return Vec3Simd_Add(ray.origin, Vec3Simd_ScalarMul(ray.dir, t));
}
