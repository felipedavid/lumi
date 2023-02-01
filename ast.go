package main

type Visitor interface {
	visitLiteralExpr(e LiteralExpr) (any, error)
	visitUnaryExpr(e UnaryExpr) (any, error)
	visitBinaryExpr(e BinaryExpr) (any, error)
	visitGroupingExpr(e GroupingExpr) (any, error)
}

type Expr interface {
	accept(visitor Visitor) (any, error)
}

type LiteralExpr struct {
	value any
}

func (e LiteralExpr) accept(visitor Visitor) (any, error) {
	return visitor.visitLiteralExpr(e)
}

type UnaryExpr struct {
	operator Token
	right    Expr
}

func (e UnaryExpr) accept(visitor Visitor) (any, error) {
	return visitor.visitUnaryExpr(e)
}

type BinaryExpr struct {
	left     Expr
	operator Token
	right    Expr
}

func (e BinaryExpr) accept(visitor Visitor) (any, error) {
	return visitor.visitBinaryExpr(e)
}

type GroupingExpr struct {
	expr Expr
}

func (e GroupingExpr) accept(visitor Visitor) (any, error) {
	return visitor.visitGroupingExpr(e)
}
