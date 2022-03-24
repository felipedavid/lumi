#include <assert.h>
#include <stdlib.h>

#include "common.h"

#define buf__hdr(b) ((b) ? (Buf_Hdr *) ((char *)b - offsetof(Buf_Hdr, val)) : NULL)
#define buf__fit(b, n) (((buf_len(b)+n) <= buf_cap(b)) ? 0 : ((b) = buf__grow((b), buf_len(b)+n, sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)

#define buf_push(b, n) (buf__fit(b, 1), b[buf__hdr(b)->len++] = n)
#define buf_free(b) ((b) ? free(buf__hdr(b)), (b) = NULL : 0 )

void *buf__grow(void *buf, size_t min_cap, size_t elmen_size) {
    size_t new_cap = MAX(min_cap, buf_cap(buf)*2+1);
    size_t new_size = offsetof(Buf_Hdr, val) + (new_cap * elmen_size);

    Buf_Hdr *new_buf;
    if (buf) {
        new_buf = realloc(buf__hdr(buf), new_size);
    } else {
        new_buf = malloc(new_size);
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
