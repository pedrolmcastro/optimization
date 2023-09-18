#ifndef STATIC_H
#define STATIC_H


#include <stddef.h>
#include <string.h>

#include "config.h"
#include "scalar.h"


typedef scalar_t static_mat_t[SIZE][SIZE];


static inline void randomize_static_mat(static_mat_t mat) {
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            mat[i][j] = rand_scalar();
        }
    }
}

static inline void zero_static_mat(static_mat_t mat) {
    memset(mat, 0, SIZE * SIZE * sizeof(scalar_t));
}


#endif
