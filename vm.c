#include <stdio.h>

#include "vm.h"
#include "debug.h"
#include "compiler.h"

VM vm;

void vm_init() {
    vm.stack_top = 0;
}

void vm_free() {
}

Interpret_Result vm_run() {
#define READ_CONSTANT() (vm.chunk->constants[*vm.ip++])

// Why use a while loop that dosen't loop? To make everything the
// same scope, and fix problems with the semicolon on the macro call
#define BINARY_OP(op) \
    do { \
        double y = vm_pop(); \
        double x = vm_pop(); \
        vm_push(x op y); \
    } while(false)

    for (;;) {
#ifdef DEBUG_TRACE
        printf("       ");
        for (Value *v = vm.stack; v < (vm.stack+vm.stack_top); v++) {
            printf("[ ");
            value_print(*v);
            printf(" ]");
        }
        printf("\n");
        disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif 
        switch(*vm.ip++) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            vm_push(constant);
            value_print(constant);
            printf("\n");
        } break;
        case OP_NEG: vm_push(-vm_pop()); break;
        case OP_ADD: BINARY_OP(+); break;
        case OP_SUB: BINARY_OP(-); break;
        case OP_MUL: BINARY_OP(*); break;
        case OP_DIV: BINARY_OP(/); break;
        case OP_RET: {
            value_print(vm_pop());
            printf("\n");
            return OK;
        } break;
        }
    }
#undef READ_CONSTANT
#undef BINARY_OP
}

Interpret_Result vm_interpret(const char *source) {
    Chunk chunk;
    chunk_init(&chunk);

    // Compiler the source filing up a chunk with bytecode
    if (!compiler_compile(source, &chunk)) {
        return COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = chunk.code;

    // Run the chunk on the VM
    return vm_run();
}

void vm_push(Value value) {
    buf_push(vm.stack, value);
    vm.stack_top++;
}

Value vm_pop() {
    vm.stack_top--;
    return buf_pop(vm.stack);
}
