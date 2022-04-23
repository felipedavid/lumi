#include <assert.h>
#include <stdlib.h>

#include "common.h"
#include "lexer.h"

extern Token token;

void buf_test(void) {
    int *buf = NULL;
    enum { N = 1024};

    for (int i = 0; i < N; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == N);

    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }

    buf_free(buf);
    assert(buf == NULL);
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);
}

void intern_str_test(void) {
    char a[] = "Hi there";
    char b[] = "Hi there";
    const char *pa = str_intern(a);
    const char *pb = str_intern(b);
    assert(pa == pb);

    char c[] = "Hi there.";
    const char *pc = str_intern(c);
    assert(pa != pc);
}

void lex_test(void) {
#define TEST_TYPE(t) (token_next(), assert(token.type == (t)))

    lex_init("(2+3)*5 >= a");
    TEST_TYPE('(');
    TEST_TYPE(TOKEN_NUMBER);
    TEST_TYPE('+');
    TEST_TYPE(TOKEN_NUMBER);
    TEST_TYPE(')');
    TEST_TYPE('*');
    TEST_TYPE(TOKEN_NUMBER);
    TEST_TYPE(TOKEN_GTEQ);
    TEST_TYPE(TOKEN_NAME);

    lex_init("\"hello\" == \"there\"");
    TEST_TYPE(TOKEN_STRING);
    TEST_TYPE(TOKEN_EQEQ);
    TEST_TYPE(TOKEN_STRING);
#undef TEST_TYPE
}
