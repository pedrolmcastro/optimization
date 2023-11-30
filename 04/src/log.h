#pragma once

#define LogDebug(...) LogEvent(LOG_DEBUG, __VA_ARGS__)
#define LogInfo(...)  LogEvent(LOG_INFO, __VA_ARGS__)
#define LogWarn(...)  LogEvent(LOG_WARN, __VA_ARGS__)
#define LogError(...) LogEvent(LOG_ERROR, __VA_ARGS__)

typedef enum {
    LOG_ALL = 0,

    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,

    LOG_NONE,
} LogLevel;

void LogEvent(LogLevel level, const char* fmt, ...);
