#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <ostream>
#include "TokenType.h"

class Token {
private:
    TokenType type;
    std::string lexeme;
    std::variant<std::string, double, std::monostate> literal;
    int line;

public:
    Token(TokenType type, std::string lexeme, std::variant<std::string, double, std::monostate> literal, int line);
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

#endif // TOKEN_H 