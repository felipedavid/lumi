#include "parser.h"
#include "common.h"

struct Interpreter {
    bool had_error = false;

    void interpret(Stmt *root);
    u64 evaluate(Expr *expr);
};
