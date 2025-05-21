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
class If;
class Expression;
class Var;
class Print;
class While;

// Generic visitor interface using templates
template<typename R>
class StmtVisitor {
public:
    virtual ~StmtVisitor() = default;
    virtual R visitBlock(Block* stmt) = 0;
    virtual R visitIf(If* stmt) = 0;
    virtual R visitExpression(Expression* stmt) = 0;
    virtual R visitVar(Var* stmt) = 0;
    virtual R visitPrint(Print* stmt) = 0;
    virtual R visitWhile(While* stmt) = 0;
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

class If : public Stmt {
public:
    If(const shared_ptr<Expr>& condition, const shared_ptr<Stmt>& thenBranch, const shared_ptr<Stmt>& elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitIf(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitIf(this);
    }

    // Fields
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> thenBranch;
    shared_ptr<Stmt> elseBranch;
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

class While : public Stmt {
public:
    While(const shared_ptr<Expr>& condition, const shared_ptr<Stmt>& body) : condition(condition), body(body) {}

    std::string accept(StmtStringVisitor& visitor) override {
        return visitor.visitWhile(this);
    }

    void accept(VoidVisitor& visitor) override {
        visitor.visitWhile(this);
    }

    // Fields
    shared_ptr<Expr> condition;
    shared_ptr<Stmt> body;
};

#endif // Stmt_H
