#include <time.h>
#include <stdio.h>

#include "api.h"
#include "log.h"
#include "globals.h"

u32 GetScreenWidth() {
    return s_FrameBuffer.width;
}

u32 GetScreenHeight() {
    return s_FrameBuffer.height;
}

void WritePixel(u32 x, u32 y, ColorU32 color) {
    u32 i = y * s_FrameBuffer.width + x;
    Pixel* px = &s_FrameBuffer.data[i];
    px->r = color.r;
    px->g = color.g;
    px->b = color.b;
}

Instant GetNow() {
    return (Instant) { .clock = clock() };
}

Duration DurationSince(Instant past) {
    Instant now = GetNow();
    return (Duration) { .diff = now.clock - past.clock };
}

u64 DurationInSeconds(Duration duration) {
    return duration.diff / CLOCKS_PER_SEC;
}

u64 DurationInMillis(Duration duration) {
    return duration.diff / (CLOCKS_PER_SEC / 1000);
}

const char* DurationFormat(Duration duration) {
    static char buf[32];

    u64 millis = DurationInMillis(duration);
    sprintf(buf, "%zums", millis);
    return buf;
}

bool IsKeyPressed(u32 key) {
    return glfwGetKey(s_GlfwWindow, key) == GLFW_PRESS;
}
