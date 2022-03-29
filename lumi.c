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
    int constant_index = chunk_push_constant(&chunk, 1.2); 
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant_index, 123);

    constant_index = chunk_push_constant(&chunk, 3.4);
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant_index, 123);

    chunk_push_code(&chunk, OP_ADD, 123);

    constant_index = chunk_push_constant(&chunk, 5.6);
    chunk_push_code(&chunk, OP_CONSTANT, 123);
    chunk_push_code(&chunk, constant_index, 123);

    chunk_push_code(&chunk, OP_DIV, 123);
    chunk_push_code(&chunk, OP_NEG, 123);
    chunk_push_code(&chunk, OP_RET, 123);

    vm_init();

    vm_interpret(&chunk);

    vm_free();
    chunk_free(&chunk);

    return 0;
}
