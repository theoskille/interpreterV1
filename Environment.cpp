#include "Environment.h"
#include "Interpreter.h" // For RuntimeError

using namespace std;

Environment::Environment() {
    enclosing = nullptr;
}

Environment::Environment(Environment& enclosing) {
    this->enclosing = &enclosing;
}

void Environment::define(std::string name, Value value) {
    values[name] = value;
}

Value Environment::get(Token name) {
    if(values.count(name.lexeme) > 0) {
        return values[name.lexeme];
    }

    if(enclosing != nullptr) 
        return enclosing->get(name);
    
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token name, Value value) {
    if(values.count(name.lexeme) > 0) {
        values[name.lexeme] = value;
        return;
    }

    if(enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }
    return environment;
}

Value Environment::getAt(int distance, const std::string& name) {
    return ancestor(distance)->values[name];
}

void Environment::assignAt(int distance, Token name, Value value) {
    ancestor(distance)->values[name.lexeme] = value;
}