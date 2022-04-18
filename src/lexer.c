#include "lexer.h"

Token token;
const char *stream;

void lex_init(const char *source) {
    stream = source;
}

// Why the big ass switch? Read about jump tables.
void token_next(void) {
    token.start = stream;
    switch (*stream) {
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
        token.type = TK_NUMBER;
        while (isdigit(*stream)) {
            stream++;        
        }
    } break;
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
    case 'Z': {
        stream++;
        token.type = TK_NAME;
        while (isalnum(*stream) || *stream == '_') {
            stream++;
        }
    } break;
    case '"': {
        token.type = TK_STRING;
        while (*stream != '"') {
            if (*stream == '\n') {
            }
            stream++;
        }
    } break;
    default: {
    } break;
    }
    token.end = stream;
}

void token_print(void) {
    switch(token.type) {
    case TK_NAME:
        printf("[TK_NAME] [lexeme: %.*s]\n");
        break;
    case TK_STRING:
        printf("[TK_STRING] [lexeme: %.*s]\n"); 
        break;
    case TK_NUMBER:
        printf("[TK_NUMBER] [lexeme: %.*s]\n");
        break;
    }
}
