package main

import (
	"errors"
	"fmt"
	"strconv"
)

var (
	ErrUnexpectedCharacter = errors.New("lexer: unexpected character")
	ErrUnterminatedString  = errors.New("lexer: unterminated string")
)

type TokenKind int

const (
	None TokenKind = iota
	LeftParen
	RightParen
	LeftBrace
	RightBrace
	Comma
	Dot
	Minus
	Plus
	Semicolon
	Slash
	Star
	Bang
	BangEqual
	Equal
	EqualEqual
	Greater
	GreaterEqual
	Less
	LessEqual
	Identifier
	String
	Number
	And
	Class
	Else
	False
	Fun
	For
	If
	Nil
	Or
	Print
	Return
	Super
	This
	True
	Var
	While
	Eof
)

var tokenTypeStr = []string{
	LeftParen:    "LeftParen",
	RightParen:   "RightParen",
	LeftBrace:    "LeftBrace",
	RightBrace:   "RightBrace",
	Comma:        "Comma",
	Dot:          "Dot",
	Minus:        "Minus",
	Plus:         "Plus",
	Semicolon:    "Semicolon",
	Slash:        "Slash",
	Star:         "Star",
	Bang:         "Bang",
	BangEqual:    "BangEqual",
	Equal:        "Equal",
	EqualEqual:   "EqualEqual",
	Greater:      "Greater",
	GreaterEqual: "GreaterEqual",
	Less:         "Less",
	LessEqual:    "LessEqual",
	Identifier:   "Identifier",
	String:       "String",
	Number:       "Number",
	And:          "And",
	Class:        "Class",
	Else:         "Else",
	False:        "False",
	Fun:          "Fun",
	For:          "For",
	If:           "If",
	Nil:          "Nil",
	Or:           "Or",
	Print:        "Print",
	Return:       "Return",
	Super:        "Super",
	This:         "This",
	True:         "True",
	Var:          "Var",
	While:        "While",
	Eof:          "Eof",
}

var keywords = map[string]TokenKind{
	"and":    And,
	"class":  Class,
	"else":   Else,
	"false":  False,
	"for":    Fun,
	"if":     If,
	"nil":    Nil,
	"or":     Or,
	"print":  Print,
	"return": Return,
	"super":  Super,
	"this":   This,
	"true":   True,
	"var":    Var,
	"while":  While,
}

type Token struct {
	kind   TokenKind
	lexeme string
	value  any
	line   int
}

func (t *Token) str() string {
	return fmt.Sprintf("[kind: %s] [value: %v] [lexeme: \"%s\"] [line: %d]",
		tokenTypeStr[t.kind], t.value, t.lexeme, t.line)
}

func newToken(kind TokenKind, lexeme string, value any, line int) *Token {
	return &Token{
		kind:   kind,
		lexeme: lexeme,
		value:  value,
		line:   line,
	}
}

type Lexer struct {
	source  string
	tokens  []*Token
	start   int
	current int
	line    int
}

func newLexer(source string) *Lexer {
	return &Lexer{
		source:  source,
		tokens:  []*Token{},
		start:   0,
		current: 0,
		line:    1,
	}
}

func (l *Lexer) parseTokens() ([]*Token, error) {
	for !l.isAtEnd() {
		l.start = l.current
		err := l.parseToken()
		if err != nil {
			return nil, err
		}
	}

	l.tokens = append(l.tokens, newToken(Eof, "", nil, l.line))
	return l.tokens, nil
}

func (l *Lexer) parseToken() error {
	switch ch := l.advance(); ch {
	case '(':
		l.addToken(LeftParen, nil)
	case ')':
		l.addToken(RightParen, nil)
	case '{':
		l.addToken(LeftBrace, nil)
	case '}':
		l.addToken(RightBrace, nil)
	case ',':
		l.addToken(Comma, nil)
	case '.':
		l.addToken(Dot, nil)
	case '-':
		l.addToken(Minus, nil)
	case '+':
		l.addToken(Plus, nil)
	case ';':
		l.addToken(Semicolon, nil)
	case '*':
		l.addToken(Star, nil)
	case '!':
		l.addTokenIf(l.match('='), BangEqual, Bang)
	case '/':
		if l.match('/') {
			for l.peek() != '\n' && !l.isAtEnd() {
				l.advance()
			}
		} else {
			l.addToken(Slash, nil)
		}
	case '\n':
		l.line++
	case ' ':
	case '\r':
	case '\t':

	case '"':
		for l.peek() != '"' && !l.isAtEnd() {
			// Yes. we have multi line strings
			if l.peek() == '\n' {
				l.line++
			}
			l.advance()
		}

		if l.isAtEnd() {
			return ErrUnterminatedString
		}

		// Consume the closing '"'
		l.advance()

		l.addToken(String, l.source[l.start+1:l.current-1])
	default:
		if isDigit(ch) {
			for isDigit(l.peek()) {
				l.advance()
			}

			if l.peek() == '.' && isDigit(l.peekNext()) {
				l.advance()
				for isDigit(l.peek()) {
					l.advance()
				}
			}
			value, _ := strconv.ParseFloat(l.source[l.start:l.current], 64)
			l.addToken(Number, value)
		} else if isAlpha(ch) {
			for isAlphaNumeric(l.peek()) {
				l.advance()
			}
			kind := keywords[l.source[l.start:l.current]]
			if kind == None {
				kind = Identifier
			}
			l.addToken(kind, nil)
		} else {
			return ErrUnexpectedCharacter
		}
	}
	return nil
}

func (l *Lexer) peek() byte {
	if l.isAtEnd() {
		return 0
	}
	return l.source[l.current]
}

func (l *Lexer) peekNext() byte {
	if l.current+1 >= len(l.source) {
		return 0
	}
	return l.source[l.current+1]
}

func (l *Lexer) addTokenIf(condition bool, ifTrue TokenKind, ifFalse TokenKind) {
	if condition {
		l.addToken(ifTrue, nil)
	} else {
		l.addToken(ifFalse, nil)
	}
}

func (l *Lexer) match(ch byte) bool {
	if l.isAtEnd() {
		return false
	}
	if l.source[l.current] != ch {
		return false
	}

	l.current++
	return true
}

func (l *Lexer) addToken(kind TokenKind, value any) {
	substring := l.source[l.start:l.current]
	l.tokens = append(l.tokens, newToken(kind, substring, value, l.line))
}

func (l *Lexer) advance() byte {
	ch := l.source[l.current]
	l.current++
	return ch
}

func (l *Lexer) isAtEnd() bool {
	return l.current >= len(l.source)
}
