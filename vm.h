#ifndef VM_H
#define VM_H

#include "chunk.h"

typedef struct {
    Chunk *chunk;
    u8 *ip;
    Value *stack;
    Value *stack_top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} Interpret_Result;

void vm_init();
void vm_free();
Interpret_Result vm_run();
Interpret_Result vm_interpret(Chunk *chunk);

#endif
