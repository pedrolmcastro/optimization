#include "dyn/unidim.h"


int main() {
    dyn_unidim_mat_t left = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t right = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t product = alloc_zero_dyn_unidim_mat();


    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            for (size_t k = 0; k < SIZE; k += 4) {
                product[i * SIZE + j] += left[i * SIZE + k]     * right[k       * SIZE + j];
                product[i * SIZE + j] += left[i * SIZE + k + 1] * right[(k + 1) * SIZE + j];
                product[i * SIZE + j] += left[i * SIZE + k + 2] * right[(k + 2) * SIZE + j];
                product[i * SIZE + j] += left[i * SIZE + k + 3] * right[(k + 3) * SIZE + j];
            }
        }
    }


    free_dyn_unidim_mat(left);
    free_dyn_unidim_mat(right);
    free_dyn_unidim_mat(product);

    return 0;
}
