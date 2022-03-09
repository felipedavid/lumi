#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum Expr_Type {
    BINARY_EXPR,
    LITERAL_EXPR,
    UNARY_EXPR,
    GROUPING_EXPR,
};

struct Expr {
    Expr_Type type;
    union {
        struct {
            Token_Type op;
            Expr *expr;
        } unary;
        struct {
            Token_Type op;
            Expr *left;
            Expr *right;
        } bin;
        Expr *group;
        u64 literal;
    };
};

struct Parser {
    Lexer lex;

    void init(const char *source);
    Expr *alloc_expr(Expr expr);
    Expr *expression();
    Expr *equality();
    Expr *comparison();
    Expr *term();
    Expr *factor();
    Expr *unary();
    Expr *primary();
    Expr *parse();
    void print_ast();
};
#endif
