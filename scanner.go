package main

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

func newScanner(source []byte) *Scanner {
	return &Scanner{
		source: string(source),
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
	default:
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

func (s *Scanner) advance() byte {
	ch := s.source[s.current]
	s.current++
	return ch
}

func (s *Scanner) isAtEnd() bool {
	return s.current >= len(s.source)
}
