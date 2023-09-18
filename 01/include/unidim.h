#ifndef DYN_UNIDIM_H
#define DYN_UNIDIM_H


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "scalar.h"


typedef scalar_t* dyn_unidim_mat_t;


static inline dyn_unidim_mat_t alloc_rand_dyn_unidim_mat() {
    size_t const count = SIZE * SIZE;

    dyn_unidim_mat_t mat = (dyn_unidim_mat_t)malloc(count * sizeof(scalar_t));
    assert(mat != NULL);

    for (size_t i = 0; i < count; ++i) {
        mat[i] = rand_scalar();
    }

    return mat;
}

static inline dyn_unidim_mat_t alloc_zero_dyn_unidim_mat() {
    dyn_unidim_mat_t mat = (dyn_unidim_mat_t)calloc(SIZE * SIZE, sizeof(scalar_t));
    assert(mat != NULL);
    return mat;
}

static inline void free_dyn_unidim_mat(dyn_unidim_mat_t mat) {
    free(mat);
}


#endif
