#include <stdio.h>
#include <assert.h>

#include "lexer.h"
#include "compiler.h"

extern Token token;
Chunk *compiler_chunk;

void parse_number() {
    double val = strtod(token.start, NULL);
    chunk_push_constant(compiler_chunk, OP_CONSTANT); 
}

void parser_expression() {
    
}

void parser_consume(Token_Type type, const char *message) {
    assert(token.type == type);
    token_next();
}

bool compiler_compile(const char *source, Chunk *chunk) {
    compiler_chunk = chunk;
    lex_init(source);
    parser_expression();
    parser_consume(TOKEN_EOF, "Expected end of expression.");
    return true;
}
