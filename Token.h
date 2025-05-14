#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>
#include "TokenType.h"
#include "Literal.h"

class Token {
public:
    TokenType type;
    std::string lexeme;
    Literal literal;
    int line;

    Token(TokenType type, std::string lexeme, Literal literal, int line);
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

#endif // TOKEN_H