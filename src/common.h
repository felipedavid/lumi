#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;

// Stretchy buffers
typedef struct {
    size_t len;
    size_t cap;
    char buf[];
} Buf_Hdr;

void *buf__grow(void *ptr, size_t min_cap, size_t elem_size);

#define buf__hdr(b) ((b) ? ((Buf_Hdr *) ((char*)b - offsetof(Buf_Hdr, buf))) : NULL)
#define buf__fit(b, n) ((n <= buf_cap(b)) ? 0 : ((b) = buf__grow((b), (n), sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->len : 0)

#define buf_push(b, x) (buf__fit(b, buf_len(b)+1), (b)[buf__hdr(b)->len++] = (x)) 
#define buf_free(b) (free(buf__hdr(b)), b = NULL)

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);

// String interning
typedef struct {
    const char *str;
    size_t len;
} Interned_Str;

const char *str_intern_range(const char *start, const char *end);
const char *str_intern(const char *str);

#endif
