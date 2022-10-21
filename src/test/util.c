#include "util.h"

void _debug(char const *file, int line, char const *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);

    fprintf(stderr, "Debug at %s:%d: ", file, line);
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");

    va_end(argp);
}

void _panic(char const *file, int line, char const *fmt, ...) {
    va_list argp;
    va_start(argp, fmt);

    fprintf(stderr, "Panic at %s:%d: ", file, line);
    vfprintf(stderr, fmt, argp);
    fprintf(stderr, "\n");

    va_end(argp);
    exit(-1);
}