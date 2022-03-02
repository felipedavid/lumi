#include <stdio.h>
#include <ctype.h>

#include "lexer.h"

void Token::log() {
    switch (type) {
    case TOKEN_NUMBER:
        printf("TOKEN( type: TOKEN_NUMBER, val: %llu )\n", int_val);
        break;
    default:
        printf("TOKEN( type: %d, val: %.*s )\n", type, end - start, start);
        break;
    }
}

void Lexer::init(const char *source) {
    this->source = source;
}


// If (*source++) == ch, returns type
// else return ch
#define match(ch, type) ((Token_Type)(*(++source) == ch ? source++, type : *(source-1)))

Token *Lexer::get_next_token() {
LOOP:
    curr_tk.start = source;
    switch (*source) {
    case '0' ... '9': {
        u64 val = 0;
        while (isdigit(*source)) {
            val *= 10;
            val += *source - '0';
            source++;
        }
        curr_tk.int_val = val;
        curr_tk.type = TOKEN_NUMBER;   
    } break;
    case 'a' ... 'z':
    case 'A' ... 'Z': {
        while (isalnum(*source) || *source == '_') {
            source++;
        }
        curr_tk.type = TOKEN_IDENT;
    } break;
	case '!': curr_tk.type = match('=', TOKEN_BANG_EQUAL);    break;
	case '=': curr_tk.type = match('=', TOKEN_EQUAL_EQUAL);   break;
	case '<': curr_tk.type = match('=', TOKEN_LESS_EQUAL);    break;
	case '>': curr_tk.type = match('=', TOKEN_GREATER_EQUAL); break;
	case ' ': 
	case '\t':
	case '\r':
		source++;
		goto LOOP;
		break;
	case '\n': 
		line++; 
		goto LOOP;
		break;
    default:
        curr_tk.type = (Token_Type) *source++;
        break;
    }
    curr_tk.end = source;

    return &curr_tk;
}
