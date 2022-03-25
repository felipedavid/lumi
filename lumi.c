#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char **argv) {
    buf_test();

    Chunk chunk;

    chunk_init(&chunk);
    chunk_push_code(&chunk, OP_RETURN); 

    chunk_disassemble(&chunk, "test chunk");
    chunk_free(&chunk);

    return 0;
}
