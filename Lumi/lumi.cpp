#include <cstdio>
#include <cstdlib>
#include <vector>

#include "scanner.h"

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        perror("[!] xmalloc failed!");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *prev_ptr, size_t n_bytes) {
    void *ptr = realloc(prev_ptr, n_bytes);
    if (!ptr) {
        perror("[!] xrealloc failed!");
        exit(1);
    }
    return ptr;
}

void run(const char *source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();

    for (int i = 0; i < tokens.size(); i++) {
        printf("Token: %.*s\n", (int)(tokens[i].lexeme_end - tokens[i].lexeme_start), tokens[i].lexeme_start);
    }
}

void run_file(const char *file_name) {
    FILE *fp;
    fopen_s(&fp, file_name, "r");
    if (!fp) {
        perror("[!] Can't open file.\n");
        exit(1);
    }

    size_t f_size;
    fseek(fp, 0L, SEEK_END);
    f_size = ftell(fp);
    rewind(fp);

    char *source = (char *) xmalloc(f_size + 1);
    fread(source, f_size, 1, fp);
    source[f_size] = 0; // Make sure is a zero terminated string

    run(source);
}

const char* get_user_input() {
    static size_t cap = 256;
    static char *input = (char *) xmalloc(cap);

    char c;
    size_t len = 0;
    while ((c = getchar()) != '\n') {
        if (len > cap) input = (char *) xrealloc(input, (cap *= 2));
        input[len++] = c;
    }
    input[len] = 0;

    return input;
}

void run_prompt() {
    for (;;) {
        printf("> ");
        const char *source = get_user_input();
        if (source == NULL) break;
        run(source);
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        printf("Usage: %s <script>\n", argv[0]);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}
