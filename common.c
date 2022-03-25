#include <assert.h>
#include <stdio.h>

#include "common.h"

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xmalloc failed!\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xrealloc failed!\n");
        exit(1);
    }
    return ptr;
}


void *buf__grow(void *buf, size_t min_cap, size_t elmen_size) {
    size_t new_cap = MAX(min_cap, buf_cap(buf)*2+1);
    size_t new_size = offsetof(Buf_Hdr, val) + (new_cap * elmen_size);

    Buf_Hdr *new_buf;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->val;
}

void buf_test() {
    int *buf = NULL;
    assert(buf_len(buf) == 0);
    int n_elements = 1024;

    for (int i = 0; i < n_elements; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == n_elements);

    for (int i = 0; i < n_elements; i++) {
        assert(buf[i] == i);
    }

    buf_free(buf);
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);
    assert(buf == NULL);
}
