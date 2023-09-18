#include "bidim.h"
#include "config.h"
#include "timing.h"


int main() {
    dyn_bidim_mat_t left = alloc_rand_dyn_bidim_mat();
    dyn_bidim_mat_t right = alloc_rand_dyn_bidim_mat();
    dyn_bidim_mat_t product = alloc_zero_dyn_bidim_mat();


    BEGIN_TIMING_SECTION(matmul);

    for (size_t repeat = 0; repeat < REPEATS; ++repeat) {
        for (size_t i = 0; i < SIZE; ++i) {
            for (size_t k = 0; k < SIZE; ++k) {
                for (size_t j = 0; j < SIZE; ++j) {
                    product[i][j] += left[i][k] * right[k][j];
                }
            }
        }
    }

    END_TIMING_SECTION(matmul);
    print_timings();


    free_dyn_bidim_mat(left);
    free_dyn_bidim_mat(right);
    free_dyn_bidim_mat(product);

    return 0;
}
