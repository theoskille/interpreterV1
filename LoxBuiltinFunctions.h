#ifndef LOX_BUILTIN_FUNCTIONS_H
#define LOX_BUILTIN_FUNCTIONS_H

#include <chrono>
#include <memory>
#include <unordered_map>
#include "LoxCallable.h"

// Forward declaration
class Interpreter;

// Clock function for timing
class ClockFunction : public LoxCallable {
public:
    Value call(Interpreter* interpreter, const std::vector<Value>& arguments) override {
        // Return time in seconds since epoch
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() / 1000.0;
        return Value(seconds);
    }

    int arity() const override {
        return 0;  // Clock takes no arguments
    }

    std::string toString() const override {
        return "<native fn: clock>";
    }
};

// Creates and returns all the built-in functions
inline std::unordered_map<std::string, std::shared_ptr<LoxCallable>> getBuiltinFunctions() {
    std::unordered_map<std::string, std::shared_ptr<LoxCallable>> builtins;
    
    builtins["clock"] = std::make_shared<ClockFunction>();
    
    return builtins;
}

#endif // LOX_BUILTIN_FUNCTIONS_H 