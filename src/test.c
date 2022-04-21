#include <assert.h>

void buf_test(void) {
    int buf = NULL;
    enum { N = 1024};

    for (int i = 0; i < N; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == N)

    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }

    buf_free(buf);
    assert(buf == NULL);
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);
}

void intern_str_test(void ) {
    char a[] = "Hi there";
    char b[] = "Hi there";
    const char *pa = intern_str(a);
    const char *pb = intern_str(b);
    assert(pa == pb);

    char c[] = "Hi there.";
    const char *pc = intern_str(c);
    assert(pa != pc);
}

void lex_test(void) {
}
