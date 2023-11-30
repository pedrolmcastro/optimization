#ifndef SOFTBUFFER_H
#define SOFTBUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <GLFW/glfw3.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>

typedef struct Surface Surface;

#if defined(_WIN32)
extern Surface* sbSurfaceNew(size_t hwnd, size_t hinstance);
#else
extern Surface* sbSurfaceNew(void* x11Display, int screen, unsigned long window);
#endif

Surface* sbSurfaceFromGlfw(GLFWwindow* window) {
#if defined(_WIN32)
    HWND hwnd = glfwGetWin32Window(window);
    HINSTANCE hinstance = GetModuleHandle(NULL);
    return sbSurfaceNew(hwnd, hinstance);
#else
    void* display = glfwGetX11Display();
    GLFWmonitor* monitor = glfwGetX11Monitor();
    int screen = glfwGetX11Monitor(monitor);
    unsigned long window = glfwGetX11Window(window);
    return sbSurfaceNew(display, monitor, window);
#endif
}

extern void sbSurfaceDestroy(Surface* surface);

bool sbSurfaceResize(Surface* surface, uint32_t width, uint32_t height);

uint32_t* sbSurfaceBuffer(Surface* surface);

size_t sbSurfaceSize(const Surface* surface);

bool sbSurfacePresent(Surface* surface);

const char* sbErrorMessage();

#endif
