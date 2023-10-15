#include <assert.h>

#include "sort.h"
#include "array.h"
#include "cache.h"
#include "scalar.h"


#define SIZE (379 * KB)

int main() {
    scalar_t random[SIZE];
    randomize_array(SIZE, random);

    scalar_t sorted[SIZE];


    copy_array(SIZE, random, sorted);
    clean_cache();
    heap_sort(SIZE, sorted);
    assert(is_sorted(SIZE, sorted));


    copy_array(SIZE, random, sorted);
    clean_cache();
    merge_sort(SIZE, sorted);
    assert(is_sorted(SIZE, sorted));


    copy_array(SIZE, random, sorted);
    clean_cache();
    quick_sort(0, SIZE - 1, sorted);
    assert(is_sorted(SIZE, sorted));
}
