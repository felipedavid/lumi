#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "lexer.h"

void run(const char *source) {
    Lexer lex;
    lex.init(source);

    Token *tk = lex.next_token();
    while(tk->type) {
        lex.log_current_token();
        tk = lex.next_token();
    }
}

void run_file(const char *file_name) {
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        fprintf(stderr, "[!] Cannot open file '%s'\n", file_name);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    size_t file_len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *source = (char *) xmalloc(file_len + 1);
    fread(source, file_len, 1, f);
    source[file_len] = '\0';

    run(source);
}

void run_prompt() {
    char buf[256];

    for (;;) {
        printf(">>> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
            break;
        buf[strlen(buf)-1] = '\0';
        run(buf);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        run_prompt();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        printf("Usage: %s <script.lumi>\n", argv[0]);
    }
}
