void buf_test() {
	int *buf = NULL;			// Always make sure to initialize the stretchy buffer to zero

	assert(buf_len(buf) == 0);
	assert(buf_cap(buf) == 0);

	enum { BUF_SIZE = 4096 };

	for (int i = 0; i < BUF_SIZE; i++) {
		buf_push(buf, i);
	}
	assert(buf_len(buf) == BUF_SIZE);

	for (int i = 0; i < BUF_SIZE; i++) {
		assert(buf[i] == i);
	}

	buf_free(buf);

	assert(buf_len(buf) == 0);
	assert(buf_cap(buf) == 0);
	assert(buf == NULL);
}

#define assert_token(x) assert(match_token(x))
#define assert_token_name(x) assert(token.name == str_intern(x) && match_token(TOKEN_NAME))
#define assert_token_int(x) assert(token.int_val == (x) && match_token(TOKEN_INT))
#define assert_token_float(x) assert(token.float_val == (x) && match_token(TOKEN_FLOAT))
#define assert_token_str(x) assert((strcmp(token.str_val, (x)) == 0) && match_token(TOKEN_STR))
#define assert_token_eof() assert(is_token(0))

void lex_test() {
	// Integer literal tests
	init_stream("0 18446744073709551615 0xffffffffffffffff 042 0b1111");
	assert_token_int(0);
	assert_token_int(18446744073709551615ull);
	assert(token.mod == TOKENMOD_HEX);
	assert_token_int(0xffffffffffffffffull);
	assert(token.mod == TOKENMOD_OCT);
	assert_token_int(042);
	assert(token.mod == TOKENMOD_BIN);
	assert_token_int(0xF);
	assert_token_eof();

	// Float literals tests
	init_stream("3.14 .123 42. 3e10");
	assert_token_float(3.14);
	assert_token_float(.123);
	assert_token_float(42.);
	assert_token_float(3e10);
	assert_token_eof();

	// Char literal tests
	init_stream("'a' '\\n' '\0'");
	assert_token_int('a');
	assert_token_int('\n');
	assert_token_int('\0');
	assert_token_eof();

	// String literals tests
	init_stream("\"foo\" \"a\\nb\"");
	assert_token_str("foo");
	assert_token_str("a\nb");
	assert_token_eof();

	// Operator tests
	init_stream(": := + += ++ < <= << <<=");
	assert_token(':');
	assert_token(TOKEN_COLON_ASSIGN);
	assert_token('+');
	assert_token(TOKEN_ADD_ASSIGN);
	assert_token(TOKEN_INC);
	assert_token('<');
	assert_token(TOKEN_LTEQ);
	assert_token(TOKEN_LSHIFT);
	assert_token(TOKEN_LSHIFT_ASSIGN);
	assert_token_eof();

	// Misc tests
	init_stream("XY+(XY)_HELLO_THERE1,1234+9994");
	assert_token_name("XY");
	assert_token('+');
	assert_token('(');
	assert_token_name("XY");
	assert_token(')');
	assert_token_name("_HELLO_THERE1");
	assert_token(',');
	assert_token_int(1234);
	assert_token('+');
	assert_token_int(9994);
	assert_token_eof();
}

void intern_test() {
	char x[] = "hello";
	char y[] = "hello";
	assert(x != y);

	const char *px = str_intern(x);
	const char *py = str_intern(y);
	assert(px == py);

	char z[] = "hello!";
	const char *pz = str_intern(z);
	assert(px != pz);
}

void print_expr_line(Expr * expr) {
	print_expr(expr);
	printf("\n");
}

void expr_test() {
	Expr *exprs[] = {
		expr_binary('+', expr_int(1), expr_int(2)),
		expr_unary('-', expr_float(3.14)),
		expr_ternary(expr_name("flag"), expr_str("true"), expr_str("false")),
		expr_field(expr_name("person"), "name"),
		expr_call(expr_name("fact"), 1, ((Expr *[]){expr_int(42)})),
		expr_index(expr_field(expr_name("person"), "siblings"), expr_int(3)),
		expr_cast(typespec_pointer(typespec_name("int")), expr_name("void_ptr")),
	};

	for (Expr ** it = exprs; it != exprs + sizeof(exprs) / sizeof(*exprs); it++) {
		print_expr_line(*it);
	}
}

void ast_test() {
	expr_test();
}

void run_tests() {
	buf_test();
	lex_test();
	intern_test();
	ast_test();
}
