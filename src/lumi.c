#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "common.h"
#include "lexer.h"
#include "test.h"

extern Token token;

void run(const char *source) {
    //lex_init(source);
    //token_next();
    //while (token.type) {
    //    token_print();
    //    token_next();
    //}
    Chunk chunk;
    chunk_init(&chunk);
    chunk_push_code(&chunk, (u8)OP_STATIC_VAL);
    u8 index = chunk_push_data(&chunk, 127);
    chunk_push_code(&chunk, (u8)index);
    chunk_push_code(&chunk, (u8)OP_RETURN);
    chunk_disassemble(&chunk);
}

void run_file(const char *file_name) {
    // Open the file
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        fprintf(stderr, "[!] Cannot open file \"%s\"\n", file_name);
        exit(1);
    }

    // Get file size
    fseek(f, 0L, SEEK_END);
    size_t f_size = ftell(f);
    rewind(f);

    // Reading the file contents in a buffer.
    char *src = xmalloc(f_size);
    fread(src, f_size, 1, f);
    src[f_size] = '\0'; // Make sure we have a zero-terminated string

    run(src);
}

void run_repl(void) {
    char input[512];

    for (;;) {
        printf(">>> ");
        if (fgets(input, 512, stdin) == NULL) break;
        input[strlen(input)-1] = '\0';
        run(input);
    }
}

int main(int argc, char **argv) {
    buf_test();
    intern_str_test();
    lex_test();

    if (argc == 1) {
        run_repl();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "[!] Usage: %s <script>\n", argv[0]);
    }
    return 0;
}
