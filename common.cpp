#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"

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

// Dynamic Arrays
template <typename T>
void Vector<T>::push(T val) {
    if (len == cap) {
        size_t new_cap;
        if (arr) {
            new_cap = cap * 2;
            arr = (T *) xrealloc(arr, new_cap * sizeof(T));
        } else {
            new_cap = 10;
            arr = (T *) xmalloc(new_cap * sizeof(T));
        }
        cap = new_cap;
    } 
    arr[len++] = val;
}

template <typename T>
T Vector<T>::get(int index) {
    return arr[index];
}

// String interning

struct Intern_Str {
    const char *str;
    size_t len;
};
Vector<Intern_Str> interned_strs;

const char *intern_str_range(const char *start, const char *end) {
    size_t len = end - start;
    Intern_Str i_str;
    for (int i = 0; i < interned_strs.len; i++) {
        i_str = interned_strs.get(i);
        if (i_str.len == len && !strncmp(start, i_str.str, len)) {
            return i_str.str;
        }
    }

    char *new_str = (char *) xmalloc(len + 1);
    memcpy(new_str, start, len);
    new_str[len] = 0;
    interned_strs.push(Intern_Str{new_str, len});

    return new_str;
}
