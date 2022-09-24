package main

import (
	"fmt"
	"strconv"
)

const (
	// Single-character tokens
	LeftParen = iota
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

	// One or two character tokens
	Bang
	BangEqual
	Equal
	EqualEqual
	Greater
	GreaterEqual
	Less
	LessEqual

	// Literals
	Identifier
	String
	Number

	// Keywords
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

type TokenType int

var TokenTypeString = []string{
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

var keywords = map[string]TokenType{
	"and":    And,
	"class":  Class,
	"else":   Else,
	"false":  False,
	"for":    For,
	"fun":    Fun,
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
	tType   TokenType
	lexeme  string
	literal any
	line    int
}

func (t *Token) String() string {
	return fmt.Sprintf("{Type: %s, Lexeme: %s, Literal: %v}", TokenTypeString[t.tType], t.lexeme, t.literal)
}

type Scanner struct {
	source string
	tokens []Token

	start   int
	current int
	line    int
}

func newScanner(source string) *Scanner {
	return &Scanner{
		source: source,
		line:   1,
		tokens: make([]Token, 0, 10),
	}
}

func (s *Scanner) scanTokens() []Token {
	for !s.isAtEnd() {
		s.start = s.current
		s.scanToken()
	}

	s.tokens = append(s.tokens, Token{tType: Eof, lexeme: "", line: s.line})

	return s.tokens
}

func (s *Scanner) scanToken() {
	switch c := s.advance(); c {
	case '(':
		s.addToken(LeftParen, nil)
	case ')':
		s.addToken(RightParen, nil)
	case '{':
		s.addToken(LeftBrace, nil)
	case '}':
		s.addToken(RightBrace, nil)
	case ',':
		s.addToken(Comma, nil)
	case '.':
		s.addToken(Dot, nil)
	case '-':
		s.addToken(Minus, nil)
	case '+':
		s.addToken(Plus, nil)
	case ';':
		s.addToken(Semicolon, nil)
	case '*':
		s.addToken(Star, nil)
	case '!':
		s.addTokenIfMatch('=', BangEqual, Bang)
	case '=':
		s.addTokenIfMatch('=', EqualEqual, Equal)
	case '<':
		s.addTokenIfMatch('=', LessEqual, Less)
	case '>':
		s.addTokenIfMatch('=', GreaterEqual, Greater)
	case '/':
		if s.match('/') {
			for s.peek() != '\n' && !s.isAtEnd() {
				s.advance()
			}
		} else {
			s.addToken(Slash, nil)
		}
	// Ignore white space
	case ' ':
	case '\r':
	case '\t':

	case '\n':
		s.line++

	case '"':
		s.string()

	default:
		if isDigit(c) {
			s.number()
		} else if isAlpha(c) {
			s.identifier()
		} else {
			reportError(s.line, "Unexpected character.")
		}
	}
}

func (s *Scanner) identifier() {
	for isAlphaNumeric(s.peek()) {
		s.advance()
	}

	lexeme := s.source[s.start:s.current]

	t, isKeyword := keywords[lexeme]
	if !isKeyword {
		t = Identifier
	}
	s.addToken(t, nil)
}

func (s *Scanner) number() {
	for isDigit(s.peek()) {
		s.advance()
	}

	if s.peek() == '.' && isDigit(s.peekNext()) {
		s.advance()
		for isDigit(s.peek()) {
			s.advance()
		}
	}

	n, _ := strconv.ParseFloat(s.source[s.start:s.current], 64)
	s.addToken(Number, n)
}

func (s *Scanner) peekNext() byte {
	if s.isAtEnd() {
		return 0
	}
	return s.source[s.current+1]
}

func (s *Scanner) string() {
	for s.peek() != '"' && !s.isAtEnd() {
		if s.peek() == '\n' {
			s.line++
		}
		s.advance()
	}

	if s.isAtEnd() {
		reportError(s.line, "Unterminated string.")
		return
	}
	s.advance()

	value := s.source[s.start+1 : s.current-1]
	s.addToken(String, value)
}

func (s *Scanner) peek() byte {
	if s.isAtEnd() {
		return 0
	}
	return s.source[s.current]
}

func (s *Scanner) match(c byte) bool {
	if s.source[s.current] == c {
		s.current++
		return true
	}
	return false
}

func (s *Scanner) addTokenIfMatch(c byte, ifMatch, ifDoesNotMatch TokenType) {
	if s.isAtEnd() {
		return
	}

	if s.source[s.current] == c {
		s.addToken(ifMatch, nil)
		s.current++
	} else {
		s.addToken(ifDoesNotMatch, nil)
	}
}

func (s *Scanner) addToken(t TokenType, literal any) {
	s.tokens = append(s.tokens, Token{
		tType:   t,
		lexeme:  s.source[s.start:s.current],
		line:    s.line,
		literal: literal,
	})
}

func (s *Scanner) advance() byte {
	c := s.source[s.current]
	s.current++
	return c
}

func (s *Scanner) isAtEnd() bool {
	return s.current >= len(s.source)
}
