#ifndef CHUNK_H
#define CHUNK_K

#include "common.h"

typedef enum {
    OP_RETURN,
} OpCode;

typedef struct {
    u8 *code;
} Chunk;

void chunk_init(Chunk *chunk);
void chunk_push_code(Chunk *chunk, u8 opc);
void chunk_free(Chunk *chunk);

#endif
