#include <stdio.h>

#include "common.h"
#include "debug.h"

int disassemble_instruction(Chunk chunk, int offset) {
    printf("%04d ", offset);

    u8 instruction = chunk[offset];
    switch(instruction) {
    case OP_RETURN:
        return simple_instruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}

void disassemble_chunk(Chunk chunk, const char *name) {
    printf("=== %s ===\n", name);

    for (int offset = 0; offset < buf_len(chunk);) {
        offset = disassemble_instruction(chunk, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}
