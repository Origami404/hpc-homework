#include "matrix_macros.h"

DECL_NAME("00_baseline");

void matrix_mul(float *C, float *A, float *B, u32 n) {
    CLEAR(C, n * n);

    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            for (u32 k = 0; k < n; k++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}
