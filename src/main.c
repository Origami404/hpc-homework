#include <math.h>
#include <time.h>
#include <unistd.h>

#include "matrix.inl.h"

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

// return time in ns
i64 test_one(u32 n, matrix_mul_op_t op) {
    float *A = calloc(n * n, sizeof(*A));
    float *B = calloc(n * n, sizeof(*B));
    float *C = calloc(n * n, sizeof(*C));
    float *D = calloc(n * n, sizeof(*D));

    init_random_matrix(A, n);
    init_random_matrix(B, n);

    matrix_mul_baseline(D, A, B, n);

    i64 const begin_ns = time_in_ns();
    for (u32 cnt = 0; cnt < REPEAT; cnt++) {
        op(C, A, B, n);
    }
    i64 const end_ns = time_in_ns();

    if (!matrix_equal(C, D, n)) {
        return -1;
    } else {
        return end_ns - begin_ns;
    }
}

void test_with_report(u32 n) {
    u32 const op_cnt = sizeof(matrix_ops) / sizeof(*matrix_ops);
    for (u32 i = 0; i < op_cnt; i++) {
        i64 const duration = test_one(n, matrix_ops[i]);
        
        if (duration < 0) {
            printf("Fail: %20s\n", matrix_op_name[i]);
        } else {
            printf("Pass: %20s with %ld ns\n", matrix_op_name[i], duration);
        }
    }
}
