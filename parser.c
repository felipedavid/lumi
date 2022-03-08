#include <stdarg.h>

#include "parser.h"

Expr Parser::expression() {
    return equality();
}

Expr Parser::equality() {
    Expr expr = comparison();

    while (lex.is_token(TOKEN_BANG_EQUAL) || lex.is_token(TOKEN_EQUAL_EQUAL)) {
        Token_Type op = lex.curr_tk->type;
        // Eat token?
        Expr right = comparison();
        expr = Expr{EXPR_BINARY, {expr, right, op}};
    }

    return expr;
}

Expr Parser::comparison() {
    Expr expr = term();

    while (lex.is_token('>') || lex.is_token('<') || 
           lex.is_token(TOKEN_GREATER_EQUAL) || 
           lex.is_token(TOKEN_LESS_EQUAL)) {
        Token_Type = lex.curr_tk->type;
        Expr right = term();
        expr = Expr{EXPR_BINARY, {expr, right, op}};
    }
    return expr;
}

Expr Parser::term() {
    Expr expr;
    return expr;
}

Expr Parser::factor() {
    Expr expr;
    return expr;
}

Expr Parser::unary() {
    Expr expr;
    return expr;
}

Expr Parser::primary() {
    Expr expr;
    return expr;
}
