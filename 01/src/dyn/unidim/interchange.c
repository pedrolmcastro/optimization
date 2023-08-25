#include "dyn/unidim.h"


int main() {
    dyn_unidim_mat_t left = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t right = alloc_rand_dyn_unidim_mat();
    dyn_unidim_mat_t product = alloc_zero_dyn_unidim_mat();


    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t k = 0; k < SIZE; ++k) {
            for (size_t j = 0; j < SIZE; ++j) {
                product[i * SIZE + j] += left[i * SIZE + k] * right[k * SIZE + j];
            }
        }
    }


    free_dyn_unidim_mat(left);
    free_dyn_unidim_mat(right);
    free_dyn_unidim_mat(product);

    return 0;
}
