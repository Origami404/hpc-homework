#include "util.h"

#define CLEAR(array, array_len) \
    memset((array), 0, sizeof(*(array)) * (array_len))

#define A(i, j) (A[(i)*n + (j)])
#define B(i, j) (B[(i)*n + (j)])
#define C(i, j) (C[(i)*n + (j)])

void matrix_mul(float *, float *, float *, u32);
extern const char name[128];

#define DECL_NAME(name_) \
    const char name[128] = name_

void test_one_with_report(u32 n);
