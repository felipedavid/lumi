#include <stdio.h>

#include "vm.h"
#include "debug.h"

VM vm;

void vm_init() {
}

void vm_free() {
}

Interpret_Result vm_run() {
#define READ_CONSTANT() (vm.chunk->constants[*vm.ip++])
    for (;;) {
#ifdef DEBUG_TRACE
        disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif 
        switch(*vm.ip++) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            value_print(constant);
            printf("\n");
        } break;
        case OP_RETURN: return INTERPRET_OK;
        }
    }
#undef READ_CONSTANT
}

Interpret_Result vm_interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = chunk->code;
    return vm_run();
}
