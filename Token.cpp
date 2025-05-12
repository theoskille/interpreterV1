#include "Token.h"
using namespace std;

Token::Token(TokenType type, string lexeme, variant<string, double, monostate> literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

string Token::toString() const {
    string literalStr;
    if (holds_alternative<string>(literal)) {
        literalStr = get<string>(literal);
    } else if (holds_alternative<double>(literal)) {
        literalStr = to_string(get<double>(literal));
    } else {
        literalStr = "null";
    }
    
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