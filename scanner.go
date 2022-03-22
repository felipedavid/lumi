package main

import (
    "fmt"
)

type TokenType int

const (
    LeftParen TokenType = iota
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
    Type TokenType
    Lexeme string
    Literal interface{}
    Line int
}

func (t *Token) ToString() string {
    return fmt.Sprintf("%v %v %v", t.Type, t.Lexeme, t.Literal)
}

func NewToken(t TokenType, lexeme string, literal interface{}, line int) *Token {
    return &Token{t, lexeme, literal, line}
}

type Scanner struct {
    source string
    tokens []*Token

    start int
    current int
    line int
}

func NewScanner(source string) *Scanner {
    return &Scanner{source, make([]*Token, 0), 0, 0, 1}
}

func (s *Scanner) scanTokens() []*Token {
    for !s.isAtEnd() {
        s.start = s.current
        s.scanToken()
    }

    s.tokens = append(s.tokens, NewToken(Eof, "", nil, s.line))
    return s.tokens
}

func (s *Scanner) scanToken() {
    c := s.advance()

    switch c {
    case '(': s.addToken(LeftParen, nil)
    case ')': s.addToken(RightParen, nil)
    case '{': s.addToken(LeftBrace, nil)
    case '}': s.addToken(RightBrace, nil)
    case ',': s.addToken(Comma, nil)
    case '.': s.addToken(Dot, nil)
    case '-': s.addToken(Minus, nil)
    case '+': s.addToken(Plus, nil)
    case ';': s.addToken(Semicolon, nil)
    case '*': s.addToken(Star, nil)
    default:
        erro(s.line, "Unexpected character.") 
    }
}

func (s *Scanner) isAtEnd() bool {
    return s.current >= len(s.source)
}

func (s *Scanner) advance() rune {
    s.current += 1
    return rune(s.source[s.current - 1])
}

func (s *Scanner) addToken(t TokenType, literal interface{}) {
    lexeme := s.source[s.start:s.current]
    s.tokens = append(s.tokens, NewToken(t, lexeme, literal, s.line))
}

