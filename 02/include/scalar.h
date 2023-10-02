#ifndef SCALAR_H
#define SCALAR_H


#include <stdlib.h>


typedef int scalar_t;


// WARNING: Change this functions if the scalar_t type changes

static inline scalar_t random_scalar() {
    return rand();
}

static inline void swap_scalars(scalar_t* const first, scalar_t* const second) {
    scalar_t tmp = *first;
    *first = *second;
    *second = tmp;
}


#endif
