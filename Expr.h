#ifndef Expr_H
#define Expr_H

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "Literal.h"
#include "Value.h"

using std::shared_ptr;

class Assign;
class Binary;
class Grouping;
class LiteralExpr;
class Variable;
class Unary;

// Generic visitor interface using templates
template<typename R>
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual R visitAssign(Assign* expr) = 0;
    virtual R visitBinary(Binary* expr) = 0;
    virtual R visitGrouping(Grouping* expr) = 0;
    virtual R visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual R visitVariable(Variable* expr) = 0;
    virtual R visitUnary(Unary* expr) = 0;
};

// Convenience type aliases for common visitor types
using ExprStringVisitor = ExprVisitor<std::string>;
using ValueVisitor = ExprVisitor<Value>;

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string accept(ExprStringVisitor& visitor) = 0;
    virtual Value accept(ValueVisitor& visitor) = 0;
};

class Assign : public Expr {
public:
    Assign(const Token& name, const shared_ptr<Expr>& value) : name(name), value(value) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitAssign(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitAssign(this);
    }

    // Fields
    Token name;
    shared_ptr<Expr> value;
};

class Binary : public Expr {
public:
    Binary(const shared_ptr<Expr>& left, const Token& op, const shared_ptr<Expr>& right) : left(left), op(op), right(right) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitBinary(this);
    }

    Value accept(ValueVisitor& visitor) override {
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

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitGrouping(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitGrouping(this);
    }

    // Fields
    shared_ptr<Expr> expression;
};

class LiteralExpr : public Expr {
public:
    LiteralExpr(Literal value) : value(value) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitLiteralExpr(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitLiteralExpr(this);
    }

    // Fields
    Literal value;
};

class Variable : public Expr {
public:
    Variable(const Token& name) : name(name) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitVariable(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitVariable(this);
    }

    // Fields
    Token name;
};

class Unary : public Expr {
public:
    Unary(const Token& op, const shared_ptr<Expr>& right) : op(op), right(right) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitUnary(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitUnary(this);
    }

    // Fields
    Token op;
    shared_ptr<Expr> right;
};

#endif // Expr_H
