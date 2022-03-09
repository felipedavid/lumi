#include "lexer.h"

struct Parser {
    Lexer lex;

    void init();
    Expr expression();
    Expr equality();
    Expr comparison();
    Expr term();
    Expr factor();
    Expr unary();
};
