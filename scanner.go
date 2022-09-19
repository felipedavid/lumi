package main

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

type Token struct {
	tType   TokenType
	lexeme  string
	literal any
	line    int
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

	default:
		reportError(s.line, "Unexpected character.")
	}
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
