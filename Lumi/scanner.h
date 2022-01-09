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
    const char *lexeme;
    int line;

    Token(Token_Type type, const char *lexeme, int line);
    const char *to_string();
};

struct Scanner {
    const char *source;
    std::vector<Token> tokens;

    Scanner(const char *source);
    std::vector<Token> scan_tokens();
};
