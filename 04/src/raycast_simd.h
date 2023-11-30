#pragma once

#include "prelude.h"
#include "vec.h"
#include "simd.h"

typedef struct {
    __m128 r;
    __m128 g;
    __m128 b;
    __m128 a;
} ColorSimd;

#define COLOR_WHITE_SIMD (ColorSimd) { \
        .r = _mm_set1_ps(1.0f),        \
        .g = _mm_set1_ps(1.0f),        \
        .b = _mm_set1_ps(1.0f),        \
        .a = _mm_set1_ps(1.0f),        \
    }

void ComputeSceneSimd();
f32Simd HitSphereSimd(Vec3Simd center, f32Simd radius, RaySimd ray);
ColorSimd ComputeSkySimd(RaySimd ray);
ColorSimd ComputeRayColorSimd(RaySimd ray);
