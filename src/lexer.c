#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

#include "lexer.h"

Token token;
const char *stream;
bool lex_had_error = false;

void lex_init(const char *source) {
    stream = source;
}

void lex_error(const char *fmt, ...) {
    lex_had_error = true;

    va_list args;
    va_start(args, fmt);

    printf("Syntax error at line %d: ", token.line);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}

void token_next(void) {
#define HAS_EQUALS(yes, no)\
    stream++; \
    if (*stream == '=') { \
        token.type = yes;\
        stream++;\
    } else {\
        token.type = no;\
    }\
    break;

loop:
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
        token.type = TOKEN_NUMBER;
        while (isdigit(*stream)) {
            stream++;        
        }
    } break;
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
    case 'z':
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
    case 'Z': {
        stream++;
        token.type = TOKEN_NAME;
        while (isalnum(*stream) || (*stream == '_')) {
            stream++;
        }
    } break;
    case '"': {
        stream++;
        token.type = TOKEN_STRING;
        while (*stream && *stream != '"') {
            if (*stream == '\n') {
                lex_error("String with newline character");
            }
            stream++;
        }

        if (*stream == '"') {
            stream++;
        } else {
            lex_error("Unterminated string literal");
        }
    } break;
    case '<': HAS_EQUALS(TOKEN_LTEQ, TOKEN_LT);
    case '>': HAS_EQUALS(TOKEN_GTEQ, TOKEN_GT);
    case '=': HAS_EQUALS(TOKEN_EQEQ, TOKEN_EQ);
    case ' ':
    case '\n':
    case '\r':
    case '\t':
    case '\v': {
        while(isspace(*stream)) {
            if (*stream == '\n') {
                token.line++;
            }
            stream++;
        }
        goto loop;
    }
    default: {
        token.type = *stream++;
    } break;
    }
    token.end = stream;

#undef HAS_EQUALS
}

static const char *token_type_str[] = {
    [TOKEN_NAME]   = "TOKEN_NAME",
    [TOKEN_STRING] = "TOKEN_STRING",
    [TOKEN_NUMBER] = "TOKEN_NUMBER",
    [TOKEN_LT]     = "TOKEN_LT",
    [TOKEN_GT]     = "TOKEN_GT",
    [TOKEN_EQ]     = "TOKEN_EQ",
    [TOKEN_LTEQ]   = "TOKEN_LTEQ",
    [TOKEN_GTEQ]   = "TOKEN_GTEQ",
    [TOKEN_EQEQ]   = "TOKEN_EQEQ",
};

void token_print(void) {
    printf("[%s] [lexeme: %.*s]\n", 
        token_type_str[token.type], (int)(token.end - token.start), token.start);
}
