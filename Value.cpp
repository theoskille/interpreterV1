#include "Value.h"
#include "LoxCallable.h"

std::string Value::toString() const {
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
    if (isCallable()) return std::get<std::shared_ptr<LoxCallable>>(data)->toString();
    return "nil";
} 