#include <vector>

enum Token_Type {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EOFF
};

struct Token {
    Token_Type type;
    const char *lexeme_start;
    const char *lexeme_end;
    int line;

    const char *to_string();
};

struct Scanner {
    const char *source;
    size_t source_len;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    Scanner(const char *source);
    Token_Type scan_token(Token *tk);
    std::vector<Token> scan_tokens();
    bool is_at_end();
};
