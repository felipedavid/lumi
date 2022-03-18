#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

void disassemble_chunk(Chunk chunk, const char *name);
int disassemble_intruction(Chunk chunk, int offset);
static int simple_instruction(const char *name, int offset);

#endif
