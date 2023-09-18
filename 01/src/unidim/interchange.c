#include "config.h"
#include "timing.h"
#include "unidim.h"


int main() {
    dyn_unidim_mat_t left = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t right = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t product = alloc_zero_dyn_unidim_mat();


    BEGIN_TIMING_SECTION(execution_time);

    for (size_t repeat = 0; repeat < REPEATS; ++repeat) {
        for (size_t i = 0; i < SIZE; ++i) {
            for (size_t k = 0; k < SIZE; ++k) {
                for (size_t j = 0; j < SIZE; ++j) {
                    product[i * SIZE + j] += left[i * SIZE + k] * right[k * SIZE + j];
                }
            }
        }
    }

    END_TIMING_SECTION(execution_time);
    print_timings();


    free_dyn_unidim_mat(left);
    free_dyn_unidim_mat(right);
    free_dyn_unidim_mat(product);

    return 0;
}
