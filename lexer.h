#include "common.h"

enum Token_Type {
    TOKEN_IDENT = 256,
    TOKEN_NUMBER,
    TOKEN_ERROR,
	TOKEN_BANG_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_LESS_EQUAL,
	TOKEN_GREATER_EQUAL,
};

struct Token {
    Token_Type type = TOKEN_ERROR;   

    const char *start;
    const char *end;

    union {
        u64 int_val;
    };

    void log();
};

struct Lexer {
    Token curr_tk;
    const char *source;

	int line = 0;

    void init(const char *source);
    Token *get_next_token();
};
