#include <stdbool.h>
#include <math.h>
#include <immintrin.h>

#include <GLFW/glfw3.h>

#include "prelude.h"
#include "vec.h"
#include "simd.h"
#include "raycast.h"
#include "raycast_simd.h"

static bool s_UseSimd = false;

CanvasConfig Setup() {
    CanvasConfig config = { 0 };
    config.winName = "My Sketch";
    config.flags |= CF_ANTIALIAS;
    config.width = 1280;
    config.height = 720;

    return config;
}

void Draw() {
    s_UseSimd = IsKeyPressed(GLFW_KEY_SPACE);

    Instant start = GetNow();

    if (s_UseSimd) {
        ComputeSceneSimd();
    } else {
        ComputeScene();
    }

    Duration took = DurationSince(start);

    const char* timeFmt = DurationFormat(took);

    if (s_UseSimd) {
        LogInfo("frame took %s (SIMD)", timeFmt);
    } else {
        LogInfo("frame took %s (NO SIMD)", timeFmt);
    }
}
