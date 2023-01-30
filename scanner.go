package main

import "strconv"

type TokenType int

const (
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

type Token struct {
	tokenType TokenType
	lexeme    string
	literal   any
	line      int
}

type Scanner struct {
	source  string
	tokens  []Token
	start   int
	current int
	line    int
}

func newScanner(source string) *Scanner {
	return &Scanner{
		source: source,
		line:   1,
	}
}

func (s *Scanner) scanTokens() []Token {
	for !s.isAtEnd() {
		s.start = s.current
		s.scanToken()
	}

	s.tokens = append(s.tokens, Token{
		tokenType: Eof,
		lexeme:    "",
		line:      s.line,
	})
	return s.tokens
}

func (s *Scanner) scanToken() {
	c := s.advance()
	switch c {
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
		if s.match('=') {
			s.addToken(BangEqual, nil)
		} else {
			s.addToken(Bang, nil)
		}
	case '=':
		if s.match('=') {
			s.addToken(EqualEqual, nil)
		} else {
			s.addToken(Equal, nil)
		}
	case '<':
		if s.match('=') {
			s.addToken(LessEqual, nil)
		} else {
			s.addToken(Less, nil)
		}
	case '>':
		if s.match('=') {
			s.addToken(GreaterEqual, nil)
		} else {
			s.addToken(Greater, nil)
		}
	case '/':
		if s.match('/') {
			for s.peek() != '\n' && !s.isAtEnd() {
				s.advance()
			}
		} else {
			s.addToken(Slash, nil)
		}
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		s.line++
	case '"':
		s.string()
	default:
		if isDigit(c) {
			s.number()
			return
		}
		lumi.error(s.line, "Unexpected Character")
	}
}

func (s *Scanner) addToken(t TokenType, literal any) {
	s.tokens = append(s.tokens, Token{
		tokenType: t,
		lexeme:    s.source[s.start:s.current],
		literal:   literal,
		line:      s.line,
	})
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

	number, err := strconv.ParseFloat(s.source[s.start:s.current], 64)
	if err != nil {
		lumi.error(s.line, "invalid number literal")
		return
	}

	s.addToken(Number, number)
}

func (s *Scanner) string() {
	for s.peek() != '"' && !s.isAtEnd() {
		if s.peek() == '\n' {
			s.line++
		}
		s.advance()
	}

	if s.isAtEnd() {
		lumi.error(s.line, "Unterminated string")
		return
	}

	s.advance()

	lexeme := s.source[s.start+1 : s.current-1]
	s.addToken(String, lexeme)
}

func (s *Scanner) peekNext() byte {
	if (s.current + 1) >= len(s.source) {
		return 0
	}
	return s.source[s.current+1]
}

func (s *Scanner) peek() byte {
	if s.isAtEnd() {
		return 0
	}
	return s.source[s.current]
}

func (s *Scanner) match(ch byte) bool {
	if s.isAtEnd() {
		return false
	}
	if !(s.source[s.current] == ch) {
		return false
	}

	s.current++
	return true
}

func (s *Scanner) advance() byte {
	ch := s.source[s.current]
	s.current++
	return ch
}

func (s *Scanner) isAtEnd() bool {
	return s.current >= len(s.source)
}

func isDigit(ch byte) bool {
	return ch >= '0' && ch <= '9'
}
