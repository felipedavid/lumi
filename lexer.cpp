#include <ctype.h>

#include "lexer.h"

void Lexer::init(const char *source) {
    this->source = source;
}

Token *Lexer::get_next_token() {
    switch (*source) {
        case '0' ... '9': {
            while (isdigit(*source)) {
                source++;
            }
            curr_tk.type = TOKEN_NUMBER;   
        } break;
        case 'a' ... 'z':
        case 'A' ... 'Z': {
            while (isalnum(*source) || *source == '_') {
                source++;
            }
            curr_tk.type = TOKEN_IDENT;
        } break;
        default:
            curr_tk.type = (Token_Type) *source++;
            break;
    }

    return &curr_tk;
}
