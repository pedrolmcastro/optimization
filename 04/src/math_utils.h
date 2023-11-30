#pragma once

#include "api.h"

static inline f32 Max(f32 a, f32 b) {
    return a > b ? a : b;
}

static inline f32 Min(f32 a, f32 b) {
    return a < b ? a : b;
}

static inline f32 Clamp(f32 value, f32 min, f32 max) {
    return Max(Min(value, max), min);
}

static inline f32 Remap(f32 value, f32 oldMin, f32 oldMax, f32 newMin, f32 newMax) {
    f32 zeroToOne = value / (oldMax - oldMin) - oldMin;
    return zeroToOne * (newMax - newMin) + newMin;
}

// U32

static inline u32 MaxU32(u32 a, u32 b) {
    return a > b ? a : b;
}

static inline u32 MinU32(u32 a, u32 b) {
    return a < b ? a : b;
}

static inline u32 ClampU32(u32 value, u32 min, u32 max) {
    return MaxU32(MinU32(value, max), min);
}
