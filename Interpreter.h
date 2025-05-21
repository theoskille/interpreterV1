#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdexcept>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include "Value.h"
#include "Lox.h"
#include "Environment.h"

// Custom exception for Interpreter runtime errors
class RuntimeError : public std::runtime_error {
private:
    Token token;
    
public:
    RuntimeError(const std::string& message) 
        : std::runtime_error(message), token(TokenType::EOF_TOKEN, "", Literal(), 0) {}
    
    RuntimeError(const Token& token, const std::string& message)
        : std::runtime_error(message), token(token) {}
        
    const Token& getToken() const { return token; }
};

// Multiple inheritance to implement both visitor interfaces
class Interpreter : public ExprVisitor<Value>, public StmtVisitor<void> {
public:
    // Constructor and destructor
    Interpreter();
    ~Interpreter();

    // Main interpret method
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
    
    // Expression visitor implementation
    Value visitAssign(Assign* expr) override;
    Value visitBinary(Binary* expr) override;
    Value visitGrouping(Grouping* expr) override;
    Value visitLiteralExpr(LiteralExpr* expr) override;
    Value visitLogical(Logical* expr) override;
    Value visitUnary(Unary* expr) override;
    Value visitVariable(Variable* expr) override;

    // Statement visitor implementation
    void visitBlock(Block* stmt) override;
    void visitExpression(Expression* stmt) override;
    void visitIf(If* stmt) override;
    void visitPrint(Print* stmt) override;
    void visitVar(Var* stmt) override;
    void visitWhile(While* stmt) override;

private:
    Environment* environment;
    // Helper methods for evaluating expressions
    Value evaluate(Expr* expr);
    
    // Helper method for executing statements
    void execute(Stmt* stmt);
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, Environment* environment);
    
    // Helper for checking number operands
    void checkNumberOperand(const Token& op, const Value& operand);
    void checkNumberOperands(const Token& op, const Value& left, const Value& right);
    
    // Converting Literal to Value
    Value literalToValue(const Literal& literal);
};

#endif // INTERPRETER_H 