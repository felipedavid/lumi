#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TK_NUMBER = 128,
    TK_STRING,
    TK_NAME,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
} Token;

void lex_init(const char *source);
void token_next(void);
void token_print(void);

#endif
