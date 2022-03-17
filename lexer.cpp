#include <ctype.h>
#include <stdio.h>

#include "common.h"
#include "lexer.h"

void Lexer::init(const char *source) {
    this->source = source;
}

Token *Lexer::next_token() {
    token.start = source;

    switch (*source) {
    case '0' ... '9': {
        float val = 0;
        while (isdigit(*source)) {
            val *= 10;
            val += *source++ - '0';
        }
        token.type = TOKEN_NUMBER;
        token.val = val;
    } break;
    case 'a' ... 'z':
    case 'A' ... 'Z': {
        while (isalnum(*source) || *source == '_') {
            source++;
        }
        token.type = TOKEN_NAME;
        token.str = intern_str_range(token.start, source);
    } break;
    default:
        token.type = (Token_Type) *source++;
    }

    token.end = source;
    return &token;
}

void Lexer::log_current_token() {
    switch (token.type) {
    case TOKEN_NUMBER:
        printf("[Token] Type: TOKEN_NUMBER, Val: %f\n", token.val);
        break;
    case TOKEN_NAME:
        printf("[Token] Type: TOKEN_NAME, Val: %.*s\n", (token.end - token.start), token.start);
        break;
    }
}
