package main

import "fmt"

type ASTPrinter struct{}

func (a *ASTPrinter) print(expr Expr) string {
	return a.accept(a, expr)
}

func (a *ASTPrinter) visitBinaryExpr(expr *BinaryExpr) any {
	return a.parenthesize(expr.operator.lexeme, expr.left, expr.right)
}

func (a *ASTPrinter) visitUnaryExpr(expr *UnaryExpr) any {
	return a.parenthesize(expr.operator.lexeme, expr.right)
}

func (a *ASTPrinter) visitGroupingExpr(expr *GroupingExpr) any {
	return a.parenthesize("group", expr.expression)
}

func (a *ASTPrinter) visitLiteralExpr(expr *LiteralExpr) any {
	return fmt.Sprintf("%v", expr.value)
}

func (a *ASTPrinter) parenthesize(name string, exprs ...Expr) string {
	var s string
	s += fmt.Sprintf("(%s", name)
	for _, ex := range exprs {
		s += " "
		s += a.accept(a, ex)
	}
	s += ")"
	return s
}

func (a *ASTPrinter) accept(visitor Visitor, ex Expr) string {
	v := ex.accept(visitor)
	switch v := v.(type) {
	case string:
		return v
	default:
		return ""
	}
}
