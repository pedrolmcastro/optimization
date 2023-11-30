#include "raycast.h"

void ComputeScene() {
    u32 imageWidth = GetScreenWidth();
    u32 imageHeight = GetScreenHeight();

    f32 focalLength = 1.0f;
    f32 viewportHeight = 2.0f;
    f32 viewportWidth = viewportHeight * (f32)imageWidth / (f32)imageHeight;
    Vec3 cameraCenter = { .x = 0.0f, .y = 0.0f, .z = 0.0f };

    Vec3 viewportU = { .x = viewportWidth, .y = 0.0f, .z = 0.0f };
    Vec3 viewportV = { .x = 0.0f, .y = -viewportHeight, .z = 0.0f };

    Vec3 pixelDeltaU = Vec3_ScalarDiv(viewportU, (f32)imageWidth);
    Vec3 pixelDeltaV = Vec3_ScalarDiv(viewportV, (f32)imageHeight);

    Vec3 halfViewportU = Vec3_ScalarDiv(viewportU, 2.0f);
    Vec3 halfViewportV = Vec3_ScalarDiv(viewportV, 2.0f);
    Vec3 cameraOnViewport = Vec3_Sub(cameraCenter, (Vec3) { .x = 0.0f, .y = 0.0f, .z = focalLength });

    Vec3 viewportUpperLeft = Vec3_Sub(Vec3_Sub(cameraOnViewport, halfViewportU), halfViewportV);
    Vec3 pixelCenterOffset = Vec3_ScalarDiv(Vec3_Add(pixelDeltaU, pixelDeltaV), 2.0f);
    Vec3 startPixelPos = Vec3_Add(viewportUpperLeft, pixelCenterOffset);

    for (u32 y = 0; y < imageHeight; y++) {
        for (u32 x = 0; x < imageWidth; x++) {
            Vec3 viewportRow = Vec3_ScalarMul(pixelDeltaV, (f32)y);
            Vec3 viewportColumn = Vec3_ScalarMul(pixelDeltaU, (f32)x);
            Vec3 pixelCenter = Vec3_Add(startPixelPos, Vec3_Add(viewportRow, viewportColumn));
            Vec3 rayDirection = Vec3_Sub(pixelCenter, cameraCenter);

            Ray ray = { .dir = rayDirection, .origin = cameraCenter };
            Color color = ComputeRayColor(ray);

            WritePixel(x, y, ColorToColorU32(color));
        }
    }
}

Color ComputeRayColor(Ray ray) {
    Vec3 center = { .x = 0.0f, .y = 0.0f, .z = -1.0f };
    f32 t = HitSphere(center, 0.5f, ray);
    if (t >= 0.0f) {
        Vec3 hitPoint = Ray_At(ray, t);
        Vec3 n = Vec3_Normalize(Vec3_Sub(hitPoint, center));
        Color nColor = {
            .r = n.x + 1.0f,
            .g = n.y + 1.0f,
            .b = n.z + 1.0f,
            .a = 1.0,
        };
        return Color_ScalarMul(nColor, 0.5f);
    }

    Vec3 unitDirection = Vec3_Normalize(ray.dir);
    f32 a = 0.5f * (unitDirection.y + 1.0f);
    Color blueish = { .r = 0.5f, .g = 0.7f, .b = 1.0f, .a = a };
    return Color_Blend(COLOR_WHITE, blueish);
}

f32 HitSphere(Vec3 center, f32 radius, Ray ray) {
    Vec3 oc = Vec3_Sub(ray.origin, center);
    f32 a = Vec3_LengthSq(ray.dir);
    f32 b = 2.0f * Vec3_Dot(oc, ray.dir);
    f32 c = Vec3_LengthSq(oc) - radius * radius;
    f32 discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f) {
        return -1.0f;
    } else {
        return (-b - sqrtf(discriminant)) / (2.0f * a);
    }
}
