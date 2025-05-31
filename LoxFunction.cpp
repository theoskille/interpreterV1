#include "LoxFunction.h"
#include "Interpreter.h"
#include "ReturnException.h"

Value LoxFunction::call(Interpreter* interpreter, const std::vector<Value>& arguments) {
    // Create a new environment for the function execution
    Environment* environment = new Environment(*closure);
    
    // Bind parameters to arguments
    for (size_t i = 0; i < declaration.params.size(); i++) {
        environment->define(declaration.params[i]->lexeme, arguments[i]);
    }
    
    try {
        // Execute the function body
        interpreter->executeBlock(declaration.body, environment);
    } catch (ReturnException& returnValue) {
        // Handle return statements
        return returnValue.value;
    }
    
    // If we get here, the function didn't return a value
    return Value(); // Return nil
} 