#include "utils.h"

#include <stdio.h>

typedef enum {
    ET_NONE,
} Error_Type;

Error_Type run(const char *source) {
    printf(source);
    return ET_NONE;
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        FILE *source_file = fopen(argv[1], "r");

        // Get the file size
        fseek(source_file, 0, SEEK_END);
        size_t file_size = ftell(source_file);
        fseek(source_file, 0, SEEK_SET);

        // Allocate a buffer and copy the source code to it
        char *source_code = xmalloc(file_size + 1);
        fread(source_code, file_size, 1, source_file);
        source_code[file_size] = '\0'; // Ensure zero terminate string
         
        run(source_code);
    } else if (argc == 1) { // Run prompt when no file is specified
        enum { BUF_SIZE = 2048 };
        char buf[BUF_SIZE];
        for (;;) {
            printf(">>> ");
            if (fgets(buf, BUF_SIZE, stdin) == NULL) {
                printf("Exiting...");
                return -1;
            }
            buf[strlen(buf)-1] = '\0'; // Get rid of new line
            run(buf);
        }
    } else {
        return 1;
    }
    return 0;
}
