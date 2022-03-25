#include "common.h"
#include "chunk.h"

void chunk_init(Chunk *chunk) {
    chunk->code = NULL;
}

void chunk_push_code(Chunk *chunk, u8 opc) {
    buf_push(chunk->code, opc);
}

void chunk_free(Chunk *chunk) {
    buf_free(chunk->code);
}
