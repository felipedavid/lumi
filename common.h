#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

void *xmalloc(size_t n_bytes);

template <class T>
struct Vector {
    T *ptr = NULL;
    int len = 0;
    int cap = 0;

    void push(T val);
    void grow(size_t new_size);
};

#endif
