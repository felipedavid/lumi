typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Typespec Typespec;

typedef enum {
  TYPESPEC_NONE,
  TYPESPEC_NAME,
  TYPESPEC_FUNC,
  TYPESPEC_ARRAY,
  TYPESPEC_POINTER,
} TypespecKind;

typedef struct {
  BUF(Typespec **args);
  Typespec *ret;
} FuncTypespec;

struct Typespec {
  TypespecKind kind;
  struct {
    const char *name;
    FuncTypespec func;
    // Pointer/array types
    struct {
      Typespec *base;
      Expr *size;
    };
  };
};

Typespec *typespec_alloc(TypespecKind kind) {
  Typespec *type = xcalloc(1, sizeof(Typespec));
  type->kind = kind;
  return type;
}

Typespec *typespec_name(const char *name) {
  Typespec *type = typespec_alloc(TYPESPEC_NAME);
  type->name = name;
  return type;
}

Typespec *typespec_pointer(Typespec *base) {
  Typespec *type = typespec_alloc(TYPESPEC_POINTER);
  type->base = base;
  return type;
}

Typespec *typespec_array(Typespec *base, Expr *size) {
  Typespec *type = typespec_alloc(TYPESPEC_ARRAY);
  type->base = base;
  type->size = size;
  return type;
}

typedef enum {
  DECL_NONE,
  DECL_ENUM,
  DECL_STRUCT,
  DECL_UNION,
  DECL_VAR,
  DECL_CONST,
  DECL_TYPEDEF,
  DECL_FUNC,
} DeclKind;

typedef struct {
  const char *name;
  Typespec *type;
} EnumItem;

typedef struct {
  BUF(const char **names); 
  Typespec *type;
} AggregateItem;

typedef struct {
  const char *name;
  Typespec *type;
} FuncParam;

typedef struct {
  BUF(FuncParam *params); 
  Typespec *ret_type;
} FuncDecl;

typedef struct {
  DeclKind kind;
  const char *name;
  union {
    BUF(EnumItem *enum_items);
    BUF(AggregateItem *aggregate_items); 
    struct {
      Typespec *type;
      Expr *expr;
    };
    FuncDecl func_decl;
  };
} Decl;

typedef enum {
  EXPR_NONE,
  EXPR_INT,
  EXPR_FLOAT,
  EXPR_STR,
  EXPR_NAME,
  EXPR_CALL,
  EXPR_CAST,
  EXPR_INDEX,
  EXPR_FIELD,
  EXPR_COMPOUND,
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_TERNARY,
} ExprKind;

struct Expr {
  ExprKind kind;
  TokenKind op;
  union {
    // Literals
    uint64_t int_val;
    double float_val;
    const char *str_val;
    const char *name;
    // Compound literals
    struct {
      Typespec *compound_type;
      BUF(Expr **compound_args); 
    };
    // Casts
    struct {
      Typespec *cast_type;
      Expr *cast_expr;
    };
    struct {
      // Unary
      Expr *operand;
      union {
        BUF(Expr **args);
        Expr *index;
        const char *field;
      };
    };
    struct {
      // Binary
      Expr *left;
      Expr *right;
    };
    struct {
      // Ternary
      Expr *cond;
      Expr *then_expr;
      Expr *else_expr;
    };
  };
};

Expr *expr_alloc(ExprKind kind) {
  Expr *expr = xcalloc(1, sizeof(Expr));
  expr->kind = kind;
  return expr;
}

Expr *expr_int(uint64_t int_val) {
  Expr *expr = expr_alloc(EXPR_INT);
  expr->int_val = int_val;
  return expr;
}

Expr *expr_float(double float_val) {
  Expr *expr = expr_alloc(EXPR_FLOAT);
  expr->float_val = float_val;
  return expr;
}

Expr *expr_str(const char *str) {
  Expr *expr = expr_alloc(EXPR_STR);
  expr->str_val = str;
  return expr;
}

Expr *expr_name(const char *name) {
  Expr *expr = expr_alloc(EXPR_NAME);
  expr->name = name;
  return expr;
}

Expr *expr_call(Expr *operand, Expr **args) {
  Expr *expr = expr_alloc(EXPR_CALL);
  expr->operand = operand;
  expr->args = args;
  return expr;
}

Expr *expr_field(Expr *operand, const char *field) {
  Expr *expr = expr_alloc(EXPR_FIELD);
  expr->operand = operand;
  expr->field = field;
  return expr;
}

Expr *expr_index(Expr *operand, Expr *index) {
  Expr *expr = expr_alloc(EXPR_INDEX);
  expr->operand = operand;
  expr->index = index;
  return expr;
}

