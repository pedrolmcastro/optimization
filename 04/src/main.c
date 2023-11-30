#include <stdio.h>
#include <assert.h>

#include <GLFW/glfw3.h>
#include <softbuffer.h>

#include "log.h"
#include "math_utils.h"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

// Globals
PxBuffer s_FrameBuffer = { 0 };

Color s_FillColor = { .r = 255, .g = 255, .b = 255, .a = 255 };

double s_DeltaTime = 0;

u32 s_Flags = 0;

GLFWwindow* s_GlfwWindow = NULL;
Surface* s_Surface = NULL;

// Sketch
extern CanvasConfig Setup();
extern void Draw();

// Internal
static void InitializeLibraries();

static void UpdateFrameBuffer(u32* buf, u32 size, u32 width, u32 height);
// static void FrameBufferCB(GLFWwindow* window, int width, int height);

int main() {
    assert(sizeof(Pixel) == sizeof(u32));

    InitializeLibraries();

    CanvasConfig config = Setup();
    const char* winName = config.winName ? config.winName : "CCanvas";

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    u32 initWidth  = config.width  != 0 ? config.width  : DEFAULT_WIDTH;
    u32 initHeight = config.height != 0 ? config.height : DEFAULT_WIDTH;
    s_GlfwWindow = glfwCreateWindow(initWidth, initHeight, winName, NULL, NULL);

    s_Surface = sbSurfaceFromGlfw(s_GlfwWindow);
    if (!s_Surface) {
        LogError("Failed to create surface");
        return 1;
    }

    while (!glfwWindowShouldClose(s_GlfwWindow)) {
        glfwPollEvents();

        int width, height;
        glfwGetWindowSize(s_GlfwWindow, &width, &height);
        if (!sbSurfaceResize(s_Surface, width, height)) {
            LogError("failed to resize the surface: width = %d, height = %d", width, height);
            break;
        }

        u32* buffer = sbSurfaceBuffer(s_Surface);
        usize size = sbSurfaceSize(s_Surface);
        if (!buffer) {
            LogError("failed to get buffer");
            break;
        }
        UpdateFrameBuffer(buffer, (u32)size, width, height);
        Draw();

        if (!sbSurfacePresent(s_Surface)) {
            LogError("failed to present the buffer");
        }
    }

    LogInfo("shutting down");

    sbSurfaceDestroy(s_Surface);

    glfwDestroyWindow(s_GlfwWindow);
    return 0;
}

static void InitializeLibraries() {
    if (!glfwInit()) {
        LogError("failed to initialize glfw");
        exit(EXIT_FAILURE);
    }
}

static void UpdateFrameBuffer(u32* data, u32 size, u32 width, u32 height) {
    memset(data, 0, size * sizeof(u32));
    if (width * height != size) {
        LogError("inconsistent window and buffer sizes, width = %d, height = %d, len = %d", width, height, size);
    }
    s_FrameBuffer = (PxBuffer){ .data = (Pixel*)data, .width = width, .height = height };
}
