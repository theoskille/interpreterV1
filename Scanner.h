#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"

class Scanner {
private:
    static const std::unordered_map<std::string, TokenType> keywords;
    std::string source;
    std::vector<Token*> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    void scanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, std::variant<std::string, double, std::monostate> literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void handleString();
    void handleNumber();
    void handleIdentifier();
    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

public:
    Scanner(std::string source);
    std::vector<Token*> scanTokens();
};

#endif // SCANNER_H 