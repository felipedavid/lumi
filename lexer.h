enum Token_Type {
    TOKEN_NAME = 256,
    TOKEN_NUMBER,
};

struct Token {
    Token_Type type;
    const char *start;
    const char *end;
    union {
        double val;
        const char *str;
    };
};

struct Lexer {
    const char *source;
    Token token;

    void init(const char *source);
    Token *next_token();
    void log_current_token();
};
