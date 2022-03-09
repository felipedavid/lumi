#include "parser.h"

void Parser::init(const char *source) {
    lex.init(source)
}

Expr Parser::expression() {
    return equality();
}

Expr Parser::equality() {
    Expr expr = comparison();

    while (lex.is_token(EQUAL_EQUAL) || lex.is_token(BANG_EQUAL)) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr right = comparison();
        expr = Expr{BINARY_EXPR, {expr, op, right}};
    }

    return expr;
}

Expr Parser::comparison() {
    Expr expr = term();

    while (lex.is_token((Token_Type)'>') || lex.is_token((Token_Type)'<') || 
           lex.is_token(GREATER_EQUAL) || lex.is_token(LESS_EQUAL)) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr right = term();
        expr = Expr{BINARY_EXPR, {expr, op, right}};
    }

    return expr;
}

Expr Parser::term() {
    Expr expr = factor();

    while (lex.is_token((Token_Type)'+') || lex.is_token((Token_Type)'-')) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr right = factor();
        expr = Expr{BINARY_EXPR, {expr, op, right}};
    }

    return expr;
}

Expr Parser::factor() {
    Expr expr = unary();

    while (lex.is_token((Token_Type)'*') || lex.is_token((Token_Type)'/')) {
        Token_Type op = lex.token.type;
        lex.eat_token();
        Expr right = unary();
        expr = Expr{BINARY_EXPR, {expr, op, right}};
    }

    return expr;
}

Expr Parser::unary() {
    if (lex.is_token((Token_Type)'!') || lex.is_token((Token_Type)'-')) {
        Token_Type op = lex.token.type;
        Expr right = unary();
        return Expr{UNARY, {op, right}};
    }

    return primary();
}

Expr Parser::primary() {
    if (lex.is_token(FALSE)) return Expr{LITERAL, {false}};
    if (lex.is_token(TRUE)) return Expr{LITERAL, {true}};
    if (lex.is_token(NIL)) return Expr{LITERAL, {null}};

    if (lex.is_token(NUMBER) || lex.is_token(STRING)) {
        return Expr{LITERAL, {val}};
    }

    if (lex.is_token(LEFT_PAREN)) {
        Expr expr = expression();
        if (!lex.is_token(RIGHT_PAREN)) {
            printf("Expect ')' after expression.");
            exit(1);
        }
        return Expr{GROUPING, expr};
    }
}
