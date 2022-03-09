#include "common.h"

enum Token_Type {
    IDENT = 256,
    NUMBER,
    STRING,
    ERROR,

    BANG_EQUAL,
    EQUAL_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
};

struct Token {
    Token_Type type = TOKEN_ERROR;   

    const char *start;
    const char *end;

    union {
        u64 int_val;
        const char *name;
    } val;

    void log();
};

struct Lexer {
    Token token;
    const char *source;

    int line = 1;

    void init(const char *source);
    Token *eat_token();
    bool is_token(Token_Type tp);
}
