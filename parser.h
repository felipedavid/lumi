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
        u64 number;
        const char *name;
    };
};

enum Stmt_Type {
    STMT_EXPR,
    STMT_PRINT,
};

struct Stmt {
    Stmt_Type type;
    union {
        Expr *expr;
    };
};

struct Parser {
    Lexer lex;

    void init(const char *source);
    Expr *alloc_expr(Expr expr);
    Stmt *alloc_stmt(Stmt stmt);
    Expr *parse_expr();
    Expr *parse_expr_equality();
    Expr *parse_expr_comparison();
    Expr *parse_expr_term();
    Expr *parse_expr_factor();
    Expr *parse_expr_unary();
    Expr *parse_expr_primary();
    Stmt *parse_stmt_print();
    Stmt *parse_stmt_expr();
    Stmt *parse_stmt();
    Stmt *parse();
    void print_ast(Expr *root);
};
#endif
