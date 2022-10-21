#include "matrix_macros.h"

DECL_NAME("03_omp");

void matrix_mul(float *C, float *A, float *B, u32 n) {
    CLEAR(C, n * n);

    for (u32 i = 0; i < n; i++) {
        // #pragma omp parallel for
        for (u32 k = 0; k < n; k++) {
            for (u32 j = 0; j < n; j++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}