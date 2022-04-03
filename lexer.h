#include "common.h"

typedef enum {
    TP_NUMBER = 128,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    int line;
    union {
        u64 val;
    };
} Token;

void token_next();
void lex_init(const char *source);
void lex_test();
