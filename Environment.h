#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "Value.h"
#include "Token.h"

// Forward declare RuntimeError
class RuntimeError;

class Environment {
    private:
        std::unordered_map<std::string, Value> values;
        Environment* enclosing;

    public:
        Environment();
        Environment(Environment& enclosing);
        void define(std::string name, Value value);
        Value get(Token name);
        void assign(Token name, Value value);
};

#endif // ENVIRONMENT_H