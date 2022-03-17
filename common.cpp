#include <stdio.h>
#include <string.h>

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
        ptr = (T*) xrealloc(ptr, n_bytes);
    } else {
        ptr = (T*) malloc(n_bytes);
        len = 0;
    }
    cap = new_cap;
}

template <class T>
void Vector<T>::push(T val) {
    if (len+1 > cap) grow(len+1);
    ptr[len++] = val;
}

template <class T>
T Vector<T>::get(int pos) {
    return ptr[pos];
}

struct Intern_Str {
    const char *str;
    size_t len;
};

Vector<Intern_Str> interns;

// TODO: Reimplement with a hashtable
const char *str_intern_range(const char *start, const char *end) {
    size_t len = (size_t) (end - start);
    for (int i = 0; i < interns.len; i++) {
        if (interns.get(i).len == len && !strncmp(start, interns.get(i).str, len)) {
            return interns.get(i).str;
        }
    }

    char *str = (char *) xmalloc(len+1);
    memcpy(str, start, len);
    str[len] = 0;

    interns.push(Intern_Str{str, len});

    return str;
}

const char *str_intern(const char *str) {
    return str_intern_range(str, str + strlen(str));
}
