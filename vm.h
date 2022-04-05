#ifndef VM_H 
#define VM_H

#include "chunk.h" 
#include "value.h"

typedef struct { 
    Chunk *chunk; 
    u8 *ip; 
    Value *stack; 
    int stack_top; 
} VM;

typedef enum {
    OK,
    COMPILE_ERROR,
    RUNTIME_ERROR
} Interpret_Result;

void vm_init();
void vm_free();
Interpret_Result vm_run();
Interpret_Result vm_interpret(const char *source);
void vm_stack_reset();
void vm_push(Value value);
Value vm_pop();

#endif
