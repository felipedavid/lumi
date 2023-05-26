#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

#include "common.c"
#include "lexer.c"
#include "ast.c"
#include "tests.c"

int main(int argc, char **argv) {
	run_tests();
	return 0;
}
