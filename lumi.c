#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

void chunk_test() {
    Chunk chunk;

    chunk_init(&chunk);
    int constant = chunk_push_constant(&chunk, 1.2); 
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant, 123);

    chunk_push_code(&chunk, OP_RETURN, 123);
    chunk_push_code(&chunk, OP_RETURN, 23);
    chunk_push_code(&chunk, OP_RETURN, 24);
    chunk_push_code(&chunk, OP_RETURN, 23);
    chunk_push_code(&chunk, OP_RETURN, 23);

    chunk_disassemble(&chunk, "test chunk");

    chunk_free(&chunk);
}

int main(int argc, char **argv) {
    buf_test();
    chunk_test();

    return 0;
}
