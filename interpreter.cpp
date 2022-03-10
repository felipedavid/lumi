#include "interpreter.h"

// Temporary. Just to check the AST.
u64 Interpreter::evaluate(Expr *root) {
    switch (root->type) {
    case BINARY_EXPR: {
        u64 lv = evaluate(root->bin.left);
        u64 rv = evaluate(root->bin.right);
        char op = (char) root->bin.op;

        if (op == '+') return lv + rv;
        if (op == '-') return lv - rv;
        if (op == '*') return lv * rv;
        if (op == '/') return lv / rv;
    } break;
    case LITERAL_EXPR:
        return root->number;
        break;
    }
    return 0;
}
