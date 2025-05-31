#include "Resolver.h"
#include "Lox.h"

Resolver::Resolver(Interpreter& interpreter) : interpreter(interpreter) {}

void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& statements) {
    for (const auto& statement : statements) {
        resolve(statement.get());
    }
}

void Resolver::resolve(Stmt* stmt) {
    stmt->accept(*this);
}

void Resolver::resolve(Expr* expr) {
    expr->accept(*this);
}

void Resolver::beginScope() {
    scopes.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(const Token& name) {
    if (scopes.empty()) return;

    auto& scope = scopes.back();
    if (scope.find(name.lexeme) != scope.end()) {
        Lox::error(name, "Already a variable with this name in this scope.");
    }

    // Mark it as "not ready yet"
    scope[name.lexeme] = false;
}

void Resolver::define(const Token& name) {
    if (scopes.empty()) return;
    // Mark it as fully initialized and ready for use
    scopes.back()[name.lexeme] = true;
}

void Resolver::resolveLocal(Expr* expr, const Token& name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name.lexeme) != scopes[i].end()) {
            // Tell the interpreter at which scope level we found the variable
            interpreter.resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

// Statement visitors
void Resolver::visitBlock(Block* stmt) {
    beginScope();
    for (const auto& statement : stmt->statements) {
        resolve(statement.get());
    }
    endScope();
}

void Resolver::visitExpression(Expression* stmt) {
    resolve(stmt->expression.get());
}

void Resolver::visitFunction(Function* stmt) {
    // Define the function name in the current scope
    declare(stmt->name);
    define(stmt->name);

    // Create a new scope for the function body
    beginScope();
    
    // Define all parameters in the function scope
    for (Token* param : stmt->params) {
        declare(*param);
        define(*param);
    }
    
    // Resolve the function body statements individually
    for (const auto& statement : stmt->body) {
        resolve(statement.get());
    }
    
    endScope();
}

void Resolver::visitIf(If* stmt) {
    resolve(stmt->condition.get());
    resolve(stmt->thenBranch.get());
    if (stmt->elseBranch != nullptr) {
        resolve(stmt->elseBranch.get());
    }
}

void Resolver::visitPrint(Print* stmt) {
    resolve(stmt->expression.get());
}

void Resolver::visitReturn(Return* stmt) {
    if (stmt->value != nullptr) {
        resolve(stmt->value.get());
    }
}

void Resolver::visitVar(Var* stmt) {
    declare(stmt->name);
    if (stmt->initializer != nullptr) {
        resolve(stmt->initializer.get());
    }
    define(stmt->name);
}

void Resolver::visitWhile(While* stmt) {
    resolve(stmt->condition.get());
    resolve(stmt->body.get());
}

// Expression visitors
void Resolver::visitAssign(Assign* expr) {
    resolve(expr->value.get());
    resolveLocal(expr, expr->name);
}

void Resolver::visitBinary(Binary* expr) {
    resolve(expr->left.get());
    resolve(expr->right.get());
}

void Resolver::visitCall(Call* expr) {
    resolve(expr->callee.get());

    for (const auto& argument : expr->arguments) {
        resolve(argument.get());
    }
}

void Resolver::visitGrouping(Grouping* expr) {
    resolve(expr->expression.get());
}

void Resolver::visitLiteralExpr(LiteralExpr* expr) {
    // Nothing to resolve in a literal
}

void Resolver::visitLogical(Logical* expr) {
    resolve(expr->left.get());
    resolve(expr->right.get());
}

void Resolver::visitUnary(Unary* expr) {
    resolve(expr->right.get());
}

void Resolver::visitVariable(Variable* expr) {
    if (!scopes.empty()) {
        auto& scope = scopes.back();
        auto it = scope.find(expr->name.lexeme);
        if (it != scope.end() && it->second == false) {
            Lox::error(expr->name, "Can't read local variable in its own initializer.");
        }
    }

    resolveLocal(expr, expr->name);
}