#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef uint8_t u8;

typedef struct {
    size_t len;
    size_t cap;
    char val[0];
} Buf_Hdr;

void buf_test();

#endif
