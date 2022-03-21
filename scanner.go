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
