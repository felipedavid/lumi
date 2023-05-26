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
	Typespec **args;
	size_t num_args;
	Typespec *ret;
} FuncTypespec;

typedef struct {
	Typespec *elem;
} PtrTypespec;

typedef struct {
	Typespec *elem;
	Expr *size;
} ArrayTypespec;

struct Typespec {
	TypespecKind kind;
	struct {
		const char *name;
		FuncTypespec func;
		ArrayTypespec array;
		PtrTypespec ptr;
	};
};

Typespec *typespec_new(TypespecKind kind) {
	Typespec *t = xcalloc(1, sizeof(Typespec));
	t->kind = kind;
	return t;
}

Typespec *typespec_name(const char *name) {
	Typespec *t = typespec_new(TYPESPEC_NAME);
	t->name = name;
	return t;
}

Typespec *typespec_pointer(Typespec * base) {
	Typespec *t = typespec_new(TYPESPEC_POINTER);
	t->ptr.elem = base;
	return t;
}

Typespec *typespec_array(Typespec * base, Expr * size) {
	Typespec *t = typespec_new(TYPESPEC_ARRAY);
	t->array.elem = base;
	t->array.size = size;
	return t;
}

Typespec *typespec_func(Typespec ** args, size_t num_args, Typespec * ret) {
	Typespec *t = typespec_new(TYPESPEC_FUNC);
	t->func.args = args;
	t->func.num_args = num_args;
	t->func.ret = ret;
	return t;
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
	Expr *init;
} EnumItem;

typedef struct {
	EnumItem *items;
	size_t num_items;
} EnumDecl;

typedef struct {
	const char **names;
	size_t num_names;
	Typespec *type;
} AggregateItem;

typedef struct {
	AggregateItem *items;
	size_t num_items;
} AggregateDecl;

typedef struct {
	const char *name;
	Typespec *type;
} FuncParam;

typedef struct {
	FuncParam *params;
	size_t num_params;
	Typespec *ret_type;
} FuncDecl;

typedef struct {
	Typespec *type;
} TypedefDecl;

typedef struct {
	Expr *expr;
} ConstDecl;

typedef struct {
	DeclKind kind;
	const char *name;
	union {
		EnumDecl enum_decl;
		AggregateDecl aggregate;
		FuncDecl func_decl;
		TypedefDecl typedef_decl;
		ConstDecl const_decl;
	};
} Decl;

Decl *decl_new(DeclKind kind, const char *name) {
	Decl *d = ast_alloc(sizeof(Decl));
	d->kind = kind;
	d->name = name;
	return d;
}

Decl *decl_enum(const char *name, EnumItem * items, size_t num_items) {
	Decl *d = decl_new(DECL_ENUM, name);
	d->enum_decl.items = items;
	d->enum_decl.num_items = num_items;
	return d;
}

Decl *decl_aggregate(DeclKind kind, const char *name, AggregateItem * items, size_t num_items) {
	assert(kind == DECL_STRUCT || kind == DECL_UNION);
	Decl *d = decl_new(kind, name);
}

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

typedef struct {
	Typespec *type;
	Expr **args;
	size_t num_args;
} CompoundExpr;

typedef struct {
	Typespec *type;
	Expr *expr;
} CastExpr;

typedef struct {
	TokenKind op;
	Expr *expr;
} UnaryExpr;

typedef struct {
	TokenKind op;
	Expr *left;
	Expr *right;
} BinaryExpr;

typedef struct {
	Expr *cond;
	Expr *then_expr;
	Expr *else_expr;
} TernaryExpr;

typedef struct {
	Expr *expr;
	Expr **args;
	size_t num_args;
} CallExpr;

typedef struct {
	Expr *expr;
	Expr *index;
} IndexExpr;

typedef struct {
	Expr *expr;
	const char *name;
} FieldExpr;

