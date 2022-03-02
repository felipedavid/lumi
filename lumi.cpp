#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define USER_INPUT_SIZE 256

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        fprintf(stderr, "[!] xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

void run(const char *source) {
    Lexer lex;
    lex.init(source);

    Token *tk = lex.get_next_token();
    while (tk->type) {
        printf("%d\n", tk->type);
        tk = lex.get_next_token();
    }
}

void run_file(const char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "[!] Failed while trying to open file '%s'\n", file_name);
        exit(72);
    }

    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *source = (char *) xmalloc(len);
    fread(source, len, 1, fp);
    source[len] = 0;

    run(source);
}

void run_prompt() {
    static char user_input[USER_INPUT_SIZE];

    for (;;) {
        printf(">>> ");
        if (!fgets(user_input, USER_INPUT_SIZE, stdin)) exit(0);
        run(user_input);
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        printf("Usage: lumi <script>\n");
        return 64;
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}
