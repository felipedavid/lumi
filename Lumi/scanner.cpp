#include <cstdlib>
#include <string>

#include "scanner.h"
#include "common.h"

// WARNING: "buf" is stack allocated so it will be messed up in the
// next function call, so, be careful.
const char *Token::to_string() {
    char buf[256];
    snprintf(buf, sizeof(buf), "type: %d, lexeme: %.*s, line: %d\n", 
        type, (int)(lexeme_end - lexeme_start), lexeme_start, line);
    return buf;
}

Scanner::Scanner(const char *source) {
    this->source = source;
    this->source_len = strlen(source);
}

Token_Type Scanner::scan_token(Token *tk) {
    tk->type = NONE;
    tk->lexeme_start = source;
    tk->line = line;

    switch (*source++) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        source--;
        int val = 0;
        while (isdigit(*source)) {
            val *= 10;
            val *= *source++ - '0';
        }
        tk->val = val;
        tk->type = NUMBER;
        break;
    }
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z': 
        while (isalpha(*source) || *source == '_') source++;
        tk->name = str_intern_range(tk->lexeme_start, source);
        tk->type = IDENTIFIER;
        break;
    case '(': tk->type = LEFT_PAREN;  break;
    case ')': tk->type = RIGHT_PAREN; break;
    case '{': tk->type = LEFT_BRACE;  break;
    case '}': tk->type = RIGHT_BRACE; break;
    case ',': tk->type = COMMA;       break;
    case '.': tk->type = DOT;         break;
    case '-': tk->type = MINUS;       break;
    case '+': tk->type = PLUS;        break;
    case ';': tk->type = SEMICOLON;   break;
    case '*': tk->type = STAR;        break;
    case '!': 
        if (*source == '=') {
            tk->type = BANG_EQUAL;
            source++;
        } else tk->type = BANG;
        break;
    case '=': 
        if (*source == '=') {
            tk->type = EQUAL_EQUAL;
            source++;
        } else tk->type = EQUAL;
        break;
    case '<': 
        if (*source == '=') {
            tk->type = LESS_EQUAL;
            source++;
        } else tk->type = LESS;
        break;
    case '>': 
        if (*source == '=') {
            tk->type = GREATER_EQUAL;
            source++;
        } else tk->type = GREATER;
        break;
    case '/': 
        if (*source == '/') while (*(++source) != '\n');
        else tk->type = SLASH;
        break;
    case '"':
        tk->type = STRING;
        while (*source != '"' && *source != '\0') {
            if (*source == '\n') line++;
            source++;
        }
        if (*source == '\0') {
            fprintf(stderr, "Not ending string on line %d.\n", tk->line);
        } else source++;
    case ' ':
    case '\r':
    case '\t':
      // Ignore white spaces
      break;
    case '\n': line++; break;
    default: tk->type = EOFF; break;
    }

    tk->lexeme_end = source;
    return tk->type;
}

std::vector<Token> Scanner::scan_tokens() {
    Token tk;
    while (scan_token(&tk) != EOFF) {
		if (tk.type != NONE) tokens.push_back(tk);
    }
    tokens.push_back(tk);
    return tokens;
}

bool Scanner::is_at_end() {
    return current >= source_len;
}
