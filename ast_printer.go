package main

import "fmt"

type AstPrinter struct{}

func (a AstPrinter) print(e Expr) string {
	str, _ := e.accept(a)
	return fmt.Sprintf("%v", str)
}

func (a AstPrinter) visitLiteralExpr(e LiteralExpr) (any, error) {
	if e.value == nil {
		return nil, nil
	}
	return fmt.Sprintf("%v", e.value), nil
}

func (a AstPrinter) visitUnaryExpr(e UnaryExpr) (any, error) {
	return a.parenthesize(e.operator.lexeme, e.right), nil
}

func (a AstPrinter) visitBinaryExpr(e BinaryExpr) (any, error) {
	return a.parenthesize(e.operator.lexeme, e.left, e.right), nil
}

func (a AstPrinter) visitGroupingExpr(e GroupingExpr) (any, error) {
	return a.parenthesize("group", e.expr), nil
}

func (a AstPrinter) parenthesize(name string, exprs ...any) string {
	var builder string
	builder += "(" + name
	for _, expr := range exprs {
		if expr == nil {
			builder += "<nil>"
			continue
		}
		ast, _ := expr.(Expr).accept(a)
		builder += " "
		switch ast := ast.(type) {
		case string:
			builder += fmt.Sprintf("%v", ast)
		case nil:
			builder += "<nil>"
		case fmt.Stringer:
			builder += fmt.Sprintf("%v", ast)
		}
	}
	builder += ")"
	return builder
}
