#include "static.h"


int main() {
    static_mat_t left;
    randomize_static_mat(left);

    static_mat_t right;
    randomize_static_mat(right);

    static_mat_t product;
    zero_static_mat(product);


    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            for (size_t k = 0; k < SIZE; k += 4) {
                product[i][j] += left[i][k]     * right[k][j];
                product[i][j] += left[i][k + 1] * right[k + 1][j];
                product[i][j] += left[i][k + 2] * right[k + 2][j];
                product[i][j] += left[i][k + 3] * right[k + 3][j];
            }
        }
    }


    return 0;
}
