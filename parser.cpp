#include "common.h"
#include "string.h"
#include "parser.h"

void Parser::init(const char *source) {
    lex.init(source);
}

Expr *Parser::alloc_expr(Expr expr) {
    Expr *buf = (Expr *) xmalloc(sizeof(Expr));
    memcpy(buf, &expr, sizeof(Expr));
    return buf;
}

Expr *Parser::expression() {
    return equality();
}

Expr *Parser::equality() {
    Expr *expr = comparison();

    while (lex.is_token(EQUAL_EQUAL) || lex.is_token(BANG_EQUAL)) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr *right = comparison();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::comparison() {
    Expr *expr = term();

    while (lex.is_token((Token_Type)'>') || lex.is_token((Token_Type)'<') || 
           lex.is_token(GREATER_EQUAL) || lex.is_token(LESS_EQUAL)) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr *right = term();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::term() {
    Expr *expr = factor();

    while (lex.is_token((Token_Type)'+') || lex.is_token((Token_Type)'-')) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr *right = factor();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::factor() {
    Expr *expr = unary();

    while (lex.is_token((Token_Type)'*') || lex.is_token((Token_Type)'/')) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr *right = unary();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::unary() {
    if (lex.is_token((Token_Type)'!') || lex.is_token((Token_Type)'-')) {
        Token_Type op = lex.token.type;
        Expr *right = unary();
        return alloc_expr(Expr{UNARY_EXPR, {op, right}});
    }

    return primary();
}

Expr *Parser::primary() {
    if (lex.is_token(FALSE)) return alloc_expr(Expr{LITERAL_EXPR, .literal = false});
    if (lex.is_token(TRUE))  return alloc_expr(Expr{LITERAL_EXPR, .literal = true});
    if (lex.is_token(NIL))   return alloc_expr(Expr{LITERAL_EXPR, .literal = 0});

    if (lex.is_token(NUMBER) || lex.is_token(STRING)) {
        return alloc_expr(Expr{LITERAL_EXPR, .literal = 0});
    }

    if (lex.is_token((Token_Type)'(')) {
        Expr *expr = expression();
        if (!lex.is_token((Token_Type)')')) {
            printf("Expect ')' after expression.");
            exit(1);
        }
        lex.eat_token();
        return alloc_expr(Expr{GROUPING_EXPR, .group = expr});
    }

    printf("[!] You fucked up somewhere\n");
    exit(1);
}

Expr *Parser::parse() {
    return expression();
}

void Parser::print_ast() {
    printf("Hello there.\n");
}
