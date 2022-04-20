#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);

#endif
