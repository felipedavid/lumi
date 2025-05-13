from enum import Enum, auto
from typing import Any


class TokenKind(Enum):
    # Single-character tokens
    LEFT_PAREN = auto()
    RIGHT_PAREN = auto()
    LEFT_BRACE = auto()
    RIGHT_BRACE = auto()
    COMMA = auto()
    DOT = auto()
    MINUS = auto()
    PLUS = auto()
    SEMICOLON = auto()
    SLASH = auto()
    STAR = auto()

    # One or two character tokens
    BANG = auto()
    BANG_EQUAL = auto()
    EQUAL = auto()
    EQUAL_EQUAL = auto()
    GREATER = auto()
    GREATER_EQUAL = auto()
    LESS = auto()
    LESS_EQUAL = auto()

    # Literals
    IDENTIFIER = auto()
    STRING = auto()
    NUMBER = auto()

    # Keywords
    AND = auto()
    CLASS = auto()
    ELSE = auto()
    FALSE = auto()
    FUN = auto()
    FOR = auto()
    IF = auto()
    NIL = auto()
    OR = auto()
    PRINT = auto()
    RETURN = auto()
    SUPER = auto()
    THIS = auto()
    TRUE = auto()
    VAR = auto()
    WHILE = auto()
    EOF = auto()


class Token:
    kind: TokenKind
    lexeme: str
    literal: Any
    line: int

    def __init__(self, kind: TokenKind, lexeme: str, literal: Any, line: int):
        self.kind = kind
        self.lexeme = lexeme
        self.literal = literal
        self.line = line

    def __str__(self):
        return f"{self.kind} {self.lexeme} {self.literal}"


class Scanner:
    source: str
    tokens: list[Token] = []
    line: int = 1
    start: int = 0
    current: int = 0

    def __init__(self, source: str):
        self.source = source

    def scan_tokens(self) -> list[Token]:
        while not self.is_at_end():
           self.start = self.current 
           self.scan_token()
        
        self.tokens.append(Token(TokenKind.EOF, '', None, self.line))

        return self.tokens

    def scan_token(self) -> None:
        ch = self.advance()

        single_char_tokens_kinds = {
            '(': TokenKind.LEFT_PAREN,
            ')': TokenKind.RIGHT_PAREN,
            '{': TokenKind.LEFT_BRACE,
            '}': TokenKind.RIGHT_BRACE,
            ',': TokenKind.COMMA,
            '.': TokenKind.DOT,
            '-': TokenKind.MINUS,
            '+': TokenKind.PLUS,
            ';': TokenKind.SEMICOLON,
            '*': TokenKind.STAR,
        }

        if kind := single_char_tokens_kinds.get(ch):
            self.add_token(kind)

    def add_token(self, kind: TokenKind, literal: Any = None) -> None:
        lexeme = self.source[self.start:self.current]
        self.tokens.append(Token(kind, lexeme, literal, self.line))

    def advance(self) -> None:
        ch = self.source[self.current]
        self.current += 1
        return ch

    def is_at_end(self) -> bool:
        return self.current >= len(self.source)
    