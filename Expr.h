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
class Call;
class Grouping;
class LiteralExpr;
class Logical;
class Variable;
class Unary;

// Generic visitor interface using templates
template<typename R>
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual R visitAssign(Assign* expr) = 0;
    virtual R visitBinary(Binary* expr) = 0;
    virtual R visitCall(Call* expr) = 0;
    virtual R visitGrouping(Grouping* expr) = 0;
    virtual R visitLiteralExpr(LiteralExpr* expr) = 0;
    virtual R visitLogical(Logical* expr) = 0;
    virtual R visitVariable(Variable* expr) = 0;
    virtual R visitUnary(Unary* expr) = 0;
};

// Convenience type aliases for common visitor types
using ExprStringVisitor = ExprVisitor<std::string>;
using ValueVisitor = ExprVisitor<Value>;
using VoidExprVisitor = ExprVisitor<void>;

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string accept(ExprStringVisitor& visitor) = 0;
    virtual Value accept(ValueVisitor& visitor) = 0;
    virtual void accept(VoidExprVisitor& visitor) = 0;
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitAssign(this);
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitBinary(this);
    }

    // Fields
    shared_ptr<Expr> left;
    Token op;
    shared_ptr<Expr> right;
};

class Call : public Expr {
public:
    Call(const shared_ptr<Expr>& callee, const Token& paren, const std::vector<shared_ptr<Expr>>& arguments) : callee(callee), paren(paren), arguments(arguments) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitCall(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitCall(this);
    }
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitCall(this);
    }

    // Fields
    shared_ptr<Expr> callee;
    Token paren;
    std::vector<shared_ptr<Expr>> arguments;
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitGrouping(this);
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitLiteralExpr(this);
    }

    // Fields
    Literal value;
};

class Logical : public Expr {
public:
    Logical(const shared_ptr<Expr>& left, const Token& op, const shared_ptr<Expr>& right) : left(left), op(op), right(right) {}

    std::string accept(ExprStringVisitor& visitor) override {
        return visitor.visitLogical(this);
    }

    Value accept(ValueVisitor& visitor) override {
        return visitor.visitLogical(this);
    }
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitLogical(this);
    }

    // Fields
    shared_ptr<Expr> left;
    Token op;
    shared_ptr<Expr> right;
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitVariable(this);
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
    
    void accept(VoidExprVisitor& visitor) override {
        visitor.visitUnary(this);
    }

    // Fields
    Token op;
    shared_ptr<Expr> right;
};

#endif // Expr_H
