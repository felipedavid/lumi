#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int line;
} Line;

typedef struct {
    Line *lines;
    u8 *code;
    Value *constants;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_push_code(Chunk *chunk, u8 opc, int line);
int chunk_push_constant(Chunk *chunk, Value constant);
void chunk_free(Chunk *chunk);
int chunk_get_line(Chunk *chunk, int index);

#endif
