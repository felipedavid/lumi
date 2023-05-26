
void fatal(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	printf("FATAL: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
	exit(1);
}

void syntax_error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	printf("Syntax Error: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

typedef enum {
	FIRST_NONCHAR_TOKEN = 128,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_STR,
	TOKEN_NAME,
	TOKEN_LSHIFT,
	TOKEN_RSHIFT,
	TOKEN_EQ,
	TOKEN_NOTEQ,
	TOKEN_LTEQ,
	TOKEN_GTEQ,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_INC,
	TOKEN_DEC,
	TOKEN_COLON_ASSIGN,
	TOKEN_ADD_ASSIGN,
	TOKEN_SUB_ASSIGN,
	TOKEN_OR_ASSIGN,
	TOKEN_AND_ASSIGN,
	TOKEN_XOR_ASSIGN,
	TOKEN_LSHIFT_ASSIGN,
	TOKEN_RSHIFT_ASSIGN,
	TOKEN_MUL_ASSIGN,
	TOKEN_DIV_ASSIGN,
	TOKEN_MOD_ASSIGN,
} TokenKind;

typedef enum {
	TOKENMOD_NONE,
	TOKENMOD_HEX,
	TOKENMOD_BIN,
	TOKENMOD_OCT,
	TOKENMOD_CHAR,
} TokenMod;

typedef struct {
	TokenKind kind;
	TokenMod mod;
	const char *start;
	const char *end;
	union {
		uint64_t int_val;
		double float_val;
		const char *str_val;
		const char *name;
	};
} Token;

const char *stream;
Token token;

const char *str_intern_range(const char *start, const char *end);

uint8_t char_to_digit[256] = {
	['0'] = 0,
	['1'] = 1,
	['2'] = 2,
	['3'] = 3,
	['4'] = 4,
	['5'] = 5,
	['6'] = 6,
	['7'] = 7,
	['8'] = 8,
	['9'] = 9,
	['a'] = 10,['A'] = 10,
	['b'] = 11,['B'] = 11,
	['c'] = 12,['C'] = 12,
	['d'] = 13,['D'] = 13,
	['e'] = 14,['E'] = 14,
	['f'] = 15,['F'] = 15,
};

void scan_int() {
	uint64_t base = 10;

	// Handle hex values
	if (*stream == '0') {
		stream++;
		if (tolower(*stream) == 'x') {
			stream++;
			token.mod = TOKENMOD_HEX;
			base = 16;
		}
		else if (tolower(*stream) == 'b') {
			stream++;
			token.mod = TOKENMOD_BIN;
			base = 2;
		}
		else if (isdigit(tolower(*stream))) {
			token.mod = TOKENMOD_OCT;
			base = 8;
		}
	}

	uint64_t val = 0;
	for (;;) {
		uint64_t digit = char_to_digit[*stream];
		if (digit == 0 && *stream != '0') {
			break;
		}
		if (digit >= base) {
			syntax_error("Digit '%c' out of range of base %lu", *stream, base);
			digit = 0;
		}
		if (val > (UINT64_MAX - digit) / base) {
			syntax_error("Integer literal overflow");
			while (isdigit(*stream)) {
				stream++;
			}
			val = 0;
		}
		val = val * base + digit;
		stream++;
	}
	token.kind = TOKEN_INT;
	token.int_val = val;
}

void scan_float() {
	const char *start = stream;
	while (isdigit(*stream)) {
		stream++;
	}
	if (*stream == '.') {
		stream++;
	}
	while (isdigit(*stream)) {
		stream++;
	}
	if (tolower(*stream) == 'e') {
		stream++;
		if (*stream == '+' || *stream == '-') {
			stream++;
		}
		if (!isdigit(*stream)) {
			syntax_error("Expected digit after float literal exponent, found '%c'.", *stream);
		}
		while (isdigit(*stream)) {
			stream++;
		}
	}
	const char *end = stream;
	double val = strtod(start, NULL);
	if (val == HUGE_VAL || val == -HUGE_VAL) {
		syntax_error("Float literal overflow");
	}
	token.kind = TOKEN_FLOAT;
	token.float_val = val;
	token.mod = 0;
}

char escape_to_char[256] = {
	['n'] = '\n',
	['r'] = '\r',
	['t'] = '\t',
	['v'] = '\v',
	['b'] = '\b',
	['a'] = '\a',
	['0'] = '\0',
};

void scan_char() {
	assert(*stream == '\'');
	stream++;

	char val = 0;
	if (*stream == '\'') {
		syntax_error("Char literal cannot be empty");
		stream++;
	}
	else if (*stream == '\n') {
		syntax_error("Char literal cannot contain newline");
	}
	else if (*stream == '\\') {
		stream++;
		val = escape_to_char[*stream];
		if (val == 0 && *stream != '0') {
			syntax_error("Invalid char literal escape '\\%c'", *stream);
		}
		stream++;
	}
	else {
		val = *stream;
		stream++;
	}

	if (*stream != '\'') {
		syntax_error("Expected closing char quote, got '%c'", *stream);
	}
	else {
		stream++;
	}

	token.kind = TOKEN_INT;
	token.int_val = val;
	token.mod = TOKENMOD_CHAR;
}

void scan_str() {
	assert(*stream == '"');
	stream++;

	char *str = NULL;

	while (*stream && *stream != '"') {
		char val = *stream;
		if (val == '\n') {
			syntax_error("String literal cannot contain newline");
		}
		else if (val == '\\') {
			stream++;
			val = escape_to_char[*stream];
			if (val == 0 && *stream != '0') {
				syntax_error("Invalid string literal escape '\\%c'", *stream);
			}
		}
		buf_push(str, val);
		stream++;
	}
	if (*stream == '"') {
		stream++;
	}
	else {
		syntax_error("Unexpected end of file withing string literal");
	}

	buf_push(str, 0);			// zero terminate
	token.kind = TOKEN_STR;
	token.str_val = str;
}

#define CASE1(c, c1, k1) \
  case c: \
    token.kind = *stream++; \
    if (*stream == c1) { \
      token.kind = k1; \
      stream++; \
    } \
    break;

#define CASE2(c, c1, k1, c2, k2) \
  case c: \
    token.kind = *stream++; \
    if (*stream == c1) { \
      token.kind = k1; \
      stream++; \
    } else if (*stream == c2) { \
      token.kind = k2; \
      stream++; \
    } \
    break;


// Scans the next token and sets any relevant data in the 'token' global variable
void next_token() {
  BACK:
	token.mod = 0;
	token.start = stream;
	// WARNING: the '0' ... '9' shortcut is probably not support by most compiles
	switch (*stream) {
	case ' ':
	case '\n':
	case '\r':
	case '\v':
		while (isspace(*stream)) {
			stream++;
		}
		goto BACK;
		break;
	case '\'':
		scan_char();
		break;
	case '"':
		scan_str();
		break;
	case '.':
		scan_float();
		break;
	case '0' ... '9':
		{
			while (isdigit(*stream)) {
				stream++;
			}

			char ch = *stream;
			stream = token.start;
			if (ch == '.' || tolower(ch) == 'e') {
				scan_float();
			}
			else {
				scan_int();
			}
		}
		break;
	case '_':
	case 'a' ... 'z':
	case 'A' ... 'Z':
		{
			while (is_alpha_numeric(*stream) || *stream == '_') {
				stream++;
			}
			token.kind = TOKEN_NAME;
			token.name = str_intern_range(token.start, stream);
		}
		break;
	case '<':
		token.kind = *stream;
		stream++;
		if (*stream == '<') {
			token.kind = TOKEN_LSHIFT;
			stream++;
			if (*stream == '=') {
				token.kind = TOKEN_LSHIFT_ASSIGN;
				stream++;
			}
		}
		else if (*stream == '=') {
			token.kind = TOKEN_LTEQ;
			stream++;
		}
		break;
	case '>':
		token.kind = *stream;
		stream++;
		if (*stream == '>') {
			token.kind = TOKEN_RSHIFT;
			stream++;
			if (*stream == '=') {
				token.kind = TOKEN_RSHIFT_ASSIGN;
				stream++;
			}
		}
		else if (*stream == '=') {
			token.kind = TOKEN_GTEQ;
			stream++;
		}
		break;
		CASE1('^', '=', TOKEN_XOR_ASSIGN);
		CASE1(':', '=', TOKEN_COLON_ASSIGN);
		CASE1('*', '=', TOKEN_MUL_ASSIGN);
		CASE1('/', '=', TOKEN_DIV_ASSIGN);
		CASE1('%', '=', TOKEN_MOD_ASSIGN);
		CASE2('+', '+', TOKEN_INC, '=', TOKEN_ADD_ASSIGN);
		CASE2('-', '-', TOKEN_DEC, '=', TOKEN_SUB_ASSIGN);
		CASE2('&', '&', TOKEN_AND, '=', TOKEN_AND_ASSIGN);
		CASE2('|', '|', TOKEN_OR, '=', TOKEN_OR_ASSIGN);
	default:
		token.kind = *stream++;
		break;
	}
	token.end = stream;
}

#undef CASE1
#undef CASE2

void init_stream(const char *str) {
	stream = str;
	next_token();
}

void print_token(Token token) {
	switch (token.kind) {
	case TOKEN_INT:
		printf("TOKEN INT: %lu\n", token.int_val);
		break;
	case TOKEN_FLOAT:
		printf("TOKEN FLOAT: %lf\n", token.float_val);
		break;
	case TOKEN_NAME:
		printf("TOKEN NAME: %.*s\n", (int)(token.end - token.start), token.start);
		break;
	default:
		printf("TOKEN '%c'\n", token.kind);
		break;
	}
}

size_t copy_token_kind_name(char *dest, size_t dest_size, TokenKind kind) {
	size_t n = 0;
	switch (kind) {
	case 0:
		n = snprintf(dest, dest_size, "end of file");
		break;
	case TOKEN_INT:
		n = snprintf(dest, dest_size, "integer");
		break;
	case TOKEN_FLOAT:
		n = snprintf(dest, dest_size, "float");
		break;
	case TOKEN_NAME:
		n = snprintf(dest, dest_size, "name");
		break;
	default:
		if (kind < 128 && isprint(kind)) {
			n = snprintf(dest, dest_size, "%c", kind);
		}
		else {
			n = snprintf(dest, dest_size, "<ASCII %d>", kind);
		}
	}
	return n;
}

const char *token_kind_str(TokenKind kind) {
	static char buf[256];
	int n = copy_token_kind_name(buf, sizeof(buf), kind);
	assert(n + 1 <= sizeof(buf));
	return buf;
}

bool is_token(TokenKind kind) {
	return token.kind == kind;
}

bool is_token_name(const char *name) {
	return token.kind == TOKEN_NAME && token.name == name;
}

bool match_token(TokenKind kind) {
	if (is_token(kind)) {
		next_token();
		return true;
	}
	return false;
}

bool expect_token(TokenKind kind) {
	if (is_token(kind)) {
		next_token();
		return true;
	}
	fatal("expected token %s, got %s", token_kind_str(kind), token_kind_str(token.kind));
	return false;
}
