#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, char **argv) {
    Chunk chunk = NULL;

    buf_push(chunk, OP_RETURN);
    disassemble_chunk(chunk, "test chunk");
    buf_free(chunk);

    return 0;
}
