#include "util.h"

#define CLEAR(array, array_len) \
    memset((array), 0, sizeof(*(array)) * (array_len))

#define A(i, j) (A[(i)*n + (j)])
#define B(i, j) (B[(i)*n + (j)])
#define C(i, j) (C[(i)*n + (j)])
