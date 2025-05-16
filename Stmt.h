#ifndef Stmt_H
#define Stmt_H

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "Literal.h"
#include "Value.h"
#include "Expr.h"

using std::shared_ptr;

class Block;
class Expression;
class Var;
class Print;

// Generic visitor interface using templates
template<typename R>
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual R visitBlock(Block* stmt) = 0;
    virtual R visitExpression(Expression* stmt) = 0;
    virtual R visitVar(Var* stmt) = 0;
    virtual R visitPrint(Print* stmt) = 0;
};

// Convenience type aliases for common visitor types
using StmtStringVisitor = StmtVisitor<std::string>;
using VoidVisitor = StmtVisitor<void>;

// Base statement class
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual std::string accept(StmtStringVisitor& visitor) = 0;
    virtual void accept(VoidVisitor& visitor) = 0;
};

class Block : public Stmt {
public:
    Block(const std::vector<shared_ptr<Stmt>>& statements) : statements(statements) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitBlock(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitBlock(this);
    }

    // Fields
    std::vector<shared_ptr<Stmt>> statements;
};

class Expression : public Stmt {
public:
    Expression(const shared_ptr<Expr>& expression) : expression(expression) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitExpression(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitExpression(this);
    }

    // Fields
    shared_ptr<Expr> expression;
};

class Var : public Stmt {
public:
    Var(const Token& name, const shared_ptr<Expr>& initializer) : name(name), initializer(initializer) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitVar(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitVar(this);
    }

    // Fields
    Token name;
    shared_ptr<Expr> initializer;
};

class Print : public Stmt {
public:
    Print(const shared_ptr<Expr>& expression) : expression(expression) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitPrint(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitPrint(this);
    }

    // Fields
    shared_ptr<Expr> expression;
};

#endif // Stmt_H
