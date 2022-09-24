package main

import "fmt"

type RuntimeError struct {
	token  Token
	errMsg string
}

func (r *RuntimeError) Error() string {
	return fmt.Sprintf("line #%d at '%v': '%s'", r.token.line, r.token.lexeme, r.errMsg)
}

type ParserError RuntimeError

func (p *ParserError) Error() string {
	if p.token.tType == Eof {
		return fmt.Sprintf("line #%d at end: %s", p.token.line, p.errMsg)
	}
	return fmt.Sprintf("line #%d at '%v': %s", p.token.line, p.token.lexeme, p.errMsg)
}
