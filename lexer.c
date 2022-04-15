#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#include "lexer.h"

Token token;
char *stream;

void token_next() {
START_OVER:
    token.start = stream;
    switch (*stream) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        u64 val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream - '0';
            stream++;
        }
        token.val = val;
        token.type = TOKEN_NUMBER;
    } break;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z': {
        while (isalnum(*stream) || *stream == '_') stream++;
        token.type = TOKEN_IDENTIFIER;
        token.name = str_intern_range(token.start, stream);
    } break;
    case '"': {
        stream++;
        while (*stream != '"') {
            if (*stream == '\n') token.line++;
            stream++;
        };
        stream++;
        token.type = TOKEN_STRING;
    } break;
    case ' ': case '\t': case '\r': case '\n':
        while (isspace(*stream)) {
            if (*stream == '\n') token.line++;
            stream++;
        }
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
        printf("[line: %d] [type: TOKEN_NUMBER] [lexeme: \"%.*s\"] [val: %llu]\n", 
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
    token_next();
}

void lex_test() {
#define TEST_TYPE(t) assert(token.type == (t)); token_next()
    const char *source = "+(32)\n\n\"hello\n\"*5";
    lex_init(source);

    TEST_TYPE('+');
    TEST_TYPE('(');
    TEST_TYPE(TOKEN_NUMBER);
    TEST_TYPE(')');
    TEST_TYPE(TOKEN_STRING);
    TEST_TYPE('*');
    TEST_TYPE(TOKEN_NUMBER);
    assert(token.line == 4);
#undef TEST_TYPE
}
