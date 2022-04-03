#include <stdio.h>
#include <ctype.h>

#include "lexer.h"

Token token;
char *stream;

void token_next() {
    token.start = stream;
    switch (*stream) {
    case '0' ... '9': {
        u64 val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream - '0';
            stream++;
        }
        token.val = val;
        token.type = TP_NUMBER;
    } break;
    default: {
        token.type = *stream++;
    }
    }
    token.end = stream;
}

void token_print() {
    printf("[TOKEN] ");
    switch (token.type) {
    case TP_NUMBER: 
        printf("[lexeme: \"%.*s\"] [val: %d]\n", 
            (int)(token.end - token.start), token.start, token.val);
        break;
    default:
        printf("[lexeme: \"%.*s\"]\n", 
            (int)(token.end - token.start), token.start);
    }
}

void lex_init(const char *source) {
    stream = (char *) source;
    token.line = 1;
}

void lex_test() {
    lex_init("+()1234+904");
    token_next();
    while (token.type) {
        token_print();
        token_next();
    }
}
