#ifndef CACHE_H
#define CACHE_H


#include <stddef.h>


#define KB (1024)
#define MB (1024 * KB)
#define GB (1024 * MB)

#define CACHE_SIZE (12 * MB)


typedef unsigned char byte_t;


static inline void clean_cache() {
    static byte_t dummy[CACHE_SIZE];

    for (size_t i = 0; i < CACHE_SIZE; ++i) {
        dummy[i] += 1;
    }
}


#endif
