#ifndef LOX_CALLABLE_H
#define LOX_CALLABLE_H

#include <vector>
#include "Value.h"

// Forward declarations
class Interpreter;

class LoxCallable {
public:
    virtual ~LoxCallable() = default;
    virtual Value call(Interpreter* interpreter, const std::vector<Value>& arguments) = 0;
    virtual int arity() const = 0;  // Number of arguments the function expects
    virtual std::string toString() const = 0;
};

#endif // LOX_CALLABLE_H 