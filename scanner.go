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

var keywords = map[string]TokenType{
	"and":    And,
	"break":  Break,
	"class":  Class,
	"else":   Else,
	"false":  False,
	"for":    For,
	"fun":    Fun,
	"get":    Getter,
	"if":     If,
	"nil":    Nil,
	"or":     Or,
	"print":  Print,
	"return": Return,
	"set":    Setter,
	"static": Static,
	"super":  Super,
	"this":   This,
	"true":   True,
	"var":    Var,
	"while":  While,
}

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
    case '!': s.addIfMatch('=', BangEqual, Bang)
    case '=': s.addIfMatch('=', EqualEqual, Equal)
    case '<': s.addIfMatch('=', LessEqual, Less)
    case '>': s.addIfMatch('=', GreaterEqual, Greater)
    case '/':
        if s.match('/' {
            for s.peek() != '\n' && !s.isAtEnd() {
                s.advance()
            }
        } else {
            s.addToken(Slash, nil)
        }
    case ' ':
    case '\r':
    case '\t':
        break
    case '\n': s.line++
    case '"': s.str()
    default:
        if isDigit(c) {
            s.number()
        } else if isAlpha(c) {
            s.identifier()
        } else {
            erro(s.line, "Unexpected character.") 
        }
    }
}

func (s *Scanner) identifier() {
    for isAlphaNumeric(s.peek()) {
        s.advance()
    }

    lexeme := s.source[s.start:s.current]
    t := keywords[lexeme]

    if t == nil {
        t = Identifier
    }

    addToken(t, nil)
}

func (s *Scanner) number() {
    for isDigit(s.peek()) {
        s.advance()
    }

    if s.peek() == '.' && isDigit(s.peekNext()) {
        advance()

        for isDigit(s.peek()) {
            s.advance()
        }
    }

    s.addToken(Number, strconv.ParseFloat(s.source[s.start:s.current], 32))
}

func (s *Scanner) str() {
    for s.peek() != '"' && !s.isAtEnd() {
        if s.peek() == '\n' {
            s.line++
        }
        s.advance()
    }

    if s.isAtEnd() {
        erro(s.line, "Unterminated string.")
        return
    }

    s.advance()

    value := s.source[s.start + 1 : s.current - 1]
    s.addToken(String, value)
}

func (s *Scanner) match(expected rune) bool {
    if s.isAtEnd() {
        return false
    }
    if rune(s.source[s.current]) != expected {
        return false
    }

    s.current++
    return true
}

func (s *Scanner) peek() rune {
    if s.isAtEnd() {
        return '\0'
    }
    return rune(s.source[s.current])
}

func (s *Scanner) peekNext() rune {
    if s.current + 1 >= len(s.source) {
        return '\0'
    }
    return s.source[s.current + 1]
}

func (s *Scanner) isAtEnd() bool {
    return s.current >= len(s.source)
}

func (s *Scanner) advance() rune {
    return rune(s.source[s.current++])
}

func (s *Scanner) addToken(t TokenType, literal interface{}) {
    lexeme := s.source[s.start:s.current]
    s.tokens = append(s.tokens, NewToken(t, lexeme, literal, s.line))
}

func (s *Scanner) addIfMatch(ch rune, t1 TokenType, t2 TokenType) [
    if s.match(ch) {
        s.addToken(t1, nil)
    } else {
        s.addToken(t2, nil)
    }
}

func isDigit(ch rune) bool {
    return c >= '0' && c <= '9'
}

func isAlpha(ch rune) bool {
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
            ch == '_'
}

func isAlphaNumeric(ch rune) bool {
    return isDigit(ch) || isAlpha(ch)
}
