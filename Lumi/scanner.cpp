#include <cstdlib>
#include <string>

#include "scanner.h"

// WARNING: "buf" is stack allocated so it will be messed up in the
// next function call, so, be careful.
const char *Token::to_string() {
    char buf[256];
    snprintf(buf, sizeof(buf), "%d %.*s %d\n", type, (int)(lexeme_end - lexeme_start), lexeme_start, line);
    return buf;
}

Scanner::Scanner(const char *source) {
    this->source = source;
    this->source_len = strlen(source);
}

Token_Type Scanner::scan_token(Token *tk) {
    tk->lexeme_start = source;
    switch (*source) {
          case '(': tk->type = LEFT_PAREN; break;
          case ')': tk->type = RIGHT_PAREN; break;
          case '{': tk->type = LEFT_BRACE; break;
          case '}': tk->type = RIGHT_BRACE; break;
          case ',': tk->type = COMMA; break;
          case '.': tk->type = DOT; break;
          case '-': tk->type = MINUS; break;
          case '+': tk->type = PLUS; break;
          case ';': tk->type = SEMICOLON; break;
          case '*': tk->type = STAR; break;
          default: tk->type = EOFF; break;
    }
    tk->line = line;
    tk->lexeme_end = source;
    return tk->type;
}

std::vector<Token> Scanner::scan_tokens() {
    Token tk;
    while (scan_token(&tk) != EOFF) {
        tokens.push_back(tk);
    }
    tokens.push_back(tk);
    return tokens;
}

bool Scanner::is_at_end() {
    return current >= source_len;
}
