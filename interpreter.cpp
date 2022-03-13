#include "interpreter.h"

void Interpreter::interpret(Stmt *root) {
    switch (root->type) {
    case STMT_EXPR: 
        evaluate(root->expr);
        break;
    case STMT_PRINT:
        printf("%d\n", evaluate(root->expr));
        break;
    }
}

// Only supports integers at the moment.
u64 Interpreter::evaluate(Expr *expr) {
    switch (expr->type) {
    case UNARY_EXPR: {
        u64 v = evaluate(expr->unary.expr);
        char op = (char) expr->unary.op;

        switch (op) {
        case '-': return -v;
        case '!': return !v;
        }
    }
    case BINARY_EXPR: {
        u64 lv = evaluate(expr->bin.left);
        u64 rv = evaluate(expr->bin.right);
        char op = (char) expr->bin.op;

        switch (op) {
        case '+': return lv + rv;
        case '-': return lv - rv;
        case '*': return lv * rv;
        case '/': return lv / rv;
        case '>': return lv > rv;
        case '<': return lv < rv;
        }

        switch (expr->bin.op) {
        case GREATER_EQUAL: return lv >= rv;
        case LESS_EQUAL:    return lv <= rv;
        case EQUAL_EQUAL:   return lv == rv;
        case BANG_EQUAL:    return lv == rv;
        }
    }
    case LITERAL_EXPR:
        return expr->number;
    case GROUPING_EXPR:
        return evaluate(expr->group);
    }

    return 0;
}
