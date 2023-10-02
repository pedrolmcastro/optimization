#ifndef SORT_H
#define SORT_H


#include <stddef.h>
#include <stdbool.h>

#include "array.h"
#include "scalar.h"


static inline bool is_sorted(
    size_t const size,
    scalar_t const array[const size]
) {
    for (size_t i = 1; i < size; ++i) {
        if (array[i] < array[i - 1]) {
            return false;
        }
    }

    return true;
}


static inline void heapify(
    size_t const root,
    size_t const size,
    scalar_t array[const size]
) {
    size_t largest = root;

    size_t const left = 2 * root + 1;
    size_t const right = left + 1;

    if (left < size && array[left] > array[largest]) {
        largest = left;
    }

    if (right < size && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != root) {
        swap_scalars(&array[root], &array[largest]);
        heapify(largest, size, array);
    }
}

static inline void heap_sort(size_t const size, scalar_t array[const size]) {
    for (size_t i = size / 2; i > 0; --i) {
        heapify(i - 1, size, array);
    }

    for (size_t i = size - 1; i > 0; --i) {
        swap_scalars(&array[0], &array[i]);
        heapify(0, i, array);
    }
}


static inline void merge(
    size_t const left_size,
    scalar_t const left[restrict const left_size],
    size_t const right_size,
    scalar_t const right[restrict const right_size],
    scalar_t output[restrict const left_size + right_size]
) {
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;

    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            output[k++] = left[i++];
        }
        else {
            output[k++] = right[j++];
        }
    }

    while (i < left_size) {
        output[k++] = left[i++];
    }

    while (j < right_size) {
        output[k++] = right[j++];
    }
}

static inline void merge_sort(size_t const size, scalar_t array[const size]) {
    if (size <= 1) {
        return;
    }

    size_t const left_size = size / 2;
    scalar_t left[left_size];
    copy_array(left_size, array, left);

    size_t const right_size = size - left_size;
    scalar_t right[right_size];
    copy_array(right_size, &array[left_size], right);

    merge_sort(left_size, left);
    merge_sort(right_size, right);

    merge(left_size, left, right_size, right, array);
}


static inline scalar_t median_pivot(
    size_t const low,
    size_t const high,
    scalar_t const array[const high - low + 1]
) {
    scalar_t const at_low = array[low];
    scalar_t const at_high = array[high];
    scalar_t const at_middle = array[(low + high) / 2];

    if (at_low < at_high) {
        if (at_high < at_middle) {
            return at_high;
        }

        if (at_low < at_middle) {
            return at_middle;
        }

        return at_low;
    }

    if (at_low < at_middle) {
        return at_low;
    }

    if (at_high < at_middle) {
        return at_middle;
    }

    return at_high;
}

static inline void quick_sort(
    size_t const low,
    size_t const high,
    scalar_t array[const high - low + 1]
) {
    size_t i = low;
    size_t j = high;

    scalar_t const pivot = median_pivot(low, high, array);

    while (i <= j) {
        while (i < high && array[i] < pivot) {
            ++i;
        }

        while (j > low && array[j] > pivot) {
            --j;
        }

        if (i <= j) {
            swap_scalars(&array[i++], &array[j--]);
        }
    }

    if (i < high) {
        quick_sort(i, high, array);
    }

    if (j > low) {
        quick_sort(low, j, array);
    }
}


#endif
