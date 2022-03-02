
enum Token_Type {
    TOKEN_IDENT = 256,
    TOKEN_NUMBER,
    TOKEN_ERROR,
};

struct Token {
    Token_Type type = TOKEN_ERROR;   
};

struct Lexer {
    Token curr_tk;
    const char *source;

    void init(const char *source);
    Token *get_next_token();
};
