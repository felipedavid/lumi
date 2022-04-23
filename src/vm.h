#ifndef VM_H
#define VM_H

#include "common.h"

typedef double Value;

typedef enum {
    OP_RETURN,
    OP_STATIC_VAL,
} Op_Code;

typedef struct {
    u8 *code;
    Value *sdata; // Static data
} Chunk;

typedef struct {
    u8 *stack;
} VM;

void chunk_init(Chunk *ck);
void chunk_free(Chunk *ck);
void chunk_push_code(Chunk *ck, u8 code);
void chunk_push_data(Chunk *ck, Value val);
void chunk_disassemble(Chunk *ck);
void vm_init();
void vm_run();


#endif
