#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>
#include <ostream>
#include <cmath>
#include <stdexcept>

// Value class for interpreter runtime
class Value {
private:
    std::variant<std::string, double, bool, std::monostate> data;

public:
    // Constructors
    Value() : data(std::monostate{}) {}
    Value(std::string val) : data(std::move(val)) {}
    Value(double val) : data(val) {}
    Value(bool val) : data(val) {}
    Value(std::monostate) : data(std::monostate{}) {}

    // Type checks
    bool isString() const { return std::holds_alternative<std::string>(data); }
    bool isNumber() const { return std::holds_alternative<double>(data); }
    bool isBoolean() const { return std::holds_alternative<bool>(data); }
    bool isNil() const { return std::holds_alternative<std::monostate>(data); }
    
    // Value getters with type checking
    std::string getString() const { 
        if (!isString()) throw std::runtime_error("Expected string.");
        return std::get<std::string>(data); 
    }
    
    double getNumber() const { 
        if (!isNumber()) throw std::runtime_error("Expected number.");
        return std::get<double>(data); 
    }
    
    bool getBoolean() const { 
        if (!isBoolean()) throw std::runtime_error("Expected boolean.");
        return std::get<bool>(data); 
    }
    
    // Conversion to string for display
    std::string toString() const {
        if (isString()) return std::get<std::string>(data);
        if (isNumber()) {
            std::string text = std::to_string(std::get<double>(data));
            // Remove trailing zeros
            if (text.find('.') != std::string::npos) {
                text = text.substr(0, text.find_last_not_of('0') + 1);
                if (text.back() == '.') text = text.substr(0, text.size() - 1);
            }
            return text;
        }
        if (isBoolean()) return std::get<bool>(data) ? "true" : "false";
        return "nil";
    }
    
    // Check truthiness according to Lox rules
    bool isTruthy() const {
        if (isNil()) return false;
        if (isBoolean()) return std::get<bool>(data);
        if (isNumber()) return std::get<double>(data) != 0;
        return true; // Strings are always truthy
    }
    
    // Equality
    bool equals(const Value& other) const {
        if (isNil() && other.isNil()) return true;
        if (isNumber() && other.isNumber()) 
            return std::get<double>(data) == std::get<double>(other.data);
        if (isString() && other.isString())
            return std::get<std::string>(data) == std::get<std::string>(other.data);
        if (isBoolean() && other.isBoolean())
            return std::get<bool>(data) == std::get<bool>(other.data);
        return false; // Different types are never equal
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << v.toString();
        return os;
    }
    
    // Convenience operator overloads
    bool operator==(const Value& other) const { return equals(other); }
    bool operator!=(const Value& other) const { return !equals(other); }
};

#endif // VALUE_H 