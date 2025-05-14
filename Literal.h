#ifndef LITERAL_H
#define LITERAL_H

#include <string>
#include <variant>
#include <ostream>

class Literal {
private:
    std::variant<std::string, double, bool, std::monostate> value;

public:
    // Constructors
    Literal() : value(std::monostate{}) {}
    Literal(std::string val) : value(val) {}
    Literal(double val) : value(val) {}
    Literal(bool val) : value(val) {}
    Literal(std::monostate val) : value(val) {}
    
    // Type checks
    bool isString() const { return std::holds_alternative<std::string>(value); }
    bool isNumber() const { return std::holds_alternative<double>(value); }
    bool isBoolean() const { return std::holds_alternative<bool>(value); }
    bool isNull() const { return std::holds_alternative<std::monostate>(value); }
    
    // Value getters
    std::string getString() const { return std::get<std::string>(value); }
    double getNumber() const { return std::get<double>(value); }
    bool getBoolean() const { return std::get<bool>(value); }
    
    // Conversion to string for display/debugging
    std::string toString() const {
        if (isString()) return getString();
        if (isNumber()) return std::to_string(getNumber());
        if (isBoolean()) return getBoolean() ? "true" : "false";
        return "nil";
    }
    
    // Equality operators
    bool operator==(const Literal& other) const {
        return value == other.value;
    }
    
    bool operator!=(const Literal& other) const {
        return !(*this == other);
    }
    
    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Literal& obj) {
        os << obj.toString();
        return os;
    }
};

#endif // LITERAL_H 