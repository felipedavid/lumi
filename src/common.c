#include "common.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xrealloc failed\n");
        exit(1);
    }
    return ptr;
}

/* Stretchy bufers */
typedef struct {
    size_t len;
    size_t cap;
    char buf[];
} Buf_Hdr;

// Yeah, these macros are kinda ugly, but they give us a very
// nice dynamic array implementation for any type.
#define buf__hdr(b) ((b) ? ((Buf_Hdr *) ((char*)b - offsetof(Buf_Hdr, buf))) : NULL)
#define buf__fit(b, n) ((n <= buf_cap(b)) ? 0 : (b = buf__grow((b), (n), sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->len : 0)

#define buf_push(b, x) (buf__fit(b, buf_len(b)+1), b[buf__hdr(b)->len++] = x) 

void *but__grow(void *ptr, size_t min_cap, size_t elem_size) {
    size_t new_cap = MAX(min_cap, buf_cap(ptr)*2+1);
    size_t new_size = offsetof(Buf_Hdr, buf) + (new_cap * elem_size);

    Buf_Hdr *new_buf = NULL;
    if (ptr) {
        new_buf = xrealloc(buf__hdr(ptr), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->buf;
}