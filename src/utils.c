#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] xmalloc failed\n");
        exit(-1);
    }
    return ptr;
}
