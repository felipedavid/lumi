#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "common.h"

enum Token_Type {
    IDENT = 256,
    KEYWORD,
    TRUE,
    FALSE,
    NIL,

    NUMBER,
    STRING,
    ERROR,

    BANG_EQUAL,
    EQUAL_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
};

struct Token {
    Token_Type type = ERROR;   

    const char *start;
    const char *end;

    union {
        u64 int_val;
        const char *name;
    } val;

    void log();
};

struct Lexer {
    Token token, prev_token;
    const char *source;

    int line = 1;

    void init(const char *source);
    Token *eat_token();
    bool is_token(Token_Type tp);
    bool match(Token_Type tp);
    bool is_keyword(const char *keyword);
    bool match_keyword(const char *keyword);
};

#endif
