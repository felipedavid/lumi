#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

void chunk_disassemble(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < buf_len(chunk->code);) {
        offset = disassemble_instruction(chunk, offset);
    }
}

int disassemble_instruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);

    u8 instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN: return simple_instruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
