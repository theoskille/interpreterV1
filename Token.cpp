#include "Token.h"
using namespace std;

Token::Token(TokenType type, string lexeme, Literal literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

string Token::toString() const {
    string literalStr = literal.toString();
    return string(TOKEN_NAMES[type]) + " " + lexeme + " " + literalStr;
}

ostream& operator<<(ostream& os, const Token& token) {
    os << token.toString();
    return os;
}

// Uncomment when needed:
/*
string Token::toString() {
    return type + " " + lexeme + " " + literal;
}
*/ 