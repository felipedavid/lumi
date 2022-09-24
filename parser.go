package main

import "fmt"

/*
expression → comma ;
comma      → equality ("," equality)*;
equality   → comparison ( ( "!=" | "==" ) comparison )* ;
comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term       → factor ( ( "-" | "+" ) factor )* ;
factor     → unary ( ( "/" | "*" ) unary )* ;
unary      → ( "!" | "-" ) unary
 	         | primary ;
primary    → NUMBER | STRING | "true" | "false" | "nil"
			 | "(" expression ")" ;
*/

type Parser struct {
	tokens   []Token
	current  int
	hadError bool
}

func newParser(tokens []Token) *Parser {
	return &Parser{
		tokens:   tokens,
		current:  0,
		hadError: false,
	}
}

func (p *Parser) expression() Expr {
	return p.comma()
}

func (p *Parser) comma() Expr {
	expr := p.equality()

	for p.match(Comma) {
		op := p.previous()
		right := p.equality()
		expr = BinaryExpr{
			left:     expr,
			operator: op,
			right:    right,
		}
	}

	return expr
}

func (p *Parser) equality() Expr {
	expr := p.comparison()

	for p.match(BangEqual, EqualEqual) {
		op := p.previous()
		right := p.comparison()
		expr = BinaryExpr{
			left:     expr,
			operator: op,
			right:    right,
		}
	}

	return expr
}

func (p *Parser) comparison() Expr {
	expr := p.term()

	for p.match(Greater, GreaterEqual, Less, LessEqual) {
		op := p.previous()
		right := p.term()
		expr = BinaryExpr{
			left:     expr,
			operator: op,
			right:    right,
		}
	}

	return expr
}

func (p *Parser) term() Expr {
	expr := p.factor()

	for p.match(Minus, Plus) {
		op := p.previous()
		right := p.factor()
		expr = BinaryExpr{
			left:     expr,
			operator: op,
			right:    right,
		}
	}

	return expr
}

func (p *Parser) factor() Expr {
	expr := p.unary()

	for p.match(Slash, Star) {
		op := p.previous()
		right := p.unary()
		expr = BinaryExpr{
			left:     expr,
			operator: op,
			right:    right,
		}
	}

	return expr
}

func (p *Parser) unary() Expr {
	if p.match(Bang, Minus) {
		op := p.previous()
		right := p.unary()
		return UnaryExpr{
			operator: op,
			right:    right,
		}
	}

	return p.primary()
}

func (p *Parser) primary() Expr {
	if p.match(False) {
		return LiteralExpr{value: false}
	}
	if p.match(True) {
		return LiteralExpr{value: true}
	}
	if p.match(Nil) {
		return LiteralExpr{value: nil}
	}
	if p.match(Number, String) {
		return LiteralExpr{value: p.previous().literal}
	}

	if p.match(LeftParen) {
		expr := p.expression()
		p.consume(RightParen, "Expected ')' after the expression.")
		return GroupingExpr{expression: expr}
	}
	panic(&ParserError{p.peek(), "Expect expression"})
}

func (p *Parser) parse() Expr {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println("Parse Error:", r.(error))
			p.hadError = true
			hadError = true
			p.synchronize()
		}
	}()

	return p.expression()
}

func (p *Parser) synchronize() {
	p.advance()

	for !p.isAtEnd() {
		if p.previous().tType == Semicolon {
			return
		}

		switch p.peek().tType {
		case Class:
			return
		case Fun:
			return
		case Var:
			return
		case For:
			return
		case If:
			return
		case While:
			return
		case Print:
			return
		case Return:
			return
		}
		p.advance()
	}
}

func (p *Parser) consume(t TokenType, errMsg string) Token {
	if p.check(t) {
		return p.advance()
	}
	panic(&ParserError{p.peek(), errMsg})
}

func (p *Parser) previous() Token {
	return p.tokens[p.current-1]
}

func (p *Parser) isAtEnd() bool {
	return p.peek().tType == Eof
}

func (p *Parser) peek() Token {
	return p.tokens[p.current]
}

func (p *Parser) check(t TokenType) bool {
	if p.isAtEnd() {
		return false
	}
	return p.peek().tType == t
}

func (p *Parser) advance() Token {
	if !p.isAtEnd() {
		p.current++
	}
	return p.previous()
}

func (p *Parser) match(tokenType ...TokenType) bool {
	for _, t := range tokenType {
		if p.check(t) {
			p.advance()
			return true
		}
	}
	return false
}
