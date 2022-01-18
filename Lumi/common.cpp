#include <vector>
#include <assert.h>

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        perror("[!] xmalloc failed!");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *prev_ptr, size_t n_bytes) {
    void *ptr = realloc(prev_ptr, n_bytes);
    if (!ptr) {
        perror("[!] xrealloc failed!");
        exit(1);
    }
    return ptr;
}

struct Interned_Str{
    const char *str;
    size_t len;
};
std::vector<Interned_Str> interned_strings;

const char *str_intern_range(const char *start, const char *end) {
    // Checking if the strings is alredy interned
    size_t len = (size_t) (end - start); 
    for (int i = 0; i < interned_strings.size(); i++) {
        if (interned_strings[i].len == len && strncmp(interned_strings[i].str, start, len)) {
            return interned_strings[i].str;
        }
    }

    // Interning a new string
    char *str = (char *) xmalloc(len + 1);
    strncpy_s(str, len, start, len);
    str[len] = 0;
    interned_strings.push_back(Interned_Str{str, len});

    return str;
}

// Handle zero terminated strings
const char *str_intern(const char *start) {
    return str_intern_range(start, start + strlen(start));
}

void intern_test() {
    char x[] = "Hello";
    char y[] = "Hello";
    const char *px = str_intern(x);
    const char *py = str_intern(y);
    assert(px == py);

    char z[] = "Hello!";
    const char *pz = str_intern(z);
    assert(pz != px);
}
