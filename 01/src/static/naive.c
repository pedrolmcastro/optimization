#include "config.h"
#include "static.h"
#include "timing.h"


int main() {
    static_mat_t left;
    randomize_static_mat(left);

    static_mat_t right;
    randomize_static_mat(right);

    static_mat_t product;
    zero_static_mat(product);


    BEGIN_TIMING_SECTION(matmul);

    for (size_t repeat = 0; repeat < REPEATS; ++repeat) {
        for (size_t i = 0; i < SIZE; ++i) {
            for (size_t j = 0; j < SIZE; ++j) {
                for (size_t k = 0; k < SIZE; ++k) {
                    product[i][j] += left[i][k] * right[k][j];
                }
            }
        }
    }

    END_TIMING_SECTION(matmul);
    print_timings();


    return 0;
}
