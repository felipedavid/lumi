#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

void *xmalloc(size_t n_bytes);
void *xrealloc(void *ptr, size_t n_bytes);
const char *str_intern(const char *start, const char *end);

#endif
