#include "matrix_macros.h"
#include <mpi.h>

DECL_NAME("04_mpi");

void before_all() {
    MPI_Init(0, 0);
}

void after_all() {
    MPI_Finalize();
}

void matrix_mul(float *C, float *A, float *B, u32 n) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello world from process %d of %d\n", rank, size);
}