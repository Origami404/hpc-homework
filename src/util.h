#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)
void _Noreturn _panic(char const *file, int line, char const *fmt, ...);

#ifdef DEBUG
#   define debug(...) _debug(__FILE__, __LINE__, __VA_ARGS__) 
#   define debug_do(stmt) stmt 
#   define assert_s(stmt, prompt) do { if (stmt) ; else panic("Assert: \"" #stmt "\" fail: " prompt "."); } while (0)
#   define assert(stmt) assert_s(stmt, "<empty>")
#else
#   define debug(...) 
#   define debug_do(stmt) 
#   define assert_s(stmt, prompt)
#   define assert(stmt) 
#endif

extern void _debug(char const *file, int line, char const *fmt, ...);

static inline void* checked_malloc(size_t s) {
    void *p = malloc(s);
    if (!p) panic("Memory error");
    return p;
}

static inline void* checked_realloc(void *data, size_t s) {
    void *p = realloc(data, s);
    if (!p) panic("Memory error");
    return p;
}

static inline void *checked_calloc(size_t s) {
    void *p = checked_malloc(s);
    memset(p, 0, s);
    return p;
}

#define NELEMS(array) ((int)(sizeof(array) / sizeof(*(array))))