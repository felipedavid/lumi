#ifndef LEXER_H
#define LEXER_H

#include "common.h"

typedef enum {
    TOKEN_NUMBER = 128,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_EOF,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    int line;
    union {
        u64 val;
        const char *name;
    };
} Token;

void token_next();
void tokens_print();
void lex_init(const char *source);
void lex_test();

#endif
