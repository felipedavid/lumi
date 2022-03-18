#include "common.h"

void *buf__grow(void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = MAX(new_len, buf_cap(buf) * 2 + 1);
    size_t new_size = offsetof(Buf_Hdr, val) + (new_cap * elem_size);

    Buf_Hdr *new_buf = NULL;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->val;
}

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
