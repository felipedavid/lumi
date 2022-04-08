#ifndef COMPILER_H
#define COMPILER_H

#include "common.h"
#include "chunk.h"

bool compiler_compile(const char *source, Chunk *chunk);

#endif
