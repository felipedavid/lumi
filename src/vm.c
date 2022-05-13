#include "common.h"
#include "vm.h"

#include <stdbool.h>

VM vm;

void chunk_init(Chunk *ck) {
    ck->code = NULL;
    ck->sdata = NULL;
}

void chunk_free(Chunk *ck) {
    buf_free(ck->code);
    buf_free(ck->sdata);
}

void chunk_push_code(Chunk *ck, u8 code) {
    buf_push(ck->code, code);
}

int chunk_push_data(Chunk *ck, Value val) {
    int i = buf_len(ck->sdata);
    buf_push(ck->sdata, val);
    return i;
}

const char *op_code_str[] = {
    [OP_RETURN]     = "OP_RETURN",
    [OP_STATIC_VAL] = "OP_STATIC_VAL",
};

bool disassemble_instruction(Chunk *ck, u8 *ip) {
    u8 *code = ck->code;
    Value *data = ck->sdata;

    printf("%d: ", i);
    switch (*ip) {
    case OP_RETURN: {
        printf("[%s]\n", op_code_str[*ip]);
    } break;
    case OP_STATIC_VAL: {
        printf("[%s] [%lf]\n", op_code_str[*ip], data[*(ip+1)]);
    } break;
    default: {
        printf("[%u] Undefined instruction\n", *code);
        return true;
    }
    }

    return false;
}

void chunk_disassemble(Chunk *ck) {
    for (int i = 0; i < buf_len(ck->code); i++) {
        if (disassemble_instruction(ck, &i)) break;
    }
}

void vm_init() {
    vm.stack = NULL;
    vm.sp = NULL;
}

inline void vm_push(Value val) {
    buf_push(vm.stack, val);
    vm.sp = vm.stack + buf_len(vm.stack);
}

inline Value vm_pop() {
    return buf_pop(vm.stack);
}

VM_Result vm_run(Chunk *chunk) {
#define READ_CONSTANT() (vm.chunk->sdata[*vm.ip++])
    vm.chunk = chunk;
    vm.ip = chunk->code;

    for (;;) {
#define DEBUG_MODE
#ifdef DEBUG_MODE 
        disassemble_instruction(chunk, vm.ip);
#endif
        switch (*vm.ip++) {
        case OP_RETURN: {
            return OK;
        } break;
        case OP_STATIC_VAL: {
            vm_push(READ_CONSTANT());
        } break;
        }
    }
    return OK;
#undef READ_CONSTANT
}
