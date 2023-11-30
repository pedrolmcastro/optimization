#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define COLOR_BLACK (Color) { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f }
#define COLOR_WHITE (Color) { .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f }
#define COLOR_RED   (Color) { .r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f }
#define COLOR_GREEN (Color) { .r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f }
#define COLOR_BLUE  (Color) { .r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f }

#define ArraySizeof(arr) (sizeof(arr)/sizeof(*arr))
#define UNUSED(var) (void)(var)
#define Bit(i) (1 << i)

typedef uint8_t  u8;
typedef  int8_t  i8;
typedef uint16_t u16;
typedef  int16_t i16;
typedef uint32_t u32;
typedef  int32_t i32;
typedef uint64_t u64;
typedef  int64_t i64;
typedef   size_t usize;
typedef intptr_t isize;
typedef    float f32;
typedef   double f64;

typedef struct {
    u32 b : 8;
    u32 g : 8;
    u32 r : 8;
    u32 _reserved : 8;
} Pixel;

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} Color;

// Premultiplied sRGB gamma space
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} ColorU32;

typedef struct {
    Pixel* data;
    u32 width;
    u32 height;
} PxBuffer;

typedef enum {
    CF_NONE      = 0,
    CF_ANTIALIAS = Bit(0),
} ConfigFlags;

typedef struct {
    const char* winName;
    ConfigFlags flags;
    u32 width;
    u32 height;
} CanvasConfig;

typedef struct {
    const ColorU32* data;
    u32 width;
    u32 height;
} Image;

typedef struct {
    clock_t clock;
} Instant;

typedef struct {
    clock_t diff;
} Duration;

u32 GetScreenWidth();
u32 GetScreenHeight();
void WritePixel(u32 x, u32 y, ColorU32 color);

bool IsKeyPressed(u32 key);

Instant GetNow();
Duration DurationSince(Instant past);
u64 DurationInSeconds(Duration duration);
u64 DurationInMillis(Duration duration);
const char* DurationFormat(Duration duration);

inline static ColorU32 ColorToColorU32(Color linear);
inline static u8 GammaU8FromLinearF32(f32 l);
inline static f32 LinearF32FromGammaU8(u8 s);
inline static u8 FastRound(f32 r);
inline static Color Color_ScalarMul(Color color, f32 factor);

/// IMPLEMENTATION ///

inline static ColorU32 ColorToColorU32(Color linear) {
    return (ColorU32) {
        .r = GammaU8FromLinearF32(linear.r),
        .g = GammaU8FromLinearF32(linear.g),
        .b = GammaU8FromLinearF32(linear.b),
        .a = (u8)(linear.a * 255.0f),
    };
}

inline static Color ColorU32ToColor(ColorU32 packed) {
    return (Color) {
        .r = LinearF32FromGammaU8(packed.r),
        .g = LinearF32FromGammaU8(packed.g),
        .b = LinearF32FromGammaU8(packed.b),
        .a = (f32)packed.a / 255.0f,
    };
}

/// linear [0, 1] -> gamma [0, 255] (clamped).
/// Values outside this range will be clamped to the range.
inline static u8 GammaU8FromLinearF32(f32 l) {
    if (l <= 0.0f) {
        return 0;
    } else if (l <= 0.0031308f) {
        return FastRound(3294.6f * l);
    } else if (l <= 1.00f) {
        return FastRound(269.025f * powf(l, 1.0f / 2.4f) - 14.025f);
    } else {
        return 255;
    }
}

/// gamma [0, 255] -> linear [0, 1].
inline static f32 LinearF32FromGammaU8(u8 s) {
    if (s <= 10) {
        return (f32)s / 3294.6f;
    } else {
        return powf(((f32)s + 14.025f) / 269.025f, 2.4f);
    }
}

inline static u8 FastRound(f32 r) {
    return (u8)(r + 0.5f);
}

inline static Color Color_ScalarMul(Color color, f32 factor) {
    Color result;
    result.r = color.r * factor;
    result.g = color.g * factor;
    result.b = color.b * factor;
    result.a = color.a;
    return result;
}

inline static Color Color_Blend(Color background, Color overlay) {
    Color result;
    result.r = background.r * (1.0f - overlay.a) + overlay.r * overlay.a;
    result.g = background.g * (1.0f - overlay.a) + overlay.g * overlay.a;
    result.b = background.b * (1.0f - overlay.a) + overlay.b * overlay.a;
    result.a = background.a;
    return result;
}
