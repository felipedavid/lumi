#ifndef CHUNK_H
#define CHUNK_K

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int *lines;
    u8 *code;
    Value *constants;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_push_code(Chunk *chunk, u8 opc, int line);
int chunk_push_constant(Chunk *chunk, Value constant);
void chunk_free(Chunk *chunk);

#endif
