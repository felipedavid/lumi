#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);

// Stretchy buffers
typedef struct {
    size_t len;
    size_t cap;
    char val[0];
} Buf_Hdr;

#define buf__hdr(b) ((b) ? (Buf_Hdr *) ((char *)b - offsetof(Buf_Hdr, val)) : NULL)
#define buf__fit(b, n) (((buf_len(b)+n) <= buf_cap(b)) ? 0 : \
    ((b) = buf__grow((b), buf_len(b)+n, sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)

#define buf_push(b, n) (buf__fit((b), 1), (b)[buf__hdr(b)->len++] = (n))
#define buf_pop(b) ((buf_len(b) > 0) ? (b)[--buf__hdr(b)->len] : 0)
#define buf_free(b) ((b) ? free(buf__hdr(b)), (b) = NULL : 0)

void *buf__grow(void *buf, size_t min_cap, size_t elmen_size);
void buf_test();

// String interning
const char *str_intern_range(const char *start, const char *end);
const char *str_intern(const char *str);
void str_intern_test();

#endif
