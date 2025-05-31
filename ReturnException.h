#ifndef RETURN_EXCEPTION_H
#define RETURN_EXCEPTION_H

#include <stdexcept>
#include "Value.h"

// Exception thrown when a return statement is executed
class ReturnException : public std::runtime_error {
public:
    Value value; // The returned value
    
    ReturnException(const Value& value)
        : std::runtime_error("Return from function"), value(value) {}
};

#endif // RETURN_EXCEPTION_H 