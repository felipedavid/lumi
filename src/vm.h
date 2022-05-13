#ifndef VM_H
#define VM_H

#include "common.h"

typedef double Value;

// The operations that form the architecture of our fictional chip
typedef enum {
    OP_RETURN,
    OP_STATIC_VAL,
} Op_Code;

// A chunk is just a sequence of machine code and related data
// that will be ran in our Virtual Machine
typedef struct {
    u8 *code;
    Value *sdata; // Static data
} Chunk;

typedef enum {
    OK,
} VM_Result;

typedef struct {
    Value *stack;
    Chunk *chunk;
    u8 *ip; // Points to the currently executing instruction
    Value *sp; // Points to the top of the stack
} VM;

void chunk_init(Chunk *ck);
void chunk_free(Chunk *ck);
void chunk_push_code(Chunk *ck, u8 code);
int chunk_push_data(Chunk *ck, Value val);
void chunk_disassemble(Chunk *ck);
void vm_init();
void vm_push(Value val);
Value vm_pop();
VM_Result vm_run(Chunk *chunk);

#endif
