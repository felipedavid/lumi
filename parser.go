package main

import "errors"

var ParseError = errors.New("error while parsing")

type Parser struct {
	tokens  []Token
	current int
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
		p.consume(RightParen, "Expect ')' after expression")
		return GroupingExpr{expr: expr}
	}
	return nil
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

func (p *Parser) factor() Expr {
	expr := p.unary()

	for p.match(Star, Slash) {
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

func (p *Parser) comparison() Expr {
	expr := p.term()

	for p.match(Less, LessEqual, Greater, GreaterEqual) {
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

func (p *Parser) equality() Expr {
	expr := p.comparison()

	for p.match(EqualEqual, BangEqual) {
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

func (p *Parser) expression() Expr {
	return p.equality()
}

func (p *Parser) consume(t TokenType, errorMessage string) Token {
	if p.check(t) {
		return p.advance()
	}

	lumi.error(p.peek(), errorMessage)
	panic(ParseError)
}

func (p *Parser) match(tt ...TokenType) bool {
	for _, t := range tt {
		if p.check(t) {
			p.advance()
			return true
		}
	}
	return false
}

func (p *Parser) advance() Token {
	if !p.isAtEnd() {
		p.current++
	}
	return p.previous()
}

func (p *Parser) check(t TokenType) bool {
	if p.isAtEnd() {
		return false
	}
	return p.peek().tokenType == t
}

func (p *Parser) previous() Token {
	return p.tokens[p.current-1]
}

func (p *Parser) peek() Token {
	return p.tokens[p.current]
}

func (p *Parser) isAtEnd() bool {
	return p.peek().tokenType == Eof
}
