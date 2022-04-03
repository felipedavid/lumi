#include <stdio.h>
#include <ctype.h>

#include "lexer.h"

Token token;
char *stream;

void token_next() {
START_OVER:
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
        token.type = TOKEN_NUMBER;
    } break;
    case 'A' ... 'Z': 
    case 'a' ... 'z': {
        while (isalnum(*stream) || *stream == '_') {
            stream++;
        }
        token.type = TOKEN_IDENTIFIER;
        token.name = str_intern_range(token.start, stream);
    } break;
    case '"': {
        while (*++stream != '"');
        stream++;
        token.type = TOKEN_STRING;
    } break;
    case ' ': case '\t': case '\r': case '\n':
        while (isspace(*++stream));
        goto START_OVER;
    default: {
        token.type = *stream++;
    }
    }
    token.end = stream;
}

void token_print() {
    switch (token.type) {
    case TOKEN_NUMBER: 
        printf("[line: %d] [type: TOKEN_NUMBER] [lexeme: \"%.*s\"] [val: %d]\n", 
            token.line, (int)(token.end - token.start), token.start, token.val);
        break;
    case TOKEN_IDENTIFIER:
        printf("[line: %d] [type: TOKEN_IDENTIFIER] [lexeme: \"%.*s\"]\n", 
            token.line, (int)(token.end - token.start), token.start);
        break;
    case TOKEN_STRING:
        printf("[line: %d] [type: TOKEN_STRING] [lexeme: \"%.*s\"]\n", 
            token.line, (int)(token.end - token.start), token.start);
        break;
    default:
        printf("[line: %d] [lexeme: \"%.*s\"]\n", 
            token.line, (int)(token.end - token.start), token.start);
    }
}

void tokens_print(const char *source) {
    lex_init(source);
    printf("[Source: %s]\n\n", source);
    printf("[TOKENS]\n");
    token_next();
    while (token.type) {
        token_print();
        token_next();
    }
}

void lex_init(const char *source) {
    stream = (char *) source;
    token.line = 1;
}

void lex_test() {
    const char *source = "\"uehue\" 4+2 \"haha \" 12";
    tokens_print(source);
}
