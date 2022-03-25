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
    if (offset > 0 && 
        chunk_get_line(chunk, offset) == chunk_get_line(chunk, offset - 1)) {
        printf("   | ");
    } else {
        printf("%4d ", chunk_get_line(chunk, offset));
    }

    u8 instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT: return constant_instruction("OP_CONSTANT", chunk, offset);
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

static int constant_instruction(const char *name, Chunk *chunk, int offset) {
    u8 constant_index = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant_index);
    value_print(chunk->constants[constant_index]);
    printf("\n");
    return offset + 2;
}
