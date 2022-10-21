#include <pthread.h>
#include "matrix_macros.h"

DECL_NAME("02_multi_thread");

#define THREAD_COUNT 16

struct thread_info {
    float *A;
    float *B;
    float *C;
    u32 n;
    u32 i_beg;
    u32 i_end;
};

void *worker(void *);

void matrix_mul(float *C, float *A, float *B, u32 n) {
    CLEAR(C, n * n);
    
    struct thread_info * infos = calloc(THREAD_COUNT, sizeof(*infos));

    u32 const slice_len = n / THREAD_COUNT;
    u32 const slice_rest = n % THREAD_COUNT;

    for (u32 i = 0; i < THREAD_COUNT; i++) {
        infos[i] = (struct thread_info) {
            A, B, C, n, i * slice_len, (i + 1) * slice_len 
        };
    }
    infos[THREAD_COUNT - 1].i_end += slice_rest;

    pthread_t thread_ids[THREAD_COUNT];
    for (u32 i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(thread_ids + i, 0, worker, infos + i) != 0) {
            perror("create thread failed");
        }
    }

    for (u32 i = 0; i < THREAD_COUNT; i++) {
        pthread_join(thread_ids[i], 0);
    }
}

#define W(m, i, j) \
    (info.m[(i) * info.n + (j)])

void *worker(void *arg) {
    struct thread_info info = *(struct thread_info *)arg;
    
    for (u32 i = info.i_beg; i < info.i_end; i++) {
        for (u32 k = 0; k < info.n; k++) {
            for (u32 j = 0; j < info.n; j++) {
                W(C, i, j) += W(A, i, k) * W(B, k, j);
            }
        }
    }

    return 0;
}

