#include "raycast_simd.h"

inline static ColorSimd ColorSimd_ScalarMul(ColorSimd color, f32Simd factor);
inline static ColorSimd ColorSimd_Blend(ColorSimd background, ColorSimd overlay);

void ComputeSceneSimd() {
    f32Simd zero = _mm_set1_ps(0.0f);

    u32 imageWidth = GetScreenWidth();
    u32 imageHeight = GetScreenHeight();

    f32 focalLength = 1.0f;
    f32 viewportHeight = 2.0f;
    f32 viewportWidth = viewportHeight * (f32)imageWidth / (f32)imageHeight;
    Vec3Simd cameraCenter = { .x = zero, .y = zero, .z = zero };

    Vec3Simd viewportU = { .x = _mm_set1_ps(viewportWidth), .y = zero, .z = zero };
    Vec3Simd viewportV = { .x = zero, .y = _mm_sub_ps(zero, _mm_set1_ps(viewportHeight)), .z = zero };

    Vec3Simd pixelDeltaU = Vec3Simd_ScalarDiv(viewportU, _mm_set1_ps((f32)imageWidth));
    Vec3Simd pixelDeltaV = Vec3Simd_ScalarDiv(viewportV, _mm_set1_ps((f32)imageHeight));

    Vec3Simd halfViewportU = Vec3Simd_ScalarDiv(viewportU, _mm_set1_ps(2.0f));
    Vec3Simd halfViewportV = Vec3Simd_ScalarDiv(viewportV, _mm_set1_ps(2.0f));
    Vec3Simd cameraOnViewport = Vec3Simd_Sub(cameraCenter, (Vec3Simd) { .x = zero, .y = zero, .z = _mm_set1_ps(focalLength) });

    Vec3Simd viewportUpperLeft = Vec3Simd_Sub(Vec3Simd_Sub(cameraOnViewport, halfViewportU), halfViewportV);
    Vec3Simd pixelCenterOffset = Vec3Simd_ScalarDiv(Vec3Simd_Add(pixelDeltaU, pixelDeltaV), _mm_set1_ps(2.0f));
    Vec3Simd startPixelPos = Vec3Simd_Add(viewportUpperLeft, pixelCenterOffset);
    
    u32 stepY = 2;
    u32 stepX = 2;
    u32 nLanes = 4;

    f32Simd offsetsY = _mm_set_ps(0.0f, 0.0f, 1.0f, 1.0f);
    f32Simd offsetsX = _mm_set_ps(0.0f, 1.0f, 0.0f, 1.0f);

    for (u32 y = 0; y + stepY - 1 < imageHeight; y += stepY) {
        for (u32 x = 0; x + stepX - 1 < imageWidth; x += stepX) {
            f32Simd ys = _mm_add_ps(_mm_set1_ps((f32)y), offsetsY);
            f32Simd xs = _mm_add_ps(_mm_set1_ps((f32)x), offsetsX);
            Vec3Simd viewportRow = Vec3Simd_ScalarMul(pixelDeltaV, ys);
            Vec3Simd viewportColumn = Vec3Simd_ScalarMul(pixelDeltaU, xs);
            Vec3Simd pixelCenter = Vec3Simd_Add(startPixelPos, Vec3Simd_Add(viewportRow, viewportColumn));
            Vec3Simd rayDirection = Vec3Simd_Sub(pixelCenter, cameraCenter);

            RaySimd ray = { .dir = rayDirection, .origin = cameraCenter };
            ColorSimd color = ComputeRayColorSimd(ray);

            for (u32 lane = 0; lane < nLanes; lane++) {
                Color colorAt = {
                    .r = color.r.m128_f32[lane],
                    .g = color.g.m128_f32[lane],
                    .b = color.b.m128_f32[lane],
                    .a = color.a.m128_f32[lane],
                };
                u32 dx = (u32)offsetsX.m128_f32[lane];
                u32 dy = (u32)offsetsY.m128_f32[lane];
                WritePixel(x + dx, y + dy, ColorToColorU32(colorAt));
            }
        }
    }
}

