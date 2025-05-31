#include "Interpreter.h"
#include "LoxBuiltinFunctions.h"
#include "LoxFunction.h"
#include <iostream>

using namespace std;

Interpreter::Interpreter() {
    globals = new Environment();
    environment = globals;
    
    // Define built-in functions
    auto builtins = getBuiltinFunctions();
    for (const auto& [name, function] : builtins) {
        globals->define(name, Value(function));
    }
}

Interpreter::~Interpreter() {
    delete environment;
}

void Interpreter::interpret(const vector<shared_ptr<Stmt>>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(statement.get());
        }
    } catch (RuntimeError& error) {
        Lox::runtimeError(error);
    }
}

// Statement visitor methods
void Interpreter::visitExpression(Expression* stmt) {
    evaluate(stmt->expression.get());
    // We discard the value since this is an expression statement
}

void Interpreter::visitIf(If* stmt) {
    if(evaluate(stmt->condition.get()).isTruthy()) {
        execute(stmt->thenBranch.get());
    } else if(stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch.get());
    }
}

void Interpreter::visitWhile(While* stmt) {
    while(evaluate(stmt->condition.get()).isTruthy()) {
        execute(stmt->body.get());
    }
}

void Interpreter::visitPrint(Print* stmt) {
    Value value = evaluate(stmt->expression.get());
    cout << value.toString() << endl;
}

void Interpreter::visitVar(Var* stmt) {
    Value value;  // Default constructor creates a nil value
    if(stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer.get());
    }

    environment->define(stmt->name.lexeme, value);
}

void Interpreter::visitBlock(Block* stmt) {
    executeBlock(stmt->statements, new Environment(*environment));
}

void Interpreter::visitFunction(Function* stmt) {
    // Create a function object and define it in the current environment
    auto function = make_shared<LoxFunction>(stmt, environment);
    Value functionValue;
    functionValue = Value(std::static_pointer_cast<LoxCallable>(function));
    environment->define(stmt->name.lexeme, functionValue);
}

void Interpreter::visitReturn(Return* stmt) {
    Value value; // Default to nil
    
    // Evaluate the return value if provided
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value.get());
    }
    
    // Throw a ReturnException to unwind the call stack
    throw ReturnException(value);
}

// Add the implementation for the resolve method
void Interpreter::resolve(Expr* expr, int depth) {
    locals[static_cast<void*>(expr)] = depth;
}

// Helper method for executing statements
void Interpreter::execute(Stmt* stmt) {
    stmt->accept(*this);
}

void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements, Environment* newEnvironment) {
    Environment* previous = environment;
    try {
        environment = newEnvironment;

        for(const auto& statement : statements) {
            execute(statement.get());
        }
    } catch (...) {
        environment = previous;
        delete newEnvironment;
        throw;
    }
    
    environment = previous;
    delete newEnvironment;
}

// New helper method for looking up variables
Value Interpreter::lookUpVariable(const Token& name, Expr* expr) {
    for (const auto& [key, distance] : locals) {
        if (key == static_cast<void*>(expr)) {
            return environment->getAt(distance, name.lexeme);
        }
    }
    return globals->get(name);
}

// Update visitVariable to use the lookUpVariable helper
Value Interpreter::visitVariable(Variable* expr) {
    return lookUpVariable(expr->name, expr);
}

// Update visitAssign to use the lookUpVariable helper for resolved variables
Value Interpreter::visitAssign(Assign* expr) {
    Value value = evaluate(expr->value.get());

    bool found = false;
    for (const auto& [key, distance] : locals) {
        if (key == static_cast<void*>(expr)) {
            environment->assignAt(distance, expr->name, value);
            found = true;
            break;
        }
    }
    
    if (!found) {
        globals->assign(expr->name, value);
    }

    return value;
}

// Expression visitor methods
Value Interpreter::visitLiteralExpr(LiteralExpr* expr) {
    return literalToValue(expr->value);
}

Value Interpreter::visitLogical(Logical* expr) {
    Value left = evaluate(expr->left.get());

    if(expr->op.type == OR) {
        if(left.isTruthy())
            return left;
    } else {
        if(!left.isTruthy())
            return left;
    }
    
    return evaluate(expr->right.get());
}

Value Interpreter::visitGrouping(Grouping* expr) {
    return evaluate(expr->expression.get());
}

Value Interpreter::visitUnary(Unary* expr) {
    Value right = evaluate(expr->right.get());
    
    switch (expr->op.type) {
        case MINUS:
            checkNumberOperand(expr->op, right);
            return Value(-right.getNumber());
        case BANG:
            return Value(!right.isTruthy());
        default:
            // Unreachable - Parser ensures only MINUS and BANG are used for unary operators
            return Value();
    }
}

Value Interpreter::visitBinary(Binary* expr) {
    Value left = evaluate(expr->left.get());
    Value right = evaluate(expr->right.get());
    
    switch (expr->op.type) {
        // Arithmetic operations
        case MINUS:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() - right.getNumber());
        case SLASH:
            checkNumberOperands(expr->op, left, right);
            if (right.getNumber() == 0) {
                throw RuntimeError(expr->op, "Division by zero.");
            }
            return Value(left.getNumber() / right.getNumber());
        case STAR:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() * right.getNumber());
        case PLUS:
            if (left.isNumber() && right.isNumber()) {
                return Value(left.getNumber() + right.getNumber());
            }
            if (left.isString() && right.isString()) {
                return Value(left.getString() + right.getString());
            }
            throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
            
        // Comparison operations
        case GREATER:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() > right.getNumber());
        case GREATER_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() >= right.getNumber());
        case LESS:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() < right.getNumber());
        case LESS_EQUAL:
            checkNumberOperands(expr->op, left, right);
            return Value(left.getNumber() <= right.getNumber());
            
        // Equality operations
        case BANG_EQUAL:
            return Value(left != right);
        case EQUAL_EQUAL:
            return Value(left == right);
        default:
            // Unreachable - Parser ensures only valid binary operators are used
            return Value();
    }
}

Value Interpreter::visitCall(Call* expr) {
    Value callee = evaluate(expr->callee.get());

    vector<Value> arguments;
    for(const auto& argument : expr->arguments) {
        arguments.push_back(evaluate(argument.get()));
    }

    if (!callee.isCallable()) {
        throw RuntimeError(expr->paren, "Can only call functions and classes.");
    }

    shared_ptr<LoxCallable> function = callee.getCallable();
    
    // Check argument count
    if (arguments.size() != function->arity()) {
        throw RuntimeError(expr->paren, 
            "Expected " + std::to_string(function->arity()) + 
            " arguments but got " + std::to_string(arguments.size()) + ".");
    }
    
    return function->call(this, arguments);
}

Value Interpreter::evaluate(Expr* expr) {
    return expr->accept(*this);
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (operand.isNumber()) return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (left.isNumber() && right.isNumber()) return;
    throw RuntimeError(op, "Operands must be numbers.");
}

Value Interpreter::literalToValue(const Literal& literal) {
    if (literal.isNull()) return Value();
    if (literal.isNumber()) return Value(literal.getNumber());
    if (literal.isString()) return Value(literal.getString());
    if (literal.isBoolean()) return Value(literal.getBoolean());
    
    // Should never happen
    return Value();
} 