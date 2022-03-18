#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);

typedef struct {
    size_t len;
    size_t cap;
    char val[0];
} Buf_Hdr;

void *buf__grow(void *buf, size_t new_len, size_t elem_size);

#define MAX(x, y) ((x > y) ? x : y)

#define buf__hdr(b) (b ? (Buf_Hdr *)((char *)b - offsetof(Buf_Hdr, val)) : NULL)
#define buf__fits(b, n) ((buf_len(b) + n) <= buf_cap(b))
#define buf__fit(b, n) (buf__fits(b, n) ? 0 : (b = buf__grow(b, buf_len(b) + n, sizeof(*b))))

#define buf_len(b) (b ? buf__hdr(b)->len : 0)
#define buf_cap(b) (b ? buf__hdr(b)->cap : 0)

#define buf_push(b, x) (buf__fit(b, 1), b[buf__hdr(b)->len++] = x)
#define buf_free(b) (b ? free(buf__hdr(b)), b = NULL : 0)

#endif
