#ifndef Expr_H
#define Expr_H

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "Literal.h"

using std::shared_ptr;

class Binary;
class Grouping;
class LiteralExpr;
class Unary;

// String visitor interface for AstPrinter
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual std::string visitBinary(Binary* expr) = 0;
    virtual std::string visitGrouping(Grouping* expr) = 0;
    virtual std::string visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual std::string visitUnary(Unary* expr) = 0;
};

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string accept(Visitor& visitor) = 0;
};

class Binary : public Expr {
public:
    Binary(const shared_ptr<Expr>& left, const Token& op, const shared_ptr<Expr>& right) : left(left), op(op), right(right) {}

    std::string accept(Visitor& visitor) override {
        return visitor.visitBinary(this);
    }

    // Fields
    shared_ptr<Expr> left;
    Token op;
    shared_ptr<Expr> right;
};

class Grouping : public Expr {
public:
    Grouping(const shared_ptr<Expr>& expression) : expression(expression) {}

    std::string accept(Visitor& visitor) override {
        return visitor.visitGrouping(this);
    }

    // Fields
    shared_ptr<Expr> expression;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(Literal value) : value(value) {}

    std::string accept(Visitor& visitor) override {
        return visitor.visitLiteralExpr(this);
    }

    // Fields
    Literal value;
};

class Unary : public Expr {
public:
    Unary(const Token& op, const shared_ptr<Expr>& right) : op(op), right(right) {}

    std::string accept(Visitor& visitor) override {
        return visitor.visitUnary(this);
    }

    // Fields
    Token op;
    shared_ptr<Expr> right;
};

#endif // Expr_H