ColorSimd ComputeRayColorSimd(RaySimd ray) {
    Vec3Simd center = {
        .x = _mm_set1_ps( 0.0f),
        .y = _mm_set1_ps( 0.0f),
        .z = _mm_set1_ps(-1.0f),
    };
    f32Simd t = HitSphereSimd(center, _mm_set1_ps(0.5f), ray);
    __m128 hitMask = _mm_cmpge_ps(t, _mm_set1_ps(0.0f));
    ColorSimd sky = ComputeSkySimd(ray);
    if (_mm_testz_ps(hitMask, hitMask)) {
        return sky;
    }
    Vec3Simd hitPoint = RaySimd_At(ray, t);
    Vec3Simd n = Vec3Simd_Normalize(Vec3Simd_Sub(hitPoint, center));
    ColorSimd nColor = {
        .r = _mm_add_ps(n.x, _mm_set1_ps(1.0f)),
        .g = _mm_add_ps(n.y, _mm_set1_ps(1.0f)),
        .b = _mm_add_ps(n.z, _mm_set1_ps(1.0f)),
        .a = _mm_set1_ps(1.0),
    };
    ColorSimd sphere = ColorSimd_ScalarMul(nColor, _mm_set1_ps(0.5f));
    return (ColorSimd) {
        .r = _mm_blendv_ps(sky.r, sphere.r, hitMask),
        .g = _mm_blendv_ps(sky.g, sphere.g, hitMask),
        .b = _mm_blendv_ps(sky.b, sphere.b, hitMask),
        .a = _mm_blendv_ps(sky.a, sphere.a, hitMask),
    };
}

f32Simd HitSphereSimd(Vec3Simd center, f32Simd radius, RaySimd ray) {
    f32Simd zero = _mm_set1_ps(0.0f);
    f32Simd negOne = _mm_set1_ps(-1.0f);
    Vec3Simd oc = Vec3Simd_Sub(ray.origin, center);
    f32Simd a = Vec3Simd_LengthSq(ray.dir);
    f32Simd b = _mm_mul_ps(_mm_set1_ps(2.0f), Vec3Simd_Dot(oc, ray.dir));
    f32Simd c = _mm_sub_ps(Vec3Simd_LengthSq(oc), _mm_mul_ps(radius, radius));
    f32Simd discriminant = _mm_sub_ps(_mm_mul_ps(b, b),
                                      _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(4.0f), a), c));
    __m128 le0Mask = _mm_cmplt_ps(discriminant, zero);
    f32Simd negB = _mm_sub_ps(zero, b);
    f32Simd t = _mm_div_ps(_mm_sub_ps(negB, _mm_sqrt_ps(discriminant)), _mm_mul_ps(_mm_set1_ps(2.0f), a));
    return _mm_blendv_ps(t, negOne, le0Mask);
}

ColorSimd ComputeSkySimd(RaySimd ray) {
    Vec3Simd unitDirection = Vec3Simd_Normalize(ray.dir);
    f32Simd a = _mm_mul_ps(_mm_set1_ps(0.5f), _mm_add_ps(unitDirection.y, _mm_set1_ps(1.0f)));
    // Color blueish = { .r = 0.5f, .g = 0.7f, .b = 1.0f, .a = a };
    ColorSimd blueish = {
        .r = _mm_set1_ps(0.5f),
        .g = _mm_set1_ps(0.7f),
        .b = _mm_set1_ps(1.0f),
        .a = a,
    };
    return ColorSimd_Blend(COLOR_WHITE_SIMD, blueish);
}

inline static ColorSimd ColorSimd_ScalarMul(ColorSimd color, f32Simd factor) {
    ColorSimd result;
    result.r = _mm_mul_ps(color.r, factor);
    result.g = _mm_mul_ps(color.g, factor);
    result.b = _mm_mul_ps(color.b, factor);
    result.a = color.a;
    return result;
}

inline static ColorSimd ColorSimd_Blend(ColorSimd background, ColorSimd overlay) {
    __m128 one = _mm_set1_ps(1.0f);
    ColorSimd result;
    result.r = _mm_add_ps(_mm_mul_ps(background.r, _mm_sub_ps(one, overlay.a)), _mm_mul_ps(overlay.r, overlay.a));
    result.g = _mm_add_ps(_mm_mul_ps(background.g, _mm_sub_ps(one, overlay.a)), _mm_mul_ps(overlay.g, overlay.a));
    result.b = _mm_add_ps(_mm_mul_ps(background.b, _mm_sub_ps(one, overlay.a)), _mm_mul_ps(overlay.b, overlay.a));
    result.a = background.a;
    return result;
}
