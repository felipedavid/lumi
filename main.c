#include <stdio.h>
#include <stdlib.h>

void run(const char *source) {
    printf("Running: %s", source);
}

void run_file(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        perror("unable to open file");
        return;
    }

    fseek(fp, 0, SEEK_END);
    size_t f_size = ftell(fp);
    rewind(fp);

    char *buf = malloc(f_size + 1);

    size_t read_size = fread(buf, 1, f_size, fp);
    if (read_size != f_size) {
        perror("unable to read from file");
        return;
    }
    buf[f_size] = '\0';

    run(buf);
}

void run_prompt() {
    static char buf[1024];
    for (;;) {
        printf("> ");
        char *s = fgets(buf, sizeof(buf), stdin);
        if (!s) {
            perror("fgets");
            return;
        }

        run(buf);
    }
}

int main(int argc, const char **argv) {
    if (argc > 2) {
        printf("Usage: ./lumi <script>");
        exit(64);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
}
