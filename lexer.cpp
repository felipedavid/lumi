#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"

const char *keyword_and;
const char *keyword_class;
const char *keyword_else;
const char *keyword_false;
const char *keyword_for;
const char *keyword_fun;
const char *keyword_if;
const char *keyword_nil;
const char *keyword_or;
const char *keyword_print;
const char *keyword_return;
const char *keyword_super;
const char *keyword_this;
const char *keyword_true;
const char *keyword_var;
const char *keyword_while;

void init_keywords() {
    keyword_and    = str_intern("and");
    keyword_class  = str_intern("class");
    keyword_else   = str_intern("else");
    keyword_false  = str_intern("false");
    keyword_for    = str_intern("for");
    keyword_fun    = str_intern("fun");
    keyword_if     = str_intern("if");
    keyword_nil    = str_intern("nil");
    keyword_or     = str_intern("or");
    keyword_print  = str_intern("print");
    keyword_return = str_intern("return");
    keyword_super  = str_intern("super");
    keyword_this   = str_intern("this");
    keyword_true   = str_intern("true");
    keyword_var    = str_intern("var");
    keyword_while  = str_intern("while");
}

void Token::log() {
    printf("TOKEN( type: %d, lexeme: \"%.*s\" )\n", type, end - start, start);
}

void Lexer::init(const char *source) {
    init_keywords();
    this->source = source;
}

#define MATCH(ch, token_type) \
        (token.type = (Token_Type)(*++source == ch ? source++, token_type : *(source-1)))

Token *Lexer::eat_token() {
LOOP:
    token.start = source;
    switch (*source) {
    case '0' ... '9': {
        u64 val = 0;
        while (isdigit(*source)) {
            val *= 10;
            val += *source - '0';
            source++;
        }
        token.int_val = val;
        token.type = TOKEN_NUMBER;   
    } break;
    case 'a' ... 'z':
    case 'A' ... 'Z': {
        while (isalnum(*source) || *source == '_') {
            source++;
        }
        token.name = str_intern_range(token.start, source);
        token.type = TOKEN_IDENT;
    } break;
    case '"': {
        source++;
        while (*source != '"') {
            if (*source == 0) {
                fprintf(stderr, "[!] Parsing error: Unclosed string on line %d\n", line);
                exit(1);
            }
            source++;
        }
        source++;
        token.type = TOKEN_STRING;
    } break;
    case '/':
        if (*++source == '/') {
            while (*source && *source != '\n') source++;
            goto LOOP;
        } else {
            token.type = (Token_Type) '/';
        }
        break;
    case '!': MATCH('=', TOKEN_BANG_EQUAL);    break;
    case '=': MATCH('=', TOKEN_EQUAL_EQUAL);   break;
    case '<': MATCH('=', TOKEN_LESS_EQUAL);    break;
    case '>': MATCH('=', TOKEN_GREATER_EQUAL); break;
    case ' ': 
    case '\t':
    case '\r':
        source++;
        goto LOOP;
        break;
    case '\n': 
        line++; 
        goto LOOP;
        break;
    default:
        token.type = (Token_Type) *source++;
        break;
    }
    token.end = source;

    return &token;
}

inline bool Lexer::is_token(Token_Type tp) {
    return tp == token.type;
}
