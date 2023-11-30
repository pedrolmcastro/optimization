#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "log.h"
#include "api.h"

void LogEvent(LogLevel level, const char* fmt, ...) {
    static const char* const level_name[] = {
        NULL,
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
    };

    assert(level < ArraySizeof(level_name));
    fprintf(stderr, "[%s] ", level_name[level]);

    va_list ap;
    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);

    va_end(ap);

    fprintf(stderr, "\n");
}
