#include "lexer.h"

enum Expr_Type {
    EXPR_BINARY,
};

struct Expr {
    Expr_Type type;
    union {
        struct {
            Expr *left;
            Expr *right;
            Token_Type op;
        } binary;
    };
};

struct Parser {
    Lexer lex;

    Expr expression();
    Expr equality();
    Expr comparison();
    Expr term();
    Expr factor();
    Expr unary();
    Expr primary();
};
