#pragma once

#include "util.h"

typedef void (*matrix_mul_op_t)(float *, float *, float *, u32);

void matrix_mul_baseline(float *C, float *A, float *B, u32 n);
void matrix_mul_adjust_loop_order(float *C, float *A, float *B, u32 n);

matrix_mul_op_t matrix_ops[] = {
    matrix_mul_baseline,
    matrix_mul_adjust_loop_order,
};

char const *matrix_op_name[] = {
    "baseline",
    "adjust_loop_order",
};