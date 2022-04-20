#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_NUMBER = 128,
    TOKEN_STRING,
    TOKEN_NAME,

    TOKEN_LT,
    TOKEN_GT,
    TOKEN_EQ,
    TOKEN_LTEQ,
    TOKEN_GTEQ,
    TOKEN_EQEQ,
} Token_Type;

typedef struct {
    Token_Type type;
    int line;
    const char *start;
    const char *end;
    union {
        int int_val;
        double float_val;
    };
} Token;

void lex_init(const char *source);
void token_next(void);
void token_print(void);

#endif
