#include <stdlib.h>
#include <stdio.h>

#include "common.h"

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

// Dynamic Arrays
template <typename T>
void Vector::push(T val) {
     
}

template <typename T>
T Vector::get(int index) {
    return arr[index];
}

// String interning

struct Intern_Str {
    const char *str;
    size_t len;
};
Intern_Str *interned_strs;
Vector<Intern_Str> interned_strs;

const char *intern_str_range(const char *start, const char *end) {
    size_t len = end - start;
    Intern_Str i_str;
    for (int i = 0; i < interned_strs.len(); i++) {
        i_str = internd_strs.get(i);
        if (i_str.len == len && !strncmp(start, i_str.str, len)) {
            return i_str.str;
        }
    }

    char *new_str = xmalloc(len + 1);
    memcpy(new_str, start, len);
    new_str[len] = 0;
    interned_strs.push(Intern_Str{new_str, len});

    return new_str;
}
