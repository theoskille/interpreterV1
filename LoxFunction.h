#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include <memory>
#include <string>
#include <vector>
#include "LoxCallable.h"
#include "Stmt.h"
#include "Environment.h"

class LoxFunction : public LoxCallable {
private:
    Function declaration;
    Environment* closure;  // The environment where the function was defined

public:
    LoxFunction(Function* declaration, Environment* closure)
        : declaration(*declaration), closure(closure) {}

    // Implement LoxCallable interface
    Value call(Interpreter* interpreter, const std::vector<Value>& arguments) override;
    int arity() const override {
        return declaration.params.size();
    }
    std::string toString() const override {
        return "<fn " + declaration.name.lexeme + ">";
    }
};

#endif // LOX_FUNCTION_H 