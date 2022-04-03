#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "lexer.h"

Interpret_Result run(const char *source) {
    tokens_print(source);
    return INTERPRET_OK;
}

// NOTE: The source file read by this function is heap allocated
// and will not be freed. I'm letting the OS take care of that
// when the process termites. If your PC can't handle all source
// in main memory, just download more RAM :P
void run_file(const char *file_name) {
    // Open file
    FILE *f = fopen(file_name, "r");
    if (f == NULL) {
        fprintf(stderr, "[!] Unable to open file '%s'\n", file_name);
        exit(EX_IOERR);
    }

    // Get file size
    fseek(f, 0L, SEEK_END);
    size_t file_size = ftell(f);
    rewind(f);

    // Read file contents
    char *source = (char *) xmalloc(file_size);
    fread(source, file_size, 1, f); 
    fclose(f);
    source[file_size] = '\0'; // Zero terminates string buffer (overwriting EOF)

    Interpret_Result res = run(source);
    switch(res) {
    case INTERPRET_COMPILE_ERROR: exit(EX_DATAERR);
    case INTERPRET_RUNTIME_ERROR: exit(EX_SOFTWARE);
    }
}

void run_prompt() {
#define INPUT_SIZE 512
    char input[INPUT_SIZE];

    for (;;) {
        printf(">>> ");

        if (fgets(input, INPUT_SIZE, stdin) != input) break;
        input[strlen(input)-1] = '\0'; // Getting rid of newline

        run(input);
        printf("\n");
    }
#undef INPUT_SIZE
}

int main(int argc, char **argv) {
#ifdef DEBUG_TESTS
    buf_test();
    lex_test();
#endif
    vm_init();

    if (argc == 1) {
        run_prompt();
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        fprintf(stderr, "[!] Usage: lumi <script>\n");
        return EX_USAGE;
    }

    vm_free();
    return 0;
}
