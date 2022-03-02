#include <stdlib.h>
#include <stdio.h>

#include "common.h"

#define MAX(x, y) ((x > y) ? x : y)

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        fprintf(stderr, "[!] xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        fprintf(stderr, "[!] xrealloc failed\n");
        exit(1);
    }
    return ptr;
}

template <class T>
void Vector<T>::grow(size_t new_size) {
    size_t new_cap = MAX(new_size, cap*2+1);
    size_t n_bytes = new_cap * sizeof(T);

    if (ptr) {
        ptr = xrealloc(ptr, n_bytes);
    } else {
        ptr = malloc(n_bytes);
        len = 0;
    }
    cap = new_cap;
}

template <class T>
void Vector<T>::push(T val) {
    if (len+1 > cap) grow(len+1);
    ptr[len++] = val;
}
