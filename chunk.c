#include <assert.h>

#include "common.h"
#include "chunk.h"

void chunk_init(Chunk *chunk) {
    chunk->lines = NULL;
    chunk->code = NULL;
    chunk->constants = NULL;
}

void chunk_push_code(Chunk *chunk, u8 opc, int line) {
    assert(chunk != NULL);

    // Check if this opcode is in the same line as previous one
    Line *last_line;
    if (chunk->lines == NULL || 
        (last_line = &chunk->lines[buf_len(chunk->lines)-1])->line != line) {
        // If not, push a new Line struct and set count to one.
        buf_push(chunk->lines, ((Line){1, line}));
    } else {
        // If it is, just increment the counter of that line.
        last_line->count++;
    }

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

int chunk_get_line(Chunk *chunk, int index) {
    Line *lines = chunk->lines;
    for (int i = 0; i < buf_len(lines); i++) {
        index -= lines[i].count;
        if (index < 0) {
            return lines[i].line;
        }
    }
    return -1;
}
