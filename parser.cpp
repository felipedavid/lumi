#include "common.h"
#include "string.h"
#include "parser.h"

void Parser::init(const char *source) {
    lex.init(source);
    lex.eat_token();
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

    while (lex.match(EQUAL_EQUAL) || lex.match(BANG_EQUAL)) {
        Token_Type op = lex.prev_token.type;
        Expr *right = comparison();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::comparison() {
    Expr *expr = term();

    while (lex.match((Token_Type)'>') || lex.match((Token_Type)'<') || 
           lex.match(GREATER_EQUAL) || lex.match(LESS_EQUAL)) {
        Token_Type op = lex.prev_token.type;
        Expr *right = term();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::term() {
    Expr *expr = factor();

    while (lex.match((Token_Type)'+') || lex.match((Token_Type)'-')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = factor();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::factor() {
    Expr *expr = unary();

    while (lex.match((Token_Type)'*') || lex.match((Token_Type)'/')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = unary();
        expr = alloc_expr(Expr{BINARY_EXPR, .bin = {op, expr, right}});
    }

    return expr;
}

Expr *Parser::unary() {
    if (lex.match((Token_Type)'!') || lex.match((Token_Type)'-')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = unary();
        return alloc_expr(Expr{UNARY_EXPR, {op, right}});
    }

    return primary();
}

Expr *Parser::primary() {
    if (lex.match(FALSE)) return alloc_expr(Expr{LITERAL_EXPR, .number = false});
    if (lex.match(TRUE))  return alloc_expr(Expr{LITERAL_EXPR, .number = true});
    if (lex.match(NIL))   return alloc_expr(Expr{LITERAL_EXPR, .number = 0});
    if (lex.match(NUMBER))
        return alloc_expr(Expr{LITERAL_EXPR, .number = lex.token.val.int_val});
    if (lex.match(STRING))
        return alloc_expr(Expr{LITERAL_EXPR, .name = lex.token.val.name});

    if (lex.match((Token_Type)'(')) {
        Expr *expr = expression();
        if (!lex.match((Token_Type)')')) {
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

void Parser::print_expr(Expr *expr) {
    printf("Hello there.\n");
}
