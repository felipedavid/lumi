#include "common.h"
#include "string.h"
#include "parser.h"

extern const char *print_keyword;

void Parser::init(const char *source) {
    lex.init(source);
    lex.eat_token();
}

Expr *Parser::alloc_expr(Expr expr) {
    Expr *buf = (Expr *) xmalloc(sizeof(Expr));
    memcpy(buf, &expr, sizeof(Expr));
    return buf;
}

Stmt *Parser::alloc_stmt(Stmt stmt) {
    Stmt *buf = (Stmt *) xmalloc(sizeof(Stmt));
    memcpy(buf, &stmt, sizeof(Stmt));
    return buf;
}

Expr *Parser::parse_expr() {
    return parse_expr_equality();
}

Expr *Parser::parse_expr_equality() {
    Expr *expr = parse_expr_comparison();

    while (lex.match(EQUAL_EQUAL) || lex.match(BANG_EQUAL)) {
        Token_Type op = lex.prev_token.type;
        Expr *right = parse_expr_comparison();
        expr = alloc_expr(Expr{BINARY_EXPR, {op, expr, right}});
    }

    return expr;
}

Expr *Parser::parse_expr_comparison() {
    Expr *expr = parse_expr_term();

    while (lex.match((Token_Type)'>') || lex.match((Token_Type)'<') || 
           lex.match(GREATER_EQUAL) || lex.match(LESS_EQUAL)) {
        Token_Type op = lex.prev_token.type;
        Expr *right = parse_expr_term();
        expr = alloc_expr(Expr{BINARY_EXPR, {op, expr, right}});
    }

    return expr;
}

Expr *Parser::parse_expr_term() {
    Expr *expr = parse_expr_factor();

    while (lex.match((Token_Type)'+') || lex.match((Token_Type)'-')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = parse_expr_factor();
        expr = alloc_expr(Expr{BINARY_EXPR, {op, expr, right}});
    }

    return expr;
}

Expr *Parser::parse_expr_factor() {
    Expr *expr = parse_expr_unary();

    while (lex.match((Token_Type)'*') || lex.match((Token_Type)'/')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = parse_expr_unary();
        expr = alloc_expr(Expr{BINARY_EXPR, {op, expr, right}});
    }

    return expr;
}

Expr *Parser::parse_expr_unary() {
    if (lex.match((Token_Type)'!') || lex.match((Token_Type)'-')) {
        Token_Type op = lex.prev_token.type;
        Expr *right = parse_expr_unary();
        return alloc_expr(Expr{UNARY_EXPR, {op, right}});
    }

    return parse_expr_primary();
}

Expr *Parser::parse_expr_primary() {
    if (lex.match(FALSE)) return alloc_expr(Expr{LITERAL_EXPR, .number = false});
    if (lex.match(TRUE))  return alloc_expr(Expr{LITERAL_EXPR, .number = true});
    if (lex.match(NIL))   return alloc_expr(Expr{LITERAL_EXPR, .number = 0});
    if (lex.match(NUMBER))
        return alloc_expr(Expr{LITERAL_EXPR, .number = lex.token.val.int_val});
    if (lex.match(STRING))
        return alloc_expr(Expr{LITERAL_EXPR, .name = lex.token.val.name});

    if (lex.match((Token_Type)'(')) {
        Expr *expr = parse_expr();
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

Stmt *Parser::parse_stmt_print() {
    Expr *val = parse_expr();
    if (!lex.match((Token_Type)';')) {
        fprintf(stderr, "Expected ';' after value.\n");
        return alloc_stmt(Stmt{STMT_NONE});
    }
    return alloc_stmt(Stmt{STMT_PRINT, val});
}

Stmt *Parser::parse_stmt_expr() {
    Expr *expr = parse_expr();
    if (!lex.match((Token_Type)';')) {
        fprintf(stderr, "Expected ';' after expression.\n");
        return alloc_stmt(Stmt{STMT_NONE});
    }
    return alloc_stmt(Stmt{STMT_EXPR, expr});
}

Stmt *Parser::parse_stmt() {
    Stmt *stmt = NULL;
    if (lex.is_token((Token_Type)'\0')) {
        stmt = NULL;
    } else if (lex.match_keyword(print_keyword)) {
        stmt = parse_stmt_print();
    } else {
        stmt = parse_stmt_expr();
    }

    return stmt;
}

Stmt *Parser::parse() {
    return parse_stmt();
}

void Parser::print_ast(Expr *root) {
    switch (root->type) {
    case BINARY_EXPR:
        printf("(%c ", root->bin.op);
        print_ast(root->bin.left);
        printf(" ");
        print_ast(root->bin.right);
        printf(") ");
        break;
    case LITERAL_EXPR:
        printf("%d", root->number);
        break;
    }
}
