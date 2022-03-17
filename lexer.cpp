#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"

const char *and_keyword;
const char *class_keyword;
const char *else_keyword;
const char *false_keyword;
const char *for_keyword;
const char *fun_keyword;
const char *if_keyword;
const char *nil_keyword;
const char *or_keyword;
const char *print_keyword;
const char *return_keyword;
const char *super_keyword;
const char *this_keyword;
const char *true_keyword;
const char *var_keyword;
const char *while_keyword;

void init_keywords() {
    and_keyword    = str_intern("and");
    class_keyword  = str_intern("class");
    else_keyword   = str_intern("else");
    false_keyword  = str_intern("false");
    for_keyword    = str_intern("for");
    fun_keyword    = str_intern("fun");
    if_keyword     = str_intern("if");
    nil_keyword    = str_intern("nil");
    or_keyword     = str_intern("or");
    print_keyword  = str_intern("print");
    return_keyword = str_intern("return");
    super_keyword  = str_intern("super");
    this_keyword   = str_intern("this");
    true_keyword   = str_intern("true");
    var_keyword    = str_intern("var");
    while_keyword  = str_intern("while");
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
        token.val.int_val = val;
        token.type = NUMBER;   
    } break;
    case 'a' ... 'z':
    case 'A' ... 'Z': {
        while (isalnum(*source) || *source == '_') {
            source++;
        }
        token.val.name = str_intern_range(token.start, source);
        token.type = IDENT;
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
        token.type = STRING;
    } break;
    case '/':
        if (*++source == '/') {
            while (*source && *source != '\n') source++;
            goto LOOP;
        } else {
            token.type = (Token_Type) '/';
        }
        break;
    case '!': MATCH('=', BANG_EQUAL);    break;
    case '=': MATCH('=', EQUAL_EQUAL);   break;
    case '<': MATCH('=', LESS_EQUAL);    break;
    case '>': MATCH('=', GREATER_EQUAL); break;
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

bool Lexer::is_token(Token_Type tp) {
    return tp == token.type;
}

bool Lexer::match(Token_Type tp) {
    if (tp == token.type) {
        prev_token = token;
        eat_token();
        return true;
    }
    return false;
}

bool Lexer::is_keyword(const char *keyword) {
    return token.val.name == keyword;
}

bool Lexer::match_keyword(const char *keyword) {
    if (is_keyword(keyword)) {
        eat_token();
        return true;
    }
    return false;
}
