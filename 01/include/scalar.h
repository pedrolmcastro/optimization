#ifndef SCALAR_H
#define SCALAR_H


#include <stdlib.h>


typedef int scalar_t;

static inline scalar_t rand_scalar() {
    // WARNING: Change this function if the scalar_t type changes
    return rand() % 1000;
}


#endif
