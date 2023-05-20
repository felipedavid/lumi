#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

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

void *xmalloc(size_t n_bytes) {
	void *ptr = malloc(n_bytes);
	if (ptr == NULL) {
		fprintf(stderr, "xmalloc failed");
		exit(1);
	}
	return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
	ptr = realloc(ptr, n_bytes);
	if (ptr == NULL) {
		fprintf(stderr, "xrealloc failed");
		exit(1);
	}
	return ptr;
}

// STRETCHY BUFFERS 
// In almost every dynamic array implementation people access elements using a
// function call like: "array_acess(arr_ptr, index)". I don't like that, I
// would like to access vectors with the same notation used by static arrays
// "arr[index]". Turns out that this is possible by doing some clever pointer
// arithmetic, credits to Shawn Barret for comming out with the idea.
//
// Memory layout:
// [Header][content]
//         ^-- arr_ptr
typedef struct {
	size_t len;
	size_t cap;
	char content[0];
} Buf_Header;

// Macros/functions that start with the word buf followed by two underscores
// are for internal implementation and should not be used by used directly
//
// Why we are choosing to use macros? 
// 	- Some operations like "buf_push" needs to reassign the pointer to the
// 	buffer, we could use a function but it would not be as clean
#define buf__header(b) ((b) ? (Buf_Header *)((char*)(b) - offsetof(Buf_Header, content)) : 0)
#define buf__fits(b, n) 
#define buf__make_fit(b, n) (((buf_len(b)+n) < buf_cap(b)) ? 0 : ((b) = buf__grow((b), (buf_len(b)+n), sizeof(*b))))

#define buf_len(b) ((b) ? buf__header(b)->len : 0)
#define buf_cap(b) ((b) ? buf__header(b)->cap : 0)
#define buf_end(b) ((b) + buf_len(b))
#define buf_push(b, ...) (buf__make_fit((b), 1), (b)[buf__header(b)->len++] = (__VA_ARGS__))
#define buf_free(b) ((b) ? (free(buf__header(b)), (b) = NULL) : 0)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void *buf__grow(const void *buf, size_t min_cap, size_t elem_size) {
	// Most of the time we just double the size of the buffer, but sometimes that
	// is not enough so we increase the capacity to fit 'min_cap'
	size_t new_cap = MAX(buf_cap(buf)*2+1, min_cap);
  assert(min_cap <= new_cap);

	// Calculate the actual size of the buffer to allocate (making sure to not
	// forget about the header)
	size_t new_size = sizeof(Buf_Header) + (elem_size * new_cap);

	Buf_Header *new_buffer = NULL;
	if (buf == NULL) {
		new_buffer = xmalloc(new_size);
		new_buffer->len = 0;
	} else {
		new_buffer = xrealloc(buf__header(buf), new_size);
	}
	new_buffer->cap = new_cap;

	return (void *) new_buffer->content;
}

// Helpers
typedef int bool;
#define true 1;
#define false 0;

bool is_digit(char ch) {
	return (ch >= '0' && ch <= '9');
}

bool is_alpha_numeric(char ch) {
	return (is_digit(ch) || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

// Lexing
typedef enum {
	FIRST_NONCHAR_TOKEN = 128,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_NAME,
} TokenKind;

typedef struct {
	TokenKind kind;
	const char *start;
	const char *end;
	union {
		uint64_t int_val;
    double float_val;
		const char *name;
	};
} Token;

const char *stream = "abc";
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
  ['a'] = 10, ['A'] = 10,
  ['b'] = 11, ['B'] = 11,
  ['c'] = 12, ['C'] = 12,
  ['d'] = 13, ['D'] = 13,
  ['e'] = 14, ['E'] = 14,
  ['f'] = 15, ['F'] = 15,
};

int64_t scan_int() {
  uint64_t base = 10;

  // Handle hex values
  if (*stream == '0') {
    stream++;
    if (tolower(*stream) == 'x') {
      stream++;
      base = 16;
    } else if (tolower(*stream) == 'b') {
      stream++;
      base = 2;
    } else if (isdigit(tolower(*stream))) {
      base = 8;
    } else {
      syntax_error("Invalid integer literal suffix '%c'", *stream);
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
    if (val > (UINT64_MAX - digit)/base) {
      syntax_error("Integer literal overflow");
      while (isdigit(*stream)) {
        stream++;
      }
      val = 0;
    }
    val = val * base + digit;
    stream++;
  }
  return val;
}

double scan_float() {
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
  return val;
}

// Scans the next token and sets any relevant data in the 'token' global variable
void next_token() {
BACK:
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
  case '.':
      token.kind = TOKEN_FLOAT;
      token.float_val = scan_float();
      break;
	case '0' ... '9': {
    while (isdigit(*stream)) {
      stream++;
    }
    if (*stream == '.' || tolower(*stream) == 'e') {
      stream = token.start;
      token.kind = TOKEN_FLOAT;
      token.float_val = scan_float();
    } else {
      stream = token.start;
      token.kind = TOKEN_INT;
      token.int_val = scan_int();
    }
	}	break;
	case '_':
	case 'a' ... 'z':
	case 'A' ... 'Z': {
		while (is_alpha_numeric(*stream) || *stream == '_') {
			stream++;
		}
		token.kind = TOKEN_NAME;
		token.name = str_intern_range(token.start, stream);
	} break;
	default:
		token.kind = *stream++;
		break;
	}
	token.end = stream;
}

// String interning
typedef struct {
	size_t len;
	const char *str;
} Intern;

Intern *interns;

const char *str_intern_range(const char *start, const char *end) {
	size_t len = (end - start);
  for (Intern *it = interns; it != buf_end(interns); it++) {
    if (it->len == len && strncmp(it->str, start, len) == 0) {
      return it->str;
    }
  }

	char *str = malloc(len+1);
	memcpy(str, start, len);
	str[len] = '\0';

	buf_push(interns, ((Intern){len, str}));

	return str;
}

const char *str_intern(const char *str) {
	return str_intern_range(str, (str + strlen(str)));
}

void buf_test() {
	int *buf = NULL; // Always make sure to initialize the stretchy buffer to zero

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
		} else {
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

#define assert_token(x) assert(match_token(x))
#define assert_token_name(x) assert(token.name == str_intern(x) && match_token(TOKEN_NAME))
#define assert_token_int(x) assert(token.int_val == (x) && match_token(TOKEN_INT))
#define assert_token_float(x) assert(token.float_val == (x) && match_token(TOKEN_FLOAT))
#define assert_token_eof() assert(is_token(0))

void lex_test() {
  // Integer literal tests
  init_stream("18446744073709551615 0xffffffffffffffff 042 0b1111");
  assert_token_int(18446744073709551615ull);
  assert_token_int(0xffffffffffffffffull);
  assert_token_int(042);
  assert_token_int(0xF);
  assert_token_eof();

  // Float literals tests
  init_stream("3.14 .123 42. 3e10");
  assert_token_float(3.14);
  assert_token_float(.123);
  assert_token_float(42.);
  assert_token_float(3e10);
  assert_token_eof();

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

int main(int argc, char **argv) {
	buf_test();
	lex_test();
	intern_test();
}
