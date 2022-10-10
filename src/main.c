#include "util.h"
#include <stdio.h>

float float_rand(float min, float max) {
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

void init_random_matrix(float *m, u32 n) {
    for (u32 i = 0; i < n; i++) {
        for (u32 j = 0; j < n; j++) {
            m[i * n + j] = float_rand(-20.0f, 20.0f);
        }
    }
}

// return time in ns
u32 test(u32 n) {
    return 0;
}

int main(int argc, char** argv) {
    printf("hello world!\n");
    return 0;
}