struct Expr {
	ExprKind kind;
	union {
		uint64_t int_val;
		double float_val;
		const char *str_val;
		const char *name;
		CompoundExpr compound;
		CastExpr cast;
		UnaryExpr unary;
		BinaryExpr binary;
		TernaryExpr ternary;
		CallExpr call;
		IndexExpr index;
		FieldExpr field;
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

Expr *expr_call(Expr * operand, size_t num_args, Expr ** args) {
	Expr *expr = expr_alloc(EXPR_CALL);
	expr->operand = operand;
	expr->num_args = num_args;
	expr->args = args;
	return expr;
}

Expr *expr_field(Expr * operand, const char *field) {
	Expr *expr = expr_alloc(EXPR_FIELD);
	expr->operand = operand;
	expr->field = field;
	return expr;
}

Expr *expr_index(Expr * operand, Expr * index) {
	Expr *expr = expr_alloc(EXPR_INDEX);
	expr->operand = operand;
	expr->index = index;
	return expr;
}

Expr *expr_cast(Typespec * type, Expr * expr) {
	Expr *new_expr = expr_alloc(EXPR_CAST);
	new_expr->cast_type = type;
	new_expr->cast_expr = expr;
	return new_expr;
}

Expr *expr_unary(TokenKind op, Expr * expr) {
	Expr *new_expr = expr_alloc(EXPR_UNARY);
	new_expr->op = op;
	new_expr->operand = expr;
	return new_expr;
}

Expr *expr_binary(TokenKind op, Expr * left, Expr * right) {
	Expr *expr = expr_alloc(EXPR_BINARY);
	expr->op = op;
	expr->left = left;
	expr->right = right;
	return expr;
}

Expr *expr_ternary(Expr * cond, Expr * then_expr, Expr * else_expr) {
	Expr *expr = expr_alloc(EXPR_TERNARY);
	expr->cond = cond;
	expr->then_expr = then_expr;
	expr->else_expr = else_expr;
	return expr;
}

void print_expr(Expr * expr);

void print_type(Typespec * type) {
	switch (type->kind) {
	case TYPESPEC_NAME:
		printf("%s", type->name);
		break;
	case TYPESPEC_FUNC:
		{
			FuncTypespec func = type->func;
			printf("(func (");
			for (Typespec ** it = func.args; it != func.args + func.num_args; it++) {
				printf(" ");
				print_type(*it);
			}
			printf(") ");
			print_type(func.ret);
			printf(")");
		}
		break;
	case TYPESPEC_ARRAY:
		printf("(arr ");
		print_type(type->array.elem);
		printf(" ");
		print_expr(type->array.size);
		printf(")");
		break;
	case TYPESPEC_POINTER:
		printf("(ptr ");
		print_type(type->ptr.elem);
		printf(")");
		break;
	default:
		assert(0);
	}
}

void print_expr(Expr * expr) {
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
		for (Expr ** it = expr->args; it != expr->args + expr->num_args; it++) {
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

typedef struct {
	Stmt **stmts;
	size_t num_stmts;
} StmtBlock;

typedef struct {
	Expr *cond;
	StmtBlock *block;
} Elseif;

typedef struct {
	Expr **exprs;
	size_t num_exprs;
	StmtBlock block;
} Case;

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
	Expr *expr;
} ReturnStmt;

typedef struct {
	Expr *cond;
	StmtBlock block;
} ElseIf;

typedef struct {
	Expr *cond;
	StmtBlock then_block;
	ElseIf *elseifs;
	size_t num_elseifs;
	StmtBlock else_block;
} IfStmt;

typedef struct {
	Expr *cond;
	StmtBlock block;
} WhileStmt;

typedef struct {
	Stmt *init;
	Expr *cond;
	Stmt *next;
	StmtBlock block;
} ForStmt;

typedef struct {
	Expr **exprs;
	size_t num_exprs;
	bool is_default;
	StmtBlock block;
} SwitchCase;

typedef struct {
	Expr *expr;
	SwitchCase *cases;
	size_t num_cases;
} SwitchStmt;

typedef struct {
	TokenKind op;
	Expr *left;
	Expr *right;
} AssignStmt;

typedef struct {
	const char *name;
	Expr *expr;
} InitStmt;

struct Stmt {
	StmtKind kind;
	union {
		ReturnStmt return_stmt;
		IfStmt if_stmt;
		WhileStmt while_stmt;
		ForStmt for_stmt;
		SwitchStmt switch_stmt;
		StmtBlock block;
		AssignStmt assign;
		InitStmt init;
		Expr *expr;
	};
};
