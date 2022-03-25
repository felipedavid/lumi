#include "common.h"
#include "chunk.h"

void chunk_init(Chunk *chunk) {
    chunk->lines = NULL;
    chunk->code = NULL;
    chunk->constants = NULL;
}

void chunk_push_code(Chunk *chunk, u8 opc, int line) {
    buf_push(chunk->lines, line);
    buf_push(chunk->code, opc);
}

int chunk_push_constant(Chunk *chunk, Value constant) {
    buf_push(chunk->constants, constant);
    return buf_len(chunk->constants) - 1;
}

void chunk_free(Chunk *chunk) {
    buf_free(chunk->lines);
    buf_free(chunk->code);
    buf_free(chunk->constants);
}
