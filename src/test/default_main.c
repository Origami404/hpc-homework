#include "matrix_macros.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        return 1;
    }

    before_all();
    test_one_with_report(atoi(argv[1]));
    after_all();
}