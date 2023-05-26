void *xcalloc(size_t n_items, size_t n_bytes) {
	void *ptr = calloc(n_items, n_bytes);
	if (ptr == NULL) {
		fprintf(stderr, "xcalloc failed");
		exit(1);
	}
	return ptr;
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
//  - Some operations like "buf_push" needs to reassign the pointer to the
//  buffer, we could use a function but it would not be as clean
#define BUF(x) x
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
	size_t new_cap = MAX(buf_cap(buf) * 2 + 1, min_cap);
	assert(min_cap <= new_cap);

	// Calculate the actual size of the buffer to allocate (making sure to not
	// forget about the header)
	size_t new_size = sizeof(Buf_Header) + (elem_size * new_cap);

	Buf_Header *new_buffer = NULL;
	if (buf == NULL) {
		new_buffer = xmalloc(new_size);
		new_buffer->len = 0;
	}
	else {
		new_buffer = xrealloc(buf__header(buf), new_size);
	}
	new_buffer->cap = new_cap;

	return (void *)new_buffer->content;
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

// String interning
typedef struct {
	size_t len;
	const char *str;
} Intern;

Intern *interns;

const char *str_intern_range(const char *start, const char *end) {
	size_t len = (end - start);
	for (Intern * it = interns; it != buf_end(interns); it++) {
		if (it->len == len && strncmp(it->str, start, len) == 0) {
			return it->str;
		}
	}

	char *str = malloc(len + 1);
	memcpy(str, start, len);
	str[len] = '\0';

	buf_push(interns, ((Intern) {
					   len, str}));

	return str;
}

const char *str_intern(const char *str) {
	return str_intern_range(str, (str + strlen(str)));
}
