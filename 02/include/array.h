#ifndef ARRAY_H
#define ARRAY_H


#include <stddef.h>
#include <string.h>

#include "scalar.h"


static inline void copy_array(
    size_t const size,
    scalar_t const from[restrict const size],
    scalar_t to[restrict const size]
) {
    memcpy(to, from, size * sizeof(scalar_t));
}

static inline void randomize_array(
    size_t const size,
    scalar_t array[const size]
) {
    for (size_t i = 0; i < size; ++i) {
        array[i] = random_scalar();
    }
}


#endif
