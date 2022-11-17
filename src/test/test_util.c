#include <math.h>
#include <time.h>
#include <unistd.h>
#include "matrix_macros.h"

float float_rand(float min, float max) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * (max - min);        /* [min, max] */
}

void init_random_matrix(float *m, u32 n) {
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            m[i * n + j] = float_rand(-20.0f, 20.0f);
        }
    }
}

inline static bool feq(float a, float b) { return fabsf(a - b) < 1e-4f; }

bool matrix_equal(float *A, float *B, u32 n) {
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            float const a = A[i * n + j];
            float const b = B[i * n + j];
            if (!feq(a, b)) {
                return false;
            }
        }
    }

    return true;
}

inline static i64 time_in_ns() {
    struct timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    return 1000000000L * current.tv_sec + current.tv_nsec;
}

static u32 const REPEAT = 10;

void matrix_mul_baseline(float *C, float *A, float *B, u32 n) {
    CLEAR(C, n * n);

    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            for (u32 k = 0; k < n; k++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}

void test_one_with_report(u32 n) {
    float *A = calloc(n * n, sizeof(*A));
    float *B = calloc(n * n, sizeof(*B));
    float *C = calloc(n * n, sizeof(*C));
    float *D = calloc(n * n, sizeof(*D));

    init_random_matrix(A, n);
    init_random_matrix(B, n);

    matrix_mul_baseline(D, A, B, n);

    i64 const begin_ns = time_in_ns();
    for (u32 cnt = 0; cnt < REPEAT; cnt++) {
        matrix_mul(C, A, B, n);
    }
    i64 const end_ns = time_in_ns();

    if (!matrix_equal(C, D, n)) {
        printf("Fail: %20s\n", name);
    } else {
        // printf("Pass: %20s with %ld ns\n", name, end_ns - begin_ns);
        printf("%ld", end_ns - begin_ns);
    }
}