Expr *expr_cast(Typespec *type, Expr *expr) {
  Expr *new_expr = expr_alloc(EXPR_CAST);
  new_expr->cast_type = type;
  new_expr->cast_expr = expr;
  return new_expr;
}

Expr *expr_unary(TokenKind op, Expr *expr) {
  Expr *new_expr = expr_alloc(EXPR_UNARY);
  new_expr->op = op;
  new_expr->operand = expr;
  return new_expr;
}

Expr *expr_binary(TokenKind op, Expr *left, Expr *right) {
  Expr *expr = expr_alloc(EXPR_BINARY);
  expr->op = op;
  expr->left = left;
  expr->right = right;
  return expr;
}

Expr *expr_ternary(Expr *cond, Expr *then_expr, Expr *else_expr) {
  Expr *expr = expr_alloc(EXPR_TERNARY);
  expr->cond = cond;
  expr->then_expr = then_expr;
  expr->else_expr = else_expr;
  return expr;
}

void print_expr(Expr *expr);

void print_type(Typespec *type) {
  switch (type->kind) {
  case TYPESPEC_NAME:
    printf("%s", type->name);
    break;
  case TYPESPEC_FUNC: {
    FuncTypespec func = type->func;
    printf("(func (");
    for (Typespec **it = func.args; it != buf_end(func.args); it++) {
      printf(" ");
      print_type(*it);
    }
    printf(") ");
    print_type(func.ret);
    printf(")");
  } break;
  case TYPESPEC_ARRAY:
    printf("(arr ");
    print_type(type->base);
    printf(" ");
    print_expr(type->size);
    printf(")");
    break;
  case TYPESPEC_POINTER:
    printf("(ptr ");
    print_type(type->base);
    printf(")");
    break;
  default:
    assert(0);
  }
}

void print_expr(Expr *expr) {
  switch (expr->kind) {
  case EXPR_INT:
    printf("%lu", expr->int_val);
    break;
  case EXPR_FLOAT:
    printf("%f", expr->float_val);
    break;
  case EXPR_STR:
    printf("\"%s\"", expr->str_val);
    break;
  case EXPR_NAME:
    printf("%s", expr->name);
    break;
  case EXPR_CAST:
    printf("cast (");
    print_type(expr->cast_type);
    printf(" ");
    print_expr(expr->cast_expr);
    printf(")");
    break;
  case EXPR_CALL:
    printf("(");
    print_expr(expr->operand);
    for (Expr **it = expr->args; it != buf_end(expr->args); it++) {
      printf(" ");
      print_expr(*it);
    }
    printf(")");
    break;
  case EXPR_INDEX:
    printf("(index ");
    print_expr(expr->operand);
    printf(" ");
    print_expr(expr->index);
    printf(")");
    break;
  case EXPR_FIELD:
    printf("(field ");
    print_expr(expr->operand);
    printf(" %s)", expr->field);
    break;
  case EXPR_COMPOUND:
    printf("(compound ...)");
    break;
  case EXPR_UNARY:
    printf("(%c ", expr->op);
    print_expr(expr->operand);
    printf(")");
    break;
  case EXPR_BINARY:
    printf("(%c ", expr->op);
    print_expr(expr->left);
    printf(" ");
    print_expr(expr->right);
    printf(")");
    break;
  case EXPR_TERNARY:
    printf("(if ");
    print_expr(expr->cond);
    printf(" ");
    print_expr(expr->then_expr);
    printf(" ");
    print_expr(expr->else_expr);
    printf(")");
    break;
  default:
    assert(0);
  }
}

typedef enum {
  STMT_NONE,
  STMT_RETURN,
  STMT_BREAK,
  STMT_CONTINUE,
  STMT_BLOCK,
  STMT_IF,
  STMT_WHILE,
  STMT_FOR,
  STMT_DO,
  ATMT_SWITCH,
  STMT_ASSIGN,
  STMT_AUTO_ASSIGN,
  STMT_EXPR,
} StmtKind;

typedef struct {
  BUF(Stmt **stmts);
} StmtBlock;

typedef struct {
  Expr *cond;
  StmtBlock *block;
} Elseif;

typedef struct {
  BUF(Expr **exprs);
  StmtBlock block;
} Case;

struct Stmt {
  StmtKind kind;
  Expr *expr;
  StmtBlock block;
  union {
    // If
    struct {
      BUF(Elseif *elseifs);
      StmtBlock else_block;
    };
    // For
    struct {
      StmtBlock for_init_block;
      StmtBlock for_next;
    };
    // Switch
    struct {
      BUF(Case *cases);
    };
    // Auto-assign
    struct {
      const char *name;
    };
    // Assignment operators
    struct {
      Expr *rhs;
    };
  };
};
