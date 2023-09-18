#ifndef BIDIM_H
#define BIDIM_H


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "scalar.h"


typedef scalar_t** dyn_bidim_mat_t;


static inline dyn_bidim_mat_t alloc_rand_dyn_bidim_mat() {
    dyn_bidim_mat_t mat = (dyn_bidim_mat_t)malloc(SIZE * sizeof(scalar_t*));
    assert(mat != NULL);

    for (size_t i = 0; i < SIZE; ++i) {
        mat[i] = (scalar_t*)malloc(SIZE * sizeof(scalar_t));
        assert(mat[i] != NULL);

        for (size_t j = 0; j < SIZE; ++j) {
            mat[i][j] = rand_scalar();
        }
    }

    return mat;
}

static inline dyn_bidim_mat_t alloc_zero_dyn_bidim_mat() {
    dyn_bidim_mat_t mat = (dyn_bidim_mat_t)malloc(SIZE * sizeof(scalar_t*));
    assert(mat != NULL);

    for (size_t i = 0; i < SIZE; ++i) {
        mat[i] = (scalar_t*)calloc(SIZE, sizeof(scalar_t));
        assert(mat[i] != NULL);
    }

    return mat;
}

static inline void free_dyn_bidim_mat(dyn_bidim_mat_t mat) {
    for (size_t i = 0; i < SIZE; ++i) {
        free(mat[i]);
    }

    free(mat);
}


#endif
