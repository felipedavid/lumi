#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, char **argv) {
#ifdef DEBUG_TESTS
    buf_test();
#endif

    Chunk chunk;

    chunk_init(&chunk);
    int constant = chunk_push_constant(&chunk, 1.2); 
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant, 123);
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant, 123);

    chunk_push_code(&chunk, OP_RETURN, 123);
    chunk_push_code(&chunk, OP_RETURN, 23);
    chunk_push_code(&chunk, OP_RETURN, 24);
    chunk_push_code(&chunk, OP_RETURN, 23);
    chunk_push_code(&chunk, OP_RETURN, 23);

    vm_init();

    vm_interpret(&chunk);

    vm_free();
    chunk_free(&chunk);

    return 0;
}
