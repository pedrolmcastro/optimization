#ifndef TIMING_H
#define TIMING_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Timing section definitions
// NOTE(gd): Maybe move this to a different file
#define TIMING_SECTIONS \
    DEFINE_TIMING_SECTION(matmul)


// Timing API
//
// BEGIN_TIMING_SECTION(name)
//    Marks the beggining of a timing section with `name` that has been previously
//    defined with `TIMING_SECTIONS`.
//
// END_TIMING_SECTION(name)
//    Marks the end of a timing section with `name` that has been previously defined
//    with `TIMING_SECTIONS`.
//
// print_timings()
//    prints all timings defined.
//
// print_timing_<name>()
//    prints the timing named `<name>`. This will function will be automatically
//    generated following the `TIMING_SECTIONS` definition.
//

#define BEGIN_TIMING_SECTION(name) \
    clock_t __start_##name = clock()

#define END_TIMING_SECTION(name) \
    clock_t __time_##name = clock() - __start_##name; \
    g_timings.name = __time_##name


struct timings {
#define DEFINE_TIMING_SECTION(name) clock_t name;

    TIMING_SECTIONS

#undef DEFINE_TIMING_SECTION
};

static struct timings g_timings = {0};

static inline int csv_mode() {
    static int cache = -1;
    if (cache == -1) {
        const char* use_csv = getenv("TIMING_CSV");
        cache = use_csv && !strcmp(use_csv, "1");
    }
    return cache;
}

#define DEFINE_TIMING_SECTION(name)                                   \
    static inline void print_timing_##name() {                        \
        float time = g_timings.name / (float)(CLOCKS_PER_SEC / 1000); \
        if (csv_mode()) {                                             \
            printf(#name ",%f\n", time);                              \
        } else {                                                      \
            printf("[" #name "]: %fms\n", time);                      \
        }                                                             \
    }

TIMING_SECTIONS

#undef DEFINE_TIMING_SECTION

static inline void print_timings() {
#define DEFINE_TIMING_SECTION(name) \
    if (!csv_mode()) printf("\t");  \
    print_timing_##name();

    if (!csv_mode()) printf("Timings:\n");
    TIMING_SECTIONS

#undef DEFINE_TIMING_SECTION
}

#endif
