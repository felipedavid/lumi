#include "common.h"
#include "vm.h"

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
    ck++;
}

void chunk_push_data(Chunk *ck, Value val) {
    ck++;
}

const char *op_code_str[] = {
    [OP_RETURN]     = "OP_RETURN",
    [OP_STATIC_VAL] = "OP_STATIC_VAL",
};

void chunk_disassemble(Chunk *ck) {
    u8 *code = ck->code;
    Value *data = ck->sdata;

    for (int i = 0; i < buf_len(code); i++) {
        switch (code[i]) {
        case OP_RETURN: {
            printf("[%s]\n", op_code_str[i]);
        } break;
        case OP_STATIC_VAL: {
            printf("[%s]\n", op_code_str[i], data[code[i+1]]);
            i++;
        } break;
        default:
            printf("[%u] Undefined instruction\n", *code);
            break;
        }
    }
}

void vm_init() {
}

void vm_run() {
}
