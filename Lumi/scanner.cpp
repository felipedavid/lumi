#include <cstdlib>
#include <string>

#include "scanner.h"

Token::Token(Token_Type type, const char *lexeme, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->line = line;
}

// WARNING: "buf" is stack allocated so it will be messed up in the
// next function call, so, be careful.
const char *Token::to_string() {
    char buf[256];
    snprintf(buf, sizeof(buf), "%d %s %d\n", type, lexeme, line);
    return buf;
}

Scanner::Scanner(const char *source) {
    this->source = source;
}

std::vector<Token> scan_tokens() {
    return tokens;
}
