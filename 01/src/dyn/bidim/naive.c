#include "dyn/bidim.h"


int main() {
    dyn_bidim_mat_t left = alloc_rand_dyn_bidim_mat();
    dyn_bidim_mat_t right = alloc_rand_dyn_bidim_mat();
    dyn_bidim_mat_t product = alloc_zero_dyn_bidim_mat();


    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            for (size_t k = 0; k < SIZE; ++k) {
                product[i][j] += left[i][k] * right[k][j];
            }
        }
    }


    free_dyn_bidim_mat(left);
    free_dyn_bidim_mat(right);
    free_dyn_bidim_mat(product);

    return 0;
}
