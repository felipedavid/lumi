package main

type Visitor interface {
	visitBinaryExpr(expr *BinaryExpr) any
	visitUnaryExpr(expr *UnaryExpr) any
	visitGroupingExpr(expr *GroupingExpr) any
	visitLiteralExpr(expr *LiteralExpr) any
}

type Expr interface {
	accept(v Visitor) any
}

type BinaryExpr struct {
	left     Expr
	operator Token
	right    Expr
}

func (b BinaryExpr) accept(v Visitor) any {
	return v.visitBinaryExpr(&b)
}

type UnaryExpr struct {
	operator Token
	right    Expr
}

func (u UnaryExpr) accept(v Visitor) any {
	return v.visitUnaryExpr(&u)
}

type GroupingExpr struct {
	expression Expr
}

func (g GroupingExpr) accept(v Visitor) any {
	return v.visitGroupingExpr(&g)
}

type LiteralExpr struct {
	value any
}

func (l LiteralExpr) accept(v Visitor) any {
	return v.visitLiteralExpr(&l)
}
